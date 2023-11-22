### golang并发编程基础
#### 1. 什么是sync.Cond？ (5_2023_11_22)
sync.cond是golang用来协调多个goroutine的重要手段之一，常用于当某个条件发生的时候，通知多个goroutine

#### 2. sync.cond 数据结构主要字段？  (5_2023_11_22)
```
type Cond struct {
    noCopy noCopy
    // L is held while observing or changing the condition
    L Locker
    notify  notifyList
    checker copyChecker
}
```

#### 3. sync.cond 包含哪些方法以及用法？   (5_2023_11_22)
```c++
1. func NewCond(l Locker) *Cond

样例:
cond := sync.NewCond(&sync.Mutex{})


2. func (c *Cond) Broadcast()
广播唤醒所有


3. func (c *Cond) Signal()
唤醒一个协程


4. func (c *Cond) Wait()
等待
内部逻辑:
1.将自己放入到等待队列中
2.释放锁
3.等待唤醒
4.重新加锁

标准用法:
c.L.Lock()
for !condition() {
    c.Wait()
}
// 满足条件情况下,执行的逻辑
c.L.Unlock()
```

#### 4. sync.cond那些地方一定要加锁？ (5_2023_11_22)
1. wait方法一定要先加锁，因为wait方法里面会释放锁
2. 对条件的修改一定要加锁 => 保证并发安全

#### 5. sync.cond 为什么不可复制?  (5_2023_11_22)
1. 因为sync.cond的结构体有指针，复制的时候发生浅拷贝，可能会发生无法预知的问题

#### 6. 为什么要用for 循环包裹wait()方法，if不行吗？  (5_2023_11_22)
如果使用if的话，当goroutine被唤醒的时候，其不一定满足条件，也会执行后续的代码

#### 7. atomic包提供了那些原子操作？ (5_2023_11_22)
1. ADD()
2. Load()
3. Store()
4. Swap()
5. Cas()

#### 8. 什么是CAS操作？  (5_2023_11_22)
compare and swap 比较交换，其有3个值，当前内存值，旧值，新值，只有当内存值等于旧值的时候，才会把内存值修改成新值，是乐观锁实现的原理之一


#### 9. cas操作的优缺点？ (5_2023_11_22)
优点：
1. 乐观锁，避免加锁和解锁带来的开销

缺点：
1. 在并发大的场景下，长时间自旋，浪费CPU资源
2. ABA问题 => 修改变量加一个版本
3. 只能保证一个变量的原子性

#### 10. 什么是sync.once?  (5_2023_11_22)
sync.once 使得函数无论被调用多次，但是只执行一次


#### 11. sync.once的结构体字段？ (5_2023_11_22)
```c++
type Once struct {
    done uint32
    m    Mutex
}
```


#### 12. Do()方法实现的大概逻辑？ (5_2023_11_22)
1. 判断done是不是等于0，如果等于0则表示未执行过，如果不等于0则表示已经执行过了
2. 没有执行过，加锁，然后执行函数，然后修改done的值为1


#### 13. 什么是waitGroup? (5_2023_11_22)
waitGroup是golang协调多个goroutine的重要手段之一，他的使用场景是等待所有线程都执行完，再一起往下执行


#### 14. waitGroup有那些方法，如何使用？  (5_2023_11_22)

```c++
add() => 正在执行的线程加1
done() => 正在执行的线程减1 => 如果等于0则唤醒所有被阻塞的goroutine
Wait() => 如果正在执行的线程等于0
```

#### 15. 什么是sync.Pool? (5_2023_11_22)
sync.Pool用于存储临时对象，避免频繁的创建和销毁对象，带来较大的GC压力

#### 16. sync.Pool结构体的主要字段？ (5_2023_11_22)
```c++
type Pool struct {
	noCopy noCopy

	local     unsafe.Pointer // local fixed-size per-P pool, actual type is [P]poolLocal
	localSize uintptr        // size of the local array

	victim     unsafe.Pointer // local from previous cycle
	victimSize uintptr        // size of victims array

	New func() interface{}
}
```

noCopy: 用来检验是否被拷贝

local: 是一个数组，长度为P的个数

localSize： 数组的长度，因为P的个数是可以在运行时动态修改的，所以得通过一个变量来记录localSize这个值

new: 创建新对象的函数

![image](https://github.com/Luozujian/architect/assets/27532970/3dd8dc0c-4f0a-4f7d-b901-0d7d200235b1)



#### 17. 本地对象池结构的主要字段？ (4_2023_11_22)
```c++
type poolLocal struct {
	poolLocalInternal

	pad [128 - unsafe.Sizeof(poolLocalInternal{})%128]byte
}

type poolLocalInternal struct {
	private interface{}
        shared  poolChain
```

#### 18. poolChain结构体的主要字段？(4_2023_11_22)
```c++
type poolChain struct {
	head *poolChainElt
	tail *poolChainElt
}
```

#### 19. pollChainElt结构体的主要字段？(4_2023_11_22)
```c++
type poolChainElt struct {
	poolDequeue
	next, prev *poolChainElt
}

type poolDequeue struct {
	headTail uint64
	vals []eface
}

```



#### 18. put() 方法的大概逻辑？  (4_2023_11_22)
1. 执行pool.pin方法
2. 如果l.private 为空的话，则直接放入到private
3. 否则放入到poolChain里面去


#### 19. pool.pin方法大概逻辑？ (4_2023_10_22)
1. 防止当前P被抢占，golang1.14以后实现了抢占式调度，避免在执行过程中被抢占，然后被唤醒的时候再其他P中执行，这样执行过程会有问题
2. 如果当前P小于数组的长度，则直接返回当前P对应的poolLocal数组
3. 否则根据当前P的个数重新初始化

#### 20. poolChain.pushHead的大概逻辑?  (4_2023_11_22)
1. 如果head所指向的节点为null，则创建一个节点，环形数组大小为8，tail也指向该节点
2. 如果head所指向的节点，环形数组未满，则放入元素，返回成功
3. 如果满了，则创建一个2倍于上一个节点环形数组大小的数组
4. 然后将元素放进去

##### 21. poolChain.popHead的大概逻辑？(4_2023_11_22)
1. 链表从head往tail取数据，环形数组也是从head往tail取数据，用cas来保证安全性

##### 22. poolChain.popTail的大概逻辑？(4_2023_11_22)
1. 链表从tail往head取数据，环形数组也是从tail往head取数据，用cas来保证安全性

##### 23. pushHead和popHead会有并发问题吗？ (3_2023_11_22)
不会有，只有popTail可能会和其他操作有并发问题，因为只有get方法才有可能从其他P对应的localPool窃取数据

##### 24. popTail和popHead会有并发问题吗？(3_2023_11_22)
可能会有，但是通过cas修改headTail变量，保证并发安全

##### 25. popTail和pushHead会有并发问题吗？(3_2023_11_22)
不会有，一个是修改tail变量，一个是修改head变量



//TODO
1. 环形队列判断满和空的方法 ？



#### 参考资料：

1. [ ] [sync.cond 基本用法](https://geektutu.com/post/hpg-sync-cond.html)
2. [ ] [atomic包的常见用法](https://gfw.go101.org/article/concurrent-atomic-operation.html)
3. [ ] [sync.Pool底层实现分析](https://www.cyhone.com/articles/think-in-sync-pool/)
4. [ ] [ABA问题](https://xie.infoq.cn/article/ef6528937025e72362beaca1c)
