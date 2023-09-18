#### 1. map专题
##### 1.1 map是不是并发安全的？(3_2023_09_18)
map不是并发安全的，其内部没有锁机制来保障其并发安全性。Go官方认为map大多数使用场景应该是简单的，不存在并发问题，没必要为了少部分场景而牺牲map的性能，对于存在并发情况的
少部分场景可以选择使用sync.map或者主动加锁

##### 1.2 map实现底层数据结构？(2_2023_09_14)
map的数据结构是Hash表，并且使用拉链法来解决hash冲突

##### 1.3 map get方法的大概逻辑？(2_2023_09_14)
1.计算key的hash值除桶的个数取模 => 可得到key位于哪个桶所在链表里面
2.如果正在发生扩容
2.然后取hash值的前8位，遍历该key所在的链表，看看是否存在该key，存在即返回


#### 1.4 map





//todo 可捕获异常，不可捕获异常
//todo unsafe point


#### 参考资料:
1. [ ] [1.为什么map不支持并发安全]([https://learnku.com/articles/67151](https://juejin.cn/s/golang%20map%E4%B8%BA%E4%BB%80%E4%B9%88%E5%B9%B6%E5%8F%91%E4%B8%8D%E5%AE%89%E5%85%A8)https://juejin.cn/s/golang%20map%E4%B8%BA%E4%BB%80%E4%B9%88%E5%B9%B6%E5%8F%91%E4%B8%8D%E5%AE%89%E5%85%A8)
2. [ ] [2.map的实现原理](https://zhuanlan.zhihu.com/p/495998623)
