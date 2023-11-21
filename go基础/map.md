#### 1. map专题

##### 1.1 map是什么？(5_2023_11_21)
map是用来高效存储key/val类型数据的数据结构，它可以在o(1)的时间复杂度对key，进行增，删，改，查，它的特点是：**高效、无序、并发不安全**


##### 1.2 map底层实现主要数据结构？(5_2023_11_21)
map的数据结构是Hash表，并且使用拉链法来解决hash冲突


##### 1.3 map有几种扩容形式？ (5_2023_11_21)
两种扩容方式，分别是等量扩容和翻倍扩容


##### 1.4 map两种扩容形式的条件分别是什么？ (5_2023_11_21)
等量扩容: 当B < 15的时候，溢出桶的数量大于2^B的时候，就会发生等量扩容，B >= 15的时候，溢出桶的数量 > 2^15，就会发生等量扩容

翻倍扩容: 当装载因子 > 6.5的时候，即发生翻倍扩容，装载因子等于元素个数/桶的数量


##### 1.5 map的扩容方式是什么？  (5_2023_11_21)
渐进式扩容

##### 1.6 渐进式扩容的优缺点是什么？  (5_2023_11_21)
优点:
1. 将扩容的开销，均摊到后续的写操作，用户体验更好

缺点:
1. 扩容期间，所使用的内存会比较大

##### 1.7 扩容的大概逻辑？ (5_2023_11_21)
1. 新增操作，判断是否需要扩容
2. 如果需要扩容，将bucket数据，挂到oldbuket上
3. 修改扩容相关的数据
4. 后续每次操作都将以扩容进度往前推(0,2)个下标

##### 1.8 如何判断是否在扩容？(5_2023_11_21)
oldBucket字段不为空，则表示正在扩容

##### 1.9 如何看某一个链表是否已经扩容完成？(4_2023_11_21)
如果链表第一个桶的tophash[0]位于位于[2,5]之间，则代表已经扩容完成

// todo 为什么选择2^15次方


##### 1.10 map的主要结构体？ (5_2023_11_21)	
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
10. mapextra.oldoverflow 保存旧溢出桶链表
11. mapextra.nextOverflow 下一个空闲溢出桶地址,懒申请机制


bmap:
1. tophash: 存储桶8个key的高8位的hash，用来快速判断key是否在这个桶里面，tophash[0] < minTopHash
2. key
3. val
4. overflow

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
##### 1.11 make(map) 底层有几种策略？ (5_2023_11_21)
两则策略，分别对应两个不同的函数，makemap_small()和makemap()

##### 1.12 make(map) 不同策略发生的条件分别是什么？ (5_2023_11_21)
makemap_small()发生的条件是，使用make()方法的时候没有指定大小或者指定的大小小于8
makemap()发生的条件是，使用make()方法，指定的大小大于8

##### 1.13 makemap_small()和makemap()函数分别做了什么？  (5_2023_11_21)
makemap_small(): 只做了为hmap分配内存以及指定hash种子的动作,在堆上分配内存
makemap(): 分配内存，指定hash种子，初始化桶，以及分配预留的溢出桶，内存对齐

##### 1.14 map 增加/修改操作？(5_2023_10_21)
1. 如果map为nil会空指针异常，如果写标记位为1，则会panic，否则设置写标记位
2. 根据hash值计算出在那个桶以及高8位，根据低B位，找到key所属的桶
3. 遍历桶及其链表，如果找到key，则更新value，否则创建一个新的溢出桶
4. 结束增加/修改操作

##### 1.14 map删除操作？(5_2023_10_21)
1. 定位元素位置
2. 删除元素，维护emptyRest标签


##### 1.15 map get方法的大概逻辑？(5_2023_10_21)
1.计算key的hash值除桶的个数取模 => 可得到key位于哪个桶所在链表里面
2.如果正在发生扩容
2.然后取hash值的前8位，遍历该key所在的链表，看看是否存在该key，存在即返回


##### 1.16 扩容是优先等量扩容还是翻倍扩容？ (4_2023_10_21)
优先翻倍扩容



//todo 可捕获异常，不可捕获异常
//todo 真实IP的获取
//interface类型的实现


#### 参考资料:

1. [ ] [1.map底层原理](https://zhuanlan.zhihu.com/p/495998623)
2. [ ] [2.map底层原理 mapextra](https://cloud.tencent.com/developer/article/1746966)
3. [ ] [3.为什么map不支持并发安全](https://learnku.com/articles/67151)
4. [ ] [4.map的实现原理](https://zhuanlan.zhihu.com/p/495998623)
