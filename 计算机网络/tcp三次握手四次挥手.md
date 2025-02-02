# tcp三次握手四次挥手

#### 1. TCP三次握手过程？ (3_2024_12_12)

第一次握手，客户端随机生成一个序号seq = 100，然后发送SYN标识位为1，seq = 100的报文给服务器

第二次握手，服务器接收到请求后，也会随机生成一个序号假设seq = 200，它需要对接收到的报文做确认，所以它会发送一个SYN标记位为1，
ACK标记位为1，序号为200，确认号为100 + 1的报文给客户端，然后服务器进入SYN_RECV状态。

第三次握手，客户端接受报文后，它需要对服务器的报文进行确认，所以它需要发送一个ACK标记位为1，确认号为200 + 1这样一个报文给服务器。然后服务器和
客户端都进入established状态


![image](https://github.com/Luozujian/architect/assets/27532970/a502b127-0f72-4a73-a542-a07e3e76ed3f)

#### 2. TCP三次握手第几次可以携带数据？  (3_2024_12_12)
第三次可以，第三次握手和普通携带数据的报文没啥区别。


#### 3. 为什么不是2次不是4次握手呢？  (3_2024_12_12)
tcp三次握手核心诉求是协商好双方发送数据的起始序号，如果是2次的话，当收到客户端收到第二次握手的时候，它能够确认服务端认可了他的起始序号，但是服务端没有办法
知道客户端是否认可他的起始序号，只有当服务端收到第三次握手的报文，它才能知道服务端认可了他的起始序号，两次握手没办法保证双方就起始序号协商一致。

为什么不是四次呢？因为在考虑包丢失的情况下，3次已经可以让双方协商一致了，而4次解决不了更多的问题，在考虑包丢失的情况下，3次握手可能遇到的问题，4次握手依然会遇到，并且会引入更多的问题。

所以是三次握手


#### 4. 说说四次挥手？  (3_2024_12_12)
![image](https://github.com/Luozujian/architect/assets/27532970/bb6f564b-323b-4cd2-b62f-11bef87fce8b)

四次挥手即可以由服务端发起，也可以由客户端发起。


第一次挥手: 客户端向服务端发送一个FIN = 1 ， 序列号为u 的 TCP报文，表示客户端已经数据要发送给服务器了， 客户端进入FIN-WAIT-1状态


第二次挥手: 服务器需要确认客户端的报文，所以它会向客户端发送，ACK = 1， ack = u + 1， seq = v的tcp报文，服务器进入到CLOSE-WAIT状态，客户端收到
请求后，进入到FIN-WAIT-2状态

第三次挥手: 服务器会发送一个序列号为w，FIN = 1，ACK = 1， ack = y + 1，序列号为 = w的TCP给客户端，表示服务器也没有数据要发给客户端了，服务器进入到LAST-ACK状态

第四次挥手: 客户端向服务器发送一个ACK = 1，ack = w + 1，的响应报文给服务器，然后客户端进入TIME_WAIT阶段，服务器进入到CLOSE阶段


#### 5. 为什么要有CLOSED-WAIT 状态？ (3_2024_12_12)
服务器收到客户端的发送的FIN报文后，服务器端就会进入到CLOSE-WAIT状态，用来给服务端将还没有发送完的数据发送给客户端


#### 6. 为什么要有2MSL的TIME-WAIT时间? (3_2024_12_12)
如果客户端发送的响应请求丢失，服务端没有收到客户端的确认报文，就会再次发送FIN报文释放连接，2MSL最小能够保证客户端发送的ACK报文超时，以及服务器的FIN报文重传

如果不这么做会这么样？ 可能会导致复用的连接被关闭

step1: Server 发送给 Client最后一个FIN报文，Client也接受到了，然后发送ACK报文给Server，但是Server没有接收到这个报文，然后Server重传了FIN报文，如果没有2MSL的等待时间，CLient有与Server建立起了新的连接，然后CLient收到的Server的FIN报文，导致新的连接就被关闭了



#### 7. 为什么TCP关闭连接需要4次挥手？ (3_2024_12_12)
因为TCP是全双工的，即需要关闭Client到Server的通道，也需要关闭Server到Client的通道，并且还不会同时关闭，第二次挥手的FIN报文无法和ACK报文一起发送，因为服务端可能还有数据要发送给客户端。


#### 8. 游览器最多允许多少并行请求？(2_2024_12_12)
6个，即最多允许6个TCP连接


参考资料:
1. [ ] [TCP 三次握手与四次挥手面试题](https://www.xiaolincoding.com/network/3_tcp/tcp_interview.html#tcp-%E4%B8%89%E6%AC%A1%E6%8F%A1%E6%89%8B%E8%BF%87%E7%A8%8B%E6%98%AF%E6%80%8E%E6%A0%B7%E7%9A%84)
