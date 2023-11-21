### 1. lock底层实现


##### 1.1 lock是用来做什么的？(5_2023_11_21)
lock即互斥锁，是Golang在并发场景下对共享资源进行访问控制的重要手段之一


##### 1.2 lock可重入嘛？(5_2023_11_21)
不可重入


##### 1.3 Golang为什么不支持可重入锁呢？ (5_2023_11_21)
Go开发者认为，当你要使用重入锁的时候，那就代表你代码设计的有问题，因为你可以在上层加一个互斥锁即可。所以Go不支持可重入锁



##### 1.4 那如何实现可重入锁呢？(5_2023_11_21)
需要新增两个字段:
1.当前加锁的Goroutine 唯一标识: token
2.当前加锁的次数: count

加锁操作:
1. 如果当前token等于加锁的token，则加锁成功，count++

释放锁操作:
1. --count，如果count == 0，则释放锁



##### 1.5 lock的主要结构体是啥？(5_2023_11_21)
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

![image](https://github.com/Luozujian/architect/assets/27532970/bfb50adc-d0fb-44be-9bf7-4cf126fdda03)

state，是一个32位的变量，高29位Waiter表示阻塞在该信号量的Goroutine个数，第3位分别表示是否需要唤醒Goroutine，是否被加锁，是否处于饥饿状态

Locked: 表示该 Mutex 是否已经被锁定，0表示没有锁定，1表示已经被锁定；

Woken: 表示是否有协程已经被唤醒，0表示没有协程唤醒，1表示已经有协程唤醒，正在加锁过程中；

Starving: 表示该 Mutex 是否处于饥饿状态，0表示没有饥饿，1表示饥饿状态，说明有协程阻塞了超过1ms；

Waiter: 它表示阻塞等待锁的协程个数，协程解锁时根据此值来判断是否需要释放信号量。


##### 1.6 什么是公平锁，什么是非公平锁？ (5_2023_11_21)
公平锁: 获取锁的顺序和请求锁的顺序一致，遵循按先来先得的规则，即为公平锁
非公平锁: 就不遵循先来先得的规则

公平锁局部最优 => 整体并非最优

##### 1.7 互斥锁的两种模式？ (5_2023_11_21)
正常模型: 当Lock已经被占用之后，新Goroutine进来会先自旋获取锁，当Lock被释放之后，会唤醒一个被阻塞的Goroutine，这个时候通常被阻塞的Goroutine是竞争不过新Gorotuine的，因为新Goroutine一直占用着CPU，而被唤醒的Goroutine还要等待调度，所以正常模式下Lock不是公平锁

饥饿模式: 当Lock已经被占用之后，新Goroutine会直接加入到阻塞队列的队尾，当Lock释放后，唤醒第一个阻塞的Goroutine，饥饿模式下是公平锁

#### 1.8 正常模式与饥饿模式的转换？(4_2023_10_24)
饥饿模式: 当一个Goroutine等待的时间超过1ms就会转化为饥饿模式
正常模式: 
1. 当一个Goroutine等待的时间少于1ms
2. 阻塞队列中最后一个Gorotuine

#### 1.9 自旋的条件？(4_2023_10_24)
1. 自旋的次数不能很多，通常为4
2. CPU的数量要大于1，否则不会有其他协程释放锁
3. 调度机制中的process要大于1，否则也不会有其他协程释放锁
4. 调度机制中的可运行队列必须为空，否则会延迟协程调度，需要把CPU让给更需要的进程

#### 1.10 Lock的大概逻辑？(4_2023_10_24)
1. 首先进行一次CAS，看是否可以获取到锁
2. 进入slowLock()
3. 首先判断是否可以自旋，等待锁释放，同时修改锁状态为mutexWoken => 释放锁的时候不要唤醒阻塞的协程
4. 如果不满足自旋条件 1.加锁成功 2.加入阻塞队列
5. 唤醒之后 => 1.是否要取消饥饿模式 2.继续竞争锁
```c++
if old&(mutexLocked|mutexStarving) == mutexLocked && runtime_canSpin(iter) {
	// Active spinning makes sense.
	// Try to set mutexWoken flag to inform Unlock
	// to not wake other blocked goroutines.
	if !awoke && old&mutexWoken == 0 && old>>mutexWaiterShift != 0 &&
		atomic.CompareAndSwapInt32(&m.state, old, old|mutexWoken) {
		awoke = true
	}
	runtime_doSpin()
	iter++
	old = m.state
	continue
}
```

#### 1.11 unlock的大概逻辑？(5_2023_11_21)
1. 直接将锁lock标志位清零
2. 然后判断是否是饥饿模式
3. 饥饿模式 => 释放信号量，唤醒队首Goroutine
4. 非饥饿模型 => 如果mutexWoken标识为0，并且有阻塞的元素的话，就唤醒一个G

参考资料:
1. [ ] [可重入锁的实现] https://www.axiszql.com/article/golang-mutex-recursive





#### 参考资料：
1. [ ][lock 底层实现](https://juejin.cn/post/7086756462059323429)
