# mysql基础知识

#### 1. InnoDb存储引擎有那几个隔离级别？ (2_2024_02_25)
1. 读未提交，存在胀读、不可重复读、幻读
2. 读已提交，不可重复读、幻读
3. 可重复读，可重复读 + 间隙锁 解决 大部分幻读问题，默认隔离级别
4. 串行化，无问题

#### 2. 什么是脏读、不可重复读、幻读？ (2_2024_02_25)
脏读: 能读到其他事物还未提交的数据 (其他事物可能会回滚)

不可重复读: 一个事物内，同一条数据在不通查询中返回的结果不一样

幻读: 一个事物内不同的查询时间点，对同一条件的查询，返回的结果集不同


#### 3. 事物的四大特性？(2_2024_09_20)
1. 原子性，要么全都成功，要么全都是失败
2. 持久性，执行成功，数据就不应该丢失
3. 一致性，从一个一致的状态到另一个一致的状态，不应该存在中间态
4. 隔离性，事物隔离，不应该相互打扰

参考资料: https://juejin.cn/post/6844903873325301774  解释的非常到位


#### 3. 什么是两类丢失?  (2_2024_02_25)
第一类丢失: 回滚操作覆盖其他事物提交的数据 (实现避免)
第二类丢失: 提交操作覆盖其他事物提交的数据 (RR隔离级别会加锁)


解决方案:
1. 悲观锁: 加互斥锁 select for update (并发量较大)
2. 乐关锁: 加版本号 （并发量较小）

#### 4. 什么是当前读和快照读？  (2_2024_02_25)

当前读: 读取记录的最新版本，select in share mod，select for udpate，select for delete， update、insert、delete 这些操作都会触发一次当前读
快照读: 就是根据事物的Read View去读取当前事物id可见的记录，普通的select语句触发快照读


#### 5. 记录有那三个隐藏字段？  (2_2024_02_25)
1. DB_ROW_ID， 6B 隐藏的自增ID，如果表没有主键以及唯一键来生成聚簇索引，就会使用DB_ROW_ID来生成聚簇索引
2. DB_TRX_ID，6B 事物ID，表示最后一次修改/删除这条记录的事物ID
3. BD_ROLL_PTR，7B，指向该记录的上一个版本(存储在undo日志里面)的指针
  

#### 6. 简介一下undo.log日志？  (2_2024_02_25)
undo.log: 日志分为两种分别是insert undo log和update undo log
insert undo log: 代表事物在insert新记录时产生的undo log，只有在会滚的时候需要，事物提交即可丢弃
update undo log: 事物在修改或者删除时产生，不仅在事物会滚时需要，在快照读时也需要，所以不能随机删除，只有在快照读和事物都不涉及该日志时，对应的日志才会被删除
当我们修改一条记录的时候，会对该记录施加一个x锁，然后把记录拷贝到update undo log中，然后修改该行的DB_TRX_ID为当前事物ID，DB_ROLL_PTR指向上一个版本，提交后释放锁
这样该记录会形成一个版本链条，头部是最新的版本，尾部是最老的版本


#### 7. mysql 是如何实现事物的隔离级别的？  (2_2024_02_25)
mysql 通过mvcc来实现事物隔离级别，mvcc是一种多版本控制方法，数据库的并发场景有一下三种:
1. 读读，无并发问题
2. 读写，脏读，不可重复读和幻读问题
3. 写写，第一类丢失和第二类丢失问题

MVCC主要是用来解决读写问题的，写写问题可以通过乐关锁或者悲观锁来实现。MVCC的实现主要是依赖 记录的三个隐藏字段、undo日志以及Read View 读视图


#### 8. 什么是读视图？  (2_2024_02_25)

Read View是在事物进行快照读的时候产生的，在事物进行快照读的那一刻，会产生一个数据库的快照，记录并维护当前数据库活跃的事物ID(事物ID严格递增)

#### 9. Read View的可见性算法是怎么样的？  (2_2024_02_25)
Read View是用来做可见性判断的，其遵循一个可见性算法，我可以简单的理解其内部维护了三个属性

1. trx_list，一个数值列表，当前活跃的事物ID
2. min_trx_id，trx_list 最小的一个事物ID
3. max_trx_id，Read View生成时刻，系统已分配的最大事物id
4. creator_trx_id, 生成该快照的事物id

首先比较记录的DB_TRX_ID是否小于min_trx_id，如果小于的话，则表示该条记录在创建Read View的时候就已经提交了，所以是可见的

然后比较记录的DB_TRX_ID是否大于max_trx_id，如果大于的话则表示记录被创建Read View之后的新的事务所修改，所以对当前事务不可见

如果记录的DB_TRX_Id在trx_list当中，并且DB_TRX_ID不等于当前的事务id，则表示其他事务修改了当前记录，并且还尚未提交，所以对当前事务是不可见的，如果DB_TRX_ID等于当前事务的id的话，那么对当前事务是可见的。

如果记录的DB_TRX_ID大于min_trx_id小于max_trx_id的话并且不在trx_list当中的话，那么表示修改该记录的事务在Read View之前就已经提交了，所以对当前事务是可见的。


#### 10. MVCC在隔离级别是可重复读时，是如何解决脏读和不可重复读的？  (2_2024_02_25)
在可重复读这个隔离级别下，根据Read View的可见性算法，当事物T1根据条件1执行完select语句之后，其他事物修改符合条件t1的数据，无论提交或者不提交，都对事物t1不可见，
所以解决了脏读和不可重复读的问题。


