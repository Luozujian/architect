# 一、背景
日常需求承接过程中，经常需要为数据分配一个唯一编号，数据量较小的时候，无需分库分表，可以使用自增id做为数据的编号，当数据量变大之后，需要进行分库分表，这个时候就无法使用数据库自建主键做为数据编号了，为解决上述问题，需要实现一个发号服务，用于为数据分配唯一编号。

其应该具备如下基本能力:
1. 资源维度唯一编号
2. 高性能 & 高可用 & 可扩展

能满足如下需求更好:
1. 有序递增，提升数据库插入效率
2. 可做为底层服务独立部署


# 二、算法调研
为更好满足业务需求，调研业内优秀设计，选择适用业务实际情况方案


## 2.1、数据库主键自增
利用单表自增有序、唯一来分配编号

![数据库 drawio](https://github.com/user-attachments/assets/9213eb92-7ea9-47b3-b853-07c7c2280e9a)

建表语句如下，其中stub无实际意义，为其建立唯一索引，方便修改数据

```
CREATE TABLE `sequence_id` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `stub` char(10) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`),
  UNIQUE KEY `stub` (`stub`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
```

搭配replace into语句来插入数据，可减少数据冗余，replace into 首先会尝试插入数据，如果出现唯一键重复报错，则删除原来数据，插入新数据
```
BEGIN;
REPLACE INTO sequence_id (stub) VALUES ('stub');
SELECT LAST_INSERT_ID();
COMMIT;
```

## 2.2 数据库号段模式
访问数据库一次分配一段编号，编号分配完再次访问数据库获取下一段编号

![image](https://github.com/user-attachments/assets/f04095b0-fd0b-45b1-8ad1-ba197bbc558a)


建表语句如下:
```
CREATE TABLE `sequence_id_generator` (
  `id` int(10) NOT NULL,
  `current_max_id` bigint(20) NOT NULL COMMENT '当前最大id',
  `step` int(10) NOT NULL COMMENT '号段的长度',
  `version` int(20) NOT NULL COMMENT '版本号',
  `biz_type`    int(20) NOT NULL COMMENT '业务类型',
   PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
```

当号段使用完时向数据库申请新的编号可使用如下方法，优先使用乐观锁，再使用悲观锁

```
select * from table where biz_type = *
得到current_max_id = x， version = v1
update table set current_max_id = x + step and version = v2 where version = v1 and bi_type = *
```



## 2.3 NoSql
常使用Redis incr命令实现，和数据库类似
![image](https://github.com/user-attachments/assets/80bf333c-11d6-447f-a882-39388a81dfed)

```
set sequence_id_biz_type 1
incr sequence_id_biz_type
get sequence_id_biz_type
```

## 2.4 UUID
UUID由16B，128位组成，UUIDV1由MAC地址 + 时间戳组成，除非时钟回拨，否则几乎不会重复，V4由随机数组成，128位随机数数学统计上重复的概率非常低



## 2.5 雪花算法
Twitter开源的一种分布式Id生成算法，64位
![image](https://github.com/user-attachments/assets/078f28ed-a3fd-4d4c-8453-b2a04f6fc7af)

* **timestap(42bit)**: 豪秒级时间戳，可支持126年
* **datacenter id + worker id(10位)**: 5位表示机房编号，5位表示机器编号
* **sequence(12bit)**: 序列号，每ms最多支持4096个编号生成 

在实际落地中，回根据自己的业务场景进行改造，上诉这些字段的位数也并非固定

## 2.6 各算法优缺点对比

| 算法 | 优点 | 缺点 |
| --- | --- | --- |
| 数据库自增主键 | 1.方案落地成本较低 <br> 2.编号单调递增 <br> 3.容易出现敏感数据泄漏 | 1.支持QPS较低，每次生成编号都需要访问数据库 <br> 2.对数据库压力较大 |
| 号段模式 |1.支持QPS较高，稳定性较强 <br> 2.对数据库访问压力较小 <br> 3.不容易出现敏感数据泄漏 | 1.方案落地成本较高 <br> 2.编号非严格递增 |
| NoSql | 1.支持QPS适中 <br> 2.方案落地成本较低  | 1.稳定性较差 <br> 2.数据丢失，容易生成重复编号 <br> 3.无法水平扩容 |
| UUID | 1.支持QPS较高 <br> 2.方案落地成本较低 <br> | 1.128bit存储成本较高 <br> 2.完全随机，库表插入成本较高 |
| 雪花算法 | 1.支持QPS较高 <br> 2.方案落地成本较低 | 1.时钟回拨问题 <br> 2.编号非严格单调递增 |

具体使用那种算法，需要根据业务特性而定

# 三、开源框架调研
## 3.1、UidGenerator(百度)
基于雪花算法，未解决时钟回拨问题，18年后不在维护

## 3.2、Leaf(美团)
天底下没有两片一样的树叶，非常nice，可基于号段&也可基于雪花算法

## 3.3 Tinyid (滴滴)
基于号段模式



# 问题 & 挑战







![image](https://github.com/user-attachments/assets/1f5a0463-b089-4f9d-a66c-93213ef39067)






参考资料: 
https://cloud.tencent.com/developer/article/1839391 为什么需要分布式ID
