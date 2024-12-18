### kafka基础

#### 1. 什么是kafka？(3_2023_12_1)
kafka是一个分布式的基于发布/订阅分布式消息队列，常用用于大数据实时处理领域


#### 2. 使用消息队列的好处？(3_2023_12_1)
1. 发布订阅场景 => 消息会被多个业务方消费，消息重要则需要回调确认，否则不需要确认
2. 解耦 => 消息生产方和消息消费方解耦，允许对方挂掉
3. 削峰 => 系统可能会瞬时流量非常大，系统无法处理，可以直接将消息丢消息队列，然后异步消费，达到一个削峰的效果，比如日志
4. 异步通信 => 不希望在线处理非常耗时的任务，可以使用kafka让其离线处理

#### 3. kafka 核心概念？ (3_2023_12_1)
producer: 消息生成者

consumer: 消费者

broker: 起kafka的进程

topic: 主题，消息分类

partition: 分区，用于提升topic的负载能力

leader: 针对分区的leader, 还有follow

消费者组: 一个分区只能被一个消费者组中的一个消费者消费

#### 4. topic的数据是存储在磁盘还是内存？ (3_2023_12_4)
存储在磁盘，并且是以topic + 分区id来存储的

#### 5. 分区和副本可以放在同一个broker吗？ (3_2023_12_4)
分区和副本不能放在同一个broker, 也就是说副本数不能超过broker数量

#### 6. topic在磁盘中存储了什么？ (3_2023_12_4)
存储两种文件，.log文件，用来存储数据，.index文件，用来记录索引。.index文件以第一条记录的偏移量作为文件名，这样就可以通过二分算法得到数据在那个文件，然后通过偏移量
减去第一个偏移量，可以在o(1)时间内得到，数据在.log文件中的偏移量

#### 7. kafka 是拉还是推？  (3_2023_12_4)
kafka实现的是拉取模式，客户端实现一个长轮询


#### 8. 为什么要进行分区？ (3_2023_12_4)
1. 便于水平拓展
2. 提升并发能力，以partition为维度进行读写

#### 9. producer 发送数据的策略？ (3_2023_12_04)
1. 如果指明了partition, 则发送到指定的partition
2. 如果指定了key，则对key做个HASH，然后对partition数量取余
3. 如果啥都没指定，则在第一次发送数据的时候生成的随机数，然后对partition数量取模，后续每次+1，其实就是轮询

#### 10. 如何保证producer发送数据的可靠性？   (3_2023_12_04)
ack机制，向partition发送数据，partition收到数据之后，并且根据策略同步给flower节点之后，返回ack确认，客户端收到ack确认后，可继续发送

#### 11. 什么是ISR？ (3_2023_12_04)
与leader保持同步的flower, 如果ISR中的节点长时间未和master节点同步数据，则会被踢出ISR集合，当leader节点故障时，则从ISR里面选举新的leader节点

#### 12. 数据会丢失或者重复发送吗？ (3_2023_12_04)
重复发送: leader还未返回ack，但是已经同步给flower了，然后就挂掉了,然后再向新的节点发送就重复了
数据丢失: leader节点没有ISR，然后挂掉了

#### 13. broker返回ack的策略？ (3_2023_12_04)
1. leader和flower数据同步完成之后，再返回ack，延迟高，最多允许n - 1个节点故障
2. leader和半数节点同步完数据之后，再返回ack，延迟低，最多允许 n/2 - 1个节点故障，就至少要保证有一台flower拿到数据

kafka用的是第1种方案

#### 14. 加入到ISR的条件？  (3_2023_12_04)
1. flower如果10s没有和leader进行数据传递，则踢出ISR

#### 15. ack应答机制有几种？  (3_2023_12_04)
3种分别是:
0: producer不等待broker的ack，这一操作延迟最低，broker接收到消息后，还没写入到磁盘就返回了，当broker出现故障的时候，数据丢失
1: brocker leader partition落库后就返回，如果leader partition对应的brocker挂掉之后，就会出现数据丢失
-1: leader同步数据给所有flower后才返回ack，也可能存在数据丢失的情况

#### 16. 什么是LEO、HW？(3_2023_12_04)
LEO: log end offset，每个副本最后一个offset
HW:  高水位，最小的LEO

消费数据的一致性: 只有HW前面的数据才是对消费者可见的，用于保证消费的一致性
副本数据一致性: flower当选leader后，会把hw之外的数据全都丢掉，保证副本数据的一致性

#### 17. 什么是exactly once的语义？(3_2023_12_04)
当策略设置成-1的时候，基本上可以保证不丢失数据，实现了at least once的语义，当策略设置成0的时候，基本实现了at most once的语义，即最多一次。
at least once + 幂等就实现了 = exactly once的语义

#### 18. kafka是如何支持幂等性的呢？(3_2023_12_04)
producer在初始化的时候会被分配一个PID,发往partition的消息可附带一个三元组<PID,Partition,SeqNumber>作为主键，相同主键kafka只会持久化一条，幂等性不可跨
分区，不可跨回话，因为kafka重启pid会变化。

#### 19. 消费者分区策略有哪些？(3_2023_12_04)
range: 针对单个topic,消费者个数均分，第一个消费者可能会多
roundRobin: 针对所有topic，把所有partition排序，消费者排序，然后轮询分配

![image](https://github.com/Luozujian/architect/assets/27532970/6fe97e60-082a-4aa2-9d0c-84ab7d6a2088)
![image](https://github.com/Luozujian/architect/assets/27532970/753023fa-78ad-418e-a063-b1a810eca68b)



#### 20. 如何保证消费者正确消费了数据？(3_2023_12_04)
手动发送ack，不要自动ack即可


#### 21. 




![image](https://github.com/Luozujian/architect/assets/27532970/011e6a7c-0b9d-4128-b9a5-bdce18f182f4)



参考资料:
1. [ ] [kafka消费者那些事儿](https://baijiahao.baidu.com/s?id=1767477534734641183&wfr=spider&for=pc)
2. [ ] [kafka选举](https://www.cnblogs.com/jing99/p/13870593.html)



