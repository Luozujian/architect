# rabbitmq

#### 1. rabbit是什么？ (2_2024_02_16)
rabbit是一个实现了高级消息队列AMQP协议的中间件

#### 2. rabbitmq的优点？  (2_2024_02_16)
1. 易拓展、支持高可用
2. 多语言客户端、UI图形界面

#### 3. rabbitmq 5种工作模式？ (2_2024_02_16)
1. simple
![image](https://github.com/Luozujian/architect/assets/27532970/f5c90eb5-44f0-4381-93bf-01514b1c45c0)


2. work
![image](https://github.com/Luozujian/architect/assets/27532970/ded32ca0-0282-4a7f-bdb8-c840320bafbc)

   
3. 发布/订阅
![image](https://github.com/Luozujian/architect/assets/27532970/c5f919de-e0b7-402e-9e24-3ca994143db4)

  
4. routing
![image](https://github.com/Luozujian/architect/assets/27532970/2b03d00c-2e28-43bf-904a-92bae2382b5e)


5. topic模式
topic在routing的基础上支持通配符

#### 4. 什么是死信队列？  (2_2024_02_16)
1. 被拒绝的消息
2. 过期的消息
3. 消息队列满了、被抛弃的消息

应用场景: 消息无法被正确消费，但是也不致于被丢弃，可用死信队列


#### 5. 延迟队列的实现方式？  (2_2024_02_16)
给队列设置过期时间，并且没有消费者，使其加入到死信队列，然后再消费死信队列，以此来实现延迟队列

#### 6. 过期时间设置方式？ (2_2024_02_16)
1. key设置过期时间： 必须要消费到的时候，才能感知到。 (插件能解决这个问题)
2. 队列设置过期时间: 队首元素肯定先过期