#### 11. 读已提交这个隔离级别是如何实现的呢？  (2_2024_02_25)
每次select的时候重新生成一个读视图，这样已提交的数据就是对当前事物可见的了。


#### 12. 有几中锁模式？  (2_2024_02_25)
1. 行锁: 共享锁 (s锁)、排它锁(x锁)
2. 表锁: 意向锁共享锁 (IS)，意向排他锁(IX)，表级S锁，表级X锁
3. 间隙锁、临键锁(记录锁 + 间隙锁)，间隙锁与间隙锁不胡扯，间隙锁不允许插入

意向锁是表锁，而共享锁和排他锁即有行锁也有表锁，意向锁不与行锁冲突，意向锁和意向锁也不冲突

1. 要想施加行级别的x锁，必须先施加ix
2. 要想施加行级别的s锁，必须先施加is

意向锁的作用:

当你想对整个表施加X锁的时候，如果没有意向锁，就需要遍历每一行记录，看看是否加锁，而有了意向锁，就可以很快的判断该表是否有记录加锁了。


![image](https://github.com/Luozujian/architect/assets/27532970/5af74c9d-d70f-40eb-b9ee-a695623ae98a)


##### 13. 有哪几种锁算法？ (2_2024_02_25)
1. 记录锁，精准匹配某条记录
![image](https://github.com/Luozujian/architect/assets/27532970/9d13b6f9-c4d3-4a03-ab46-226db913ab3a)

2. Gap Locks 间隙锁

间隙锁用来锁住两个记录之间的间隙，需要注意三点
1. 间隙锁只存在于RR的隔离级别
2. 间隙锁和间隙锁之间是不冲突的，间隙锁只阻塞插入
3. 当前读才会触发间隙锁，触发间隙锁必须要有索引，如果没有索引会施加一个表级的锁。(可以去试试)


间隙锁锁定的区间:  
如果是使用主键索引来做等值查询，精准匹配，那么会施加一个记录锁，如果使用唯一索引来做范围查询则会施加一个间隙锁，并且对在所查询范围里面的记录施加一个共享锁或者排它锁

如果是使用普通索引则根据检索条件向左边寻找最靠近检索条件的记录值A，条件向右边寻找最靠近检索的记录值B，作为锁定得区间(A,B)。在区间(A,B)里面的记录会相应的施加共享锁或者排它锁

![image](https://github.com/Luozujian/architect/assets/27532970/b34b9794-fe6b-4267-ae7e-0445d39f639b)


4. Next-Key Locks 临键锁


![image](https://github.com/Luozujian/architect/assets/27532970/2c9e68c7-a6a3-4e67-a032-c73a83e48b5e)



//TODO 什么时候会锁表
#### 14. 什么时候施加间隙锁、什么时候施加临键锁呢？ (2_2024_02_25)


唯一索引: 
1. 等值精准匹配，记录锁
2. 等值未匹配到，向左找到最靠近条件的记录，向右找到最靠近条件的记录，对这个区间施加间隙锁
3. 范围查询，向左找到最靠近条件的记录，向右找到最靠近条件的记录，对这个区间施加间隙锁，对区间包含的记录施加记录锁

非唯一索引:
1. 等值精准匹配，向左找到最靠近条件的记录，向右找到最靠近条件的记录，对这个区间施加间隙锁，对区间的记录施加记录锁
2. 等值非精准匹配，向左找到最靠近条件的记录，向右找到最靠近条件的记录，对这个区间施加间隙锁
3. 范围查询，向左找到最靠近条件的记录，向右找到最靠近条件的记录，对这个区间施加间隙锁，左闭右开，对区间的记录施加记录锁



#### 15. 什么时候使用表锁？  (2_2024_02_25)
1. 显示指定，lock table
2. DDL操作，比如alter table操作
3. 走不了索引、全表扫描，可能会施加表锁



#### 16. RR + 间隙锁解决幻读问题了吗？ (2_2024_02_25)
一定程度上解决了幻读问题，并没有完全解决，select 操作，执行更新操作，更新了插入的数据，根据可见性算法，就会能看到了。


#### 17. 执行一条SQL语句大致流程？ (2_2024_09_22)
执行一条SQL语句，
  1. 首先会经过连接器，连接器负责连接管理 & 权限校验
  2. 然后查询缓存(8.0废弃)，看是否能命中缓存
  3. 然后再过分析器，进行词法分析 & 语法分析，看是否存在语法问题
  4. 然后到预处理，看列、表是否存在，* 展开成具体的列等等动作
  5. 然后到优化器，优化器决定具体的执行计划，主要就是决定走那个索引，然后生成一份执行计划
  6. 然后再到执行器，具体执行执行计划
![image](https://github.com/user-attachments/assets/cd24c618-53f6-47c2-927f-f7e54a784895)
#### 18.Mysql一行的存储格式？ (2_2024_09_22)
如果存在变长字段则会存在一个变长字段长度列表，如果存在可为空的字段，则也会存在一个空值列表，用二进制编码来确定某列是否为空
![image](https://github.com/user-attachments/assets/31527bc0-e1d0-4518-98a3-1b2325f6f10e)




参考资料:
1. [ ] [数据库第一类第二类丢失更新](https://blog.csdn.net/paopaopotter/article/details/79259686)
2. [ ] [讲的非常详细](https://www.cnblogs.com/caibaotimes/p/17958671)
3. [ ] [RR + 间隙锁 并没有解决幻读问题](https://juejin.cn/post/7134186501306318856)
