# goroutine相关面试题

#### 1. go 什么情况下会发生内存泄漏？(2_20241229)
1. Goroutine 生命周期管理不当：比如死循环，waitgroup使用不当，channel阻塞，锁一直不释放

#### 2. 怎么限制goroutine的数量？(2_20241229)
1. 往带缓冲区的channel里面发数据，执行完了取一个数出来
2. 使用waitGroup


#### 3. 怎么查看goroutine的数量？(2_20241229)


#### 4. 通道关闭原则？(2_20241229)
不要在数据接收方或者有多个数据发送者的情况下关闭通道

#### 5. 




参考资料：

1. [优雅关闭channel](https://gfw.go101.org/article/channel-closing.html)
2. 
