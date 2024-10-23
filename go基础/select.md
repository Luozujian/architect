# select底层实现

#### 1.什么是select? (2_20241022)
go select与linux操作系统的select类似，为golang提供了一种IO多路复用机制，用于监听channel的读写事件是否就绪。

#### 2.select 有那两个特点？ (2_20241022)
* select 能进行非阻塞操作，用default即可，waitgroup指关心是否存在失败的任务
* 多个channel准备就绪时，随机执行其中一个case


#### 3. select中的case底层结构体是啥？  (2_20241022)
select在Go的源代码中并不存在对应的结构体，用runtime.scase表示select控制结构中的case，c为case对应的channel，elem指向case对应的代码
```
type scase struct {
	c    *hchan         // chan
	elem unsafe.Pointer // data element
}
```
#### 4. select与case对应的结构如何？  (2_20241022)
select在编译期间会被转换成oselect节点。每个oselect节点持有一组ocase节点，如果某个ocase节点的条件为空，则是default节点

![image](https://github.com/user-attachments/assets/2f4e4f74-b9ae-47ba-983a-db2dcdbde2b6)


#### 5. select 总共有几种情况？  (2_20241022)
* select 不存在任何case，优化后直接block
* select 只有一个case，优化成if语句
* select 拥有2个case，其中一个是default，优化成if else结构，if非阻塞接收或者发送
* select 拥有2个正常case, 执行selectgo函数来挑选被选择的case

#### 6. selectgo函数的初始化动作做了什么？  (2_20241022)
* 确认加锁顺序和轮训顺序

* 加锁顺序通过channel地址排序确认
* 轮训顺序通过runtime.fastrandn引入随机性

#### 7. select的循环操作做了那几个动作？ (2_20241022)
1. 根据轮训顺序查找是否有准备就绪的ch，可执行收发操作
2. 将当前goroutine加入到对应channel的sendq或者recevq队列当中，等待被其他goroutine唤醒
3. 被其他goroutine唤醒后找到满足条件的channel执行case，同时将当前goroutine从其他channel的等待队列中出队

通过对比sg结构在sg链表中的位置确认是第几个scase

#### 8. select 底层实现原理？  (2_20241022)
golang select是一种应用于channel的IO多路复用支持，其具备两个特点，支持非阻塞接收发送channel，多个channel就绪随机选择就绪的channel执行其相关case，保证公平性。






参考资料:
1. [select ](https://draveness.me/golang/docs/part2-foundation/ch05-keyword/golang-select/)
