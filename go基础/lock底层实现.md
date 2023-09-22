### 1. lock底层实现


##### 1.1 lock是用来做什么的？(2_2023_09_22)
lock是并发场景下对共享资源进行访问控制的重要手段之一

##### 1.2 lock的主要结构题是啥？(2_2023_09_22)
```c++
// A Locker represents an object that can be locked and unlocked.
type Locker interface {
    Lock()
    Unlock()
}

type Mutex struct {
    state int32 
    sema  uint32
}

const (
	mutexLocked = 1 << iota // mutex is locked
	mutexWoken
	mutexStarving
	mutexWaiterShift = iota
)
```

1. Mutex 是一个互斥锁，其零值对应了未上锁的状态，不能被拷贝
2. state 代表互斥锁的状态，比如是否被锁定
3. sema 表示信号量，协程阻塞会等待该信号量，解锁的协程释放信号量从而唤醒等待信号量的协程

![image](https://github.com/Luozujian/architect/assets/27532970/bfb50adc-d0fb-44be-9bf7-4cf126fdda03)

Locked: 表示该 Mutex 是否已经被锁定，0表示没有锁定，1表示已经被锁定；

Woken: 表示是否有协程已经被唤醒，0表示没有协程唤醒，1表示已经有协程唤醒，正在加锁过程中；

Starving: 表示该 Mutex 是否处于饥饿状态，0表示没有饥饿，1表示饥饿状态，说明有协程阻塞了超过1ms；

Waiter: 它表示阻塞等待锁的协程个数，协程解锁时根据此值来判断是否需要释放信号量。


##### 1.3 什么是公平锁，什么是非公平锁？ (2_2023_09_22)
公平锁: 获取锁的顺序和请求锁的顺序一致，遵循按先来先得的规则，即为公平锁
非公平锁: 就不遵循先来先得的规则

##### 1.4 互斥锁锁的两种模式？ (2_2023_09_22)
正常模型: 当Lock已经被占用之后，新Goroutine进来会先自旋获取锁，当Lock被释放之后，会唤醒一个被阻塞的Goroutine，这个时候通常被阻塞的Goroutine是竞争不过新Gorotuine的，因为新Goroutine一直占用着CPU，而被唤醒的Goroutine还要等待调度，所以正常模式下Lock不是公平锁

饥饿模式: 当Lock已经被占用之后，新Goroutine会直接加入到阻塞队列的队尾，当Lock释放后，唤醒第一个阻塞的Goroutine。饥饿模式下是公平锁

#### 1.5 正常模式与饥饿模式的转换？(2_2023_09_22)
饥饿模式: 当一个Goroutine等待的时间超过1ms就会转化为饥饿模式
正常模式: 
1. 当一个Goroutine等待的时间少于1ms
2. 阻塞队列中最后一个Gorotuine

#### 1.6 自旋的条件？(2_2023_09_22)
1. 自旋的次数不能很多，通常为4
2. CPU的数量要大于1，否则不会有其他协程释放锁
3. 调度机制中的process要大于1，否则也不会有其他协程释放锁
4. 调度机制中的可运行队列必须为空，否则会延迟协程调度，需要把CPU让给更需要的进程

#### 1.7 



#### 参考资料：
1. [ ][lock 底层实现](https://juejin.cn/post/7086756462059323429)
