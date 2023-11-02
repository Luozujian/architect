### redis设计与实现

#### 1. 什么是redis？ (2_2023_11_1)
redis是用C语言开发，支持网络交互，基于内存也可持久化的key/value数据库，它具备如下特点:
1. 读写速度快 (10w ops，纯内存操作)
2. 支持持久化、支持多种数据结构、支持主从复制，支持高可用，支持分布式

#### 2. 为什么要使用redis缓存？(2_2023_11_1)
主要用于以下场景:
1. 优化性能，提升API处理能力
2. 分布式数据共享
3. 分布式锁

#### 3. redis和map有啥区别？(2_2023_11_1)
redis是分布式缓存，具有内存淘汰策略，支持高可用，主从复制，集群部署，持久化
map是本地缓存，不具备内存淘汰策略，也不支持持久化，重启后数据就没了


#### 4. 为什么redis快？(2_2023_11_1)
1. 在不考虑RDB、AOF情况下，他是纯内存操作，当然快，而RDB和AOF是异步的，undo和redo log对磁盘的操作是append操作，影响较小
2. 从网络层来看，使用多路I/O复用模型
3. 单线程模型处理用户请求，避免加锁解锁带来的额外开销
4. 高效的数据结构、合理的数据编码

// 怎么个高效法要补充
// IO模型需要再看
#### 5.  redis有哪些类型的数据结构？(2_2023_11_1)
常见的5大数据结构，分别是string、list、set、hash以及zset
特殊使用场景的有3种数据结构，分别是geo(地理位置)，hyperlonglong 基数计数，在一定误差内，计算不同key的个数，无法获取具体的key是啥，bitmap位图

#### 6. string类型支持哪些操作？(2_2023_11_1)
支持9种操作，全是O(1)的时间复杂度，分别是:
|  操作   | 命令   | 例子        | 说明          |
|  ----  | ----  | ----        | ----         |
| set    | A/U | set key val   | 设置或修改值    |
| get    | G   | get key       | 获取值         |
| del    | D   | del key       | 删除key        |
| getset |     | getset key val| 先获取再修改值   |
| incr   |     | incr key      | 自增           |
| decr   |     | decr key      | 自减           |
| incrby |     | incrby key val| 增加val        |
| decrby |     | decrby key val| 减少val        |
| append |     | append key val| 拼接val，返回长度|


#### 7. string类型常用于什么场景？(2_2023_11_1)
1. 计数器、限流
2. 验证码
3. 分布式锁等等

#### 8. list类型支持哪些操作？ (2_2023_11_1)
支持14种操作

|  操作     | 命令   | 例子                         | 说明           | 时间复杂度 |
|  ----    | ----   | ----                        | ----          | ----     |
| lpush     | A     | lush  key value [value …]  | 从左添加数据     | O(1)    |
| lpushx    |       | lpushx key value            | 从右添加数据, 只有key存在，才能添加进去    | O(1)    |
| lrange    |       | lrange key start stop       | 从左往右便利，取start到stop元素       | O(n)    |
| lpop      |       | lpop key                    | 左边弹出一个元素 | O(1)    |
| llen      |       | llen key                    | 获取元素个数     | O(1)    |
| llen      |       | llen key                    | 获取元素个数     | O(1)    |
| lrem      |       | lrem key count val          | 删除count个，val等于val的节点   | O(n)    |
| lset      |       | lset key index val          | 将第index位的val设置成val      | O(n)    |
| lindex    |       | lindex key index            | 从左往右获取第index位的元素      | O(n)    |
| ltrim     |        | ltrim key start stop        | 保留start到stop的元素          | O(n)    |
| linsert   |      | linsert key before/after pivot val        | 在pivot元素之前或者之后删除元素          | O(n)    |
| rpush     | A     | lush  key value [value …]  | 从右添加数据     | O(1)    |
| rpop      |       | rpop key                    | 右边弹出一个元素 | O(1)    |
| rpoplpsuh      |       | rpoplpush source destination  | 从source链表尾部拿出一个元素，放入到dest头部 | O(1)    |


#### 9. list常见的使用场景？ (2_2023_11_2)
1. 各种链表 -> 粉丝列表、评论列表、文章列表等等

#### 10. set常见的操作？  (2_2023_11_2)
常见的有12种操作

|  操作     | 命令   | 例子                         | 说明                             | 时间复杂度 |
|  ----    | ----   | ----                        | ----                            | ----    |
| sadd     |        |  sadd key val [val...]      | set 添加数据，返回添加成功的数量     | O(1)    |
| smembers |        |  smembers key               | 返回set所有元素                   | O(n)    |
| scard    |        |  scard key                  | 返回set元素个数                   | O(1)    |
| sismember|        |  sismember key member       | member 是否在set中                | O(1)    |
| spop     |        |  spop key                   | 随机删除一个元素                   | O(1)    |
| srandmember   |        |  srandmember key           | 随机返回一个key，不删除          | O(1)    |
| smove   |           |  smove source destination member    | 把source中member这个元素移动到dest  | O(1)    |
| sdiff   |           |  sdiff key [key...]    | 第一个集合独有的元素  | O(n)    |
| sinsert |           |  sinsert [key...]    | 返回key的交集  | O(n*m)    |
| sinsertstore |       |  sinsertsort destination [key...]    | 将keys的交集，写入到dest  | O(n*m)    |
| sunion |       |  sunion [key...]    | 所有元素并集  | O(n)    |
| sunionstore |       |  sunionstore [key...] destination   | 所有元素并集并存储  | O(n)    |


#### 11. set常见的场景？ (2_2023_11_2)
1. 求集合交集、差集，并集，两个人共同的好友，两个人不同的好友


#### 12. hash常见的操作？ (2_2023_11_2)
|  操作     | 命令   | 例子                         | 说明                             | 时间复杂度 |
|  ----    | ----   | ----                        | ----                            | ----    |
| hset     |        |  sadd key val [val...]      | set 添加数据，返回添加成功的数量     | O(1)    |


#### 参考资料:
1. [ ] [Redis---List数据类型操作](https://developer.aliyun.com/article/71365)




