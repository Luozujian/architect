#### 1. map专题
##### 1.1 map是不是并发安全的？(2_2023_09_13)
map不是并发安全的，其内部没有锁机制来保障其并发安全性。Go官方认为map大多数使用场景应该是简单的，不存在并发问题，没必要为了少部分场景而牺牲map的性能，对于存在并发情况的
少部分场景可以选择使用sync.map或者主动加锁

##### 1.2 map


//todo 可捕获异常，不可捕获异常

#### 参考资料:
1. [ ] [1.为什么map不支持并发安全]([https://learnku.com/articles/67151](https://juejin.cn/s/golang%20map%E4%B8%BA%E4%BB%80%E4%B9%88%E5%B9%B6%E5%8F%91%E4%B8%8D%E5%AE%89%E5%85%A8)https://juejin.cn/s/golang%20map%E4%B8%BA%E4%BB%80%E4%B9%88%E5%B9%B6%E5%8F%91%E4%B8%8D%E5%AE%89%E5%85%A8)
2. 
