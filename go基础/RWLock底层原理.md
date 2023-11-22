### rwlock底层实现原理
#### 1. 为什么要有rwlock? (5_2023_11_22)
lock的粒度太粗了，并发操作，读读并不会出现并发问题，只有读写，写写才会出现并发问题，所以为了进一步优化锁的性能，官方推出了读写锁，在读多写少下，可进一步提升并发性能。


#### 2. rwlock底层实现数据结构？ (4_2023_10_24)
```c++
type RWMutex struct {
	w           Mutex        // held if there are pending writers
	writerSem   uint32       // semaphore for writers to wait for completing readers
	readerSem   uint32       // semaphore for readers to wait for completing writers
	readerCount atomic.Int32 // number of pending readers
	readerWait  atomic.Int32 // number of departing readers
}

1. w: 互斥锁
2. writerSem 写信号量
3. readerSem 读信号量
4. readerCount 读的协程个数
5. readerWait 读等待的个数
```

#### 3. lock方法的大概逻辑？ (4_2023_10_24)
1. 加互斥锁
2. 将readerCount设置成一个非常小的负数
3. 如果此时有正在写的goroutine，则修改readerWaiter的值，为正在当前正在写的goroutine数量，然后阻塞
4. 否则直接成功 


#### 4. unlock方法的大概逻辑？（4_2023_10_24）
1. 修改readerCount
2. 根据当前readerCount的数量释放信号量
3. 释放互斥锁


#### 5. rlock方法的大概逻辑？(4_2023_10_24)
1. 修改readerCount的值
2. 如果readerCount的值小于，则代表有正在执行的写goroutine，加入到阻塞队列
3. 否则成功


#### 6. runlock方法的大概逻辑? (4_2023_10_24)
1. 修改readCount的值
2. 如果readerCount的值小于0，则修改readerWait的值
3. 如果readerWait -1 == 0的话，则唤醒阻塞的写goroutine


#### 7. write lock在判断是否还有未完成的读协程时为什么不直接使用r!=0 ？ (4_2023_10_24)
```c++
r := rw.readerCount.Add(-rwmutexMaxReaders) + rwmutexMaxReaders
// Wait for active readers.
if r != 0 && rw.readerWait.Add(r) != 0 {
  runtime_SemacquireRWMutex(&rw.writerSem, false, 0)
}
```

因为代码第1行和第3行不是原子的，r的值可能被修改，这样可以通过后面的条件来避免不必要的阻塞。
具体case:
假设刚好只有一个读goroutine,在执行完第二行代码的时候释放了，这个时候readerWait会就等于-1，加上r刚好等于0，此时就没必要再进入阻塞态




#### 8. 读写锁的优缺点？ (3_2023_10_17)
优点: 
1. 适用于读多写少的场景，读次数/写次数越大，效率相比互斥锁提升越高，当值为1000的时候，benckmack实验显示性能提升95%

缺点:
1. 在写多读少的场景，读写锁的性能比互斥锁要差

golang高性能编程中:
当读写比为9时，此时读写锁性能比互斥锁性能高出8倍
当读写比为5:5时，此时读写锁性能是互斥锁的两倍
当读写比为1:9时，此时读写锁和互斥锁性能相当



参考资料:
1. [ ] [基础介绍-值得一看](https://segmentfault.com/a/1190000039712353)
2. [ ] [读写锁benckmark实验](https://geektutu.com/post/hpg-mutex.html)
