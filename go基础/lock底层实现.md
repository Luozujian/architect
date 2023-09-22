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





#### 参考资料：
1. [ ][lock 底层实现](https://juejin.cn/post/7086756462059323429)
