### golang并发编程基础
#### 1. 什么是sync.Cond？ (2_2023_10_07)
sync.cond是用来协调各个goroutine访问临街资源的库，常用于当某个条件发生的时候，通知多个goroutine

#### 2. sync.cond 数据结构主要字段？  (2_2023_10_07)
```
type Cond struct {
    noCopy noCopy
    // L is held while observing or changing the condition
    L Locker
    notify  notifyList
    checker copyChecker
}
```

#### 3. sync.cond 包含哪些方法以及用法？   (2_2023_10_07)
1.   




#### 参考资料：

1. [ ] [sync.cond 基本用法](https://geektutu.com/post/hpg-sync-cond.html)
