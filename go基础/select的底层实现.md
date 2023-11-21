### select底层实现

#### 1. 什么是select? (4_2023_11_21)
golang 针对channel的IO多路复用

#### 2. 多个channel就绪时会怎么样？ (4_2023_11_21)
随机选择一个channel执行

#### 3. select的分为几种类型？  (4_2023_11_21)
select 分为两种类型，阻塞和非阻塞，带default即阻塞类型，不带default则非阻塞类型
