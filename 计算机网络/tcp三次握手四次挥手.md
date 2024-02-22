# tcp三次握手四次挥手

#### 1. TCP三次握手过程？ (2_2024_02_22)

第一次握手，客户端随机生成一个序号seq = 100，然后发送SYN标识位为1，seq = 100的报文给服务器

第二次握手，服务器接收到请求后，也会随机生成一个序号假设seq = 200，它需要对接收到的报文做确认，所以它会发送一个SYN标记位为1，
ACK标记位为1，序号为200，确认号为100 + 1的报文给客户端，然后服务器进入SYN_RECV状态。

第三次握手，客户端接受报文后，它需要对服务器的报文进行确认，所以它需要发送一个ACK标记位为1，确认号为200 + 1这样一个报文给服务器。然后服务器和
客户端都进入established状态


![image](https://github.com/Luozujian/architect/assets/27532970/a502b127-0f72-4a73-a542-a07e3e76ed3f)

#### 2. 



参考资料:
1. [ ] [TCP 三次握手与四次挥手面试题](https://www.xiaolincoding.com/network/3_tcp/tcp_interview.html#tcp-%E4%B8%89%E6%AC%A1%E6%8F%A1%E6%89%8B%E8%BF%87%E7%A8%8B%E6%98%AF%E6%80%8E%E6%A0%B7%E7%9A%84)
