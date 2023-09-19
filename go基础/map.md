#### 1. map专题

##### 1.1 map是什么？(2_2023_09_18)
map是用来高效存储key/val类型数据的数据结构，它可以在o(1)的时间复杂度对key，进行增，删，改，查，它的特点是：**高效、无序、并发不安全**


##### 1.2 map底层实现主要数据结构？(2_2023_09_14)
map的数据结构是Hash表，并且使用拉链法来解决hash冲突


##### 1.3 map 扩容机制？(2_2023_09_18)
1. 等量扩容: 频繁插入和删除操作，导致溢出桶太多，但是每个桶中存储的元素并不多，需要对元素进行重拍，优化整体性能，扩容条件，当B < 15时，如果overflow的数量超过了2^B，B > 15时，overflow的数量超过了2^15次方

2. 翻倍扩容: 装载因子大于6.5，装载因子 = 元素个数/桶的数量

##### 1.4 map的主要结构体？ (2_2023_09_18)
hmap的map实现的基础结构，其主要包含如下字段:
1. count,元素个数，len()返回的数据这个值，而且必须放第一个位置
2. flags,状态标记位。如是否被多线程读写、迭代器在使用新桶、迭代器在使用旧桶等
3. B, 桶指数，桶的数量为2^B，不包含溢出桶
4. noverflow，溢出桶的数量近似值
5. hash0，hash种子
6. buckets，指向2^B个桶所组成数组的指针
7. oldbuckets，只有当扩容时才不为空，指向扩容前的桶
8. nevacuate，进度计数器，表示扩容后搬迁的进度（小于该数值的桶已迁移）
9. mapextra.overflow 保存溢出桶链表
10. extra.oldoverflow 保存旧溢出桶链表
11. extra.nextOverflow 下一个空闲溢出桶地址,懒申请机制


bmap:
1. tophash: 存储桶8个key的高8位的hash，用来快速判断key是否在这个桶里面，tophash[0] < minTopHash

实际创建bmap结构的时候，会动态生成一个存储8的元素的key,以及val还有一个指向溢出桶的指针


```c++
// A header for a Go map.
type hmap struct {
	// Note: the format of the hmap is also encoded in cmd/compile/internal/gc/reflect.go.
	// Make sure this stays in sync with the compiler's definition.
	count     int // # live cells == size of map.  Must be first (used by len() builtin)
	flags     uint8
	B         uint8  // log_2 of # of buckets (can hold up to loadFactor * 2^B items)
	noverflow uint16 // approximate number of overflow buckets; see incrnoverflow for details
	hash0     uint32 // hash seed

	buckets    unsafe.Pointer // array of 2^B Buckets. may be nil if count==0.
	oldbuckets unsafe.Pointer // previous bucket array of half the size, non-nil only when growing
	nevacuate  uintptr        // progress counter for evacuation (buckets less than this have been evacuated)

	extra *mapextra // optional fields
}

type mapextra struct {
	// If both key and elem do not contain pointers and are inline, then we mark bucket
	// type as containing no pointers. This avoids scanning such maps.
	// However, bmap.overflow is a pointer. In order to keep overflow buckets
	// alive, we store pointers to all overflow buckets in hmap.extra.overflow and hmap.extra.oldoverflow.
	// overflow and oldoverflow are only used if key and elem do not contain pointers.
	// overflow contains overflow buckets for hmap.buckets.
	// oldoverflow contains overflow buckets for hmap.oldbuckets.
	// The indirection allows to store a pointer to the slice in hiter.
	overflow    *[]*bmap
	oldoverflow *[]*bmap

	// nextOverflow holds a pointer to a free overflow bucket.
	nextOverflow *bmap
}

// A bucket for a Go map.
type bmap struct {
	// tophash generally contains the top byte of the hash value
	// for each key in this bucket. If tophash[0] < minTopHash,
	// tophash[0] is a bucket evacuation state instead.
	tophash [bucketCnt]uint8
	// Followed by bucketCnt keys and then bucketCnt elems.
	// NOTE: packing all the keys together and then all the elems together makes the
	// code a bit more complicated than alternating key/elem/key/elem/... but it allows
	// us to eliminate padding which would be needed for, e.g., map[int64]int8.
	// Followed by an overflow pointer.
}

```


![image](https://github.com/Luozujian/architect/assets/27532970/1a2097c3-2a10-4d6a-84a9-66c2953e4277)



##### 1.5 make(map) 所做的操作？ (2_2023_09_14)
1. 如果直接调用make(map)获走map(map, hint)，且hint <= 8，则调用函数 makemap_small()
2. 如果hint > 8 则调用，makemap函数

makemap_small() 只做了为hmap分配内存以及指定hash种子的动作
makemap(): 分配内存，指定hash种子，初始化桶，以及分配预留的溢出桶，内存对齐


##### 1.6 map 增加/修改操作？(2_2023_09_18)
1. 如果map为nil会空指针异常，如果写标记位为1，则会panic，否则设置写标记位
2. 根据hash值计算出在那个桶以及高8位
3. 如果找到key，则更新value，否则创建一个新的溢出桶
4. 结束增加/修改操作

##### 1.7 map删除操作？(2_2023_09_18)
1. 定位元素位置
2. 删除元素，维护emptyRest标签


##### 1.8 扩容逻辑？ (2_2023_09_18)
预扩容:
1. 判断是等量扩容还是翻倍扩容
2. 初始化相关值
3. 每次把

##### 1.5 map get方法的大概逻辑？(2_2023_09_14)
1. 如果


1.计算key的hash值除桶的个数取模 => 可得到key位于哪个桶所在链表里面
2.如果正在发生扩容
2.然后取hash值的前8位，遍历该key所在的链表，看看是否存在该key，存在即返回


#### 1.6 map set方法的大概逻辑？(2_2023_09_18)
1.计算key的hash值除桶的个数取模 => 可得到key位于哪个桶所在链表里面
2.遍历当前桶，防止key重复，如果重复则更新，否则则插入




//todo 可捕获异常，不可捕获异常
//todo unsafe point
//todo 真实IP的获取
//interface类型的实现


参考资料:

1. [ ] [map底层原理](https://zhuanlan.zhihu.com/p/495998623)
2. [ ] [map底层原理 mapextra](https://cloud.tencent.com/developer/article/1746966)


#### 参考资料:
1. [ ] [1.为什么map不支持并发安全]([https://learnku.com/articles/67151](https://juejin.cn/s/golang%20map%E4%B8%BA%E4%BB%80%E4%B9%88%E5%B9%B6%E5%8F%91%E4%B8%8D%E5%AE%89%E5%85%A8)https://juejin.cn/s/golang%20map%E4%B8%BA%E4%BB%80%E4%B9%88%E5%B9%B6%E5%8F%91%E4%B8%8D%E5%AE%89%E5%85%A8)
2. [ ] [2.map的实现原理](https://zhuanlan.zhihu.com/p/495998623)
