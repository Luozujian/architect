### 1 sync.map底层实现

#### 1.1 sync.map是用来干嘛？ (4_2023_11_21)
map是并发不安全的，那如果想要并发安全的使用map怎么办呢？加锁是一种解决方案，官方还提供了另外一种解决方案，就是sync.map


#### 1.2 sync.map 提供哪些方法？ (4_2023_11_21)
```
scene.load(key)
scene.store(key, val)
scene.delete(key)
scene.loadOrdStore(key, val)
scene.Range(func(k, v interface{}) bool {
  fmt.Println("iterate:", k, v)
  return true
})
```

#### 1.3 sync.map 底层结构体的主要字段？(4_2023_11_21)
```c++
type Map struct {
	mu Mutex   // 锁，保护read和dirty字段
	read atomic.Pointer[readOnly]  // 存仅读数据，原子操作，并发读安全，实际存储readOnly类型的数据
	dirty map[any]*entry // 存最新写入的数据
	misses int //  计数器，每次在read字段中没找所需数据时，+1
	// 当此值到达一定阈值时，将dirty字段赋值给read
}

```

#### 1.4 readOnly结构体的主要字段？ (4_2023_11_21)
```c++
type readOnly struct {
	m       map[interface{}]*entry // 其底层依然是个最简单的map
	amended bool                   // 标志位，标识m.dirty中存储的数据是否和m.read中的不一样，flase 相同，true不相同
}
```


#### 1.5 sync.map load的大概逻辑？(4_2023_11_21)
```c++
1.从read读数据，如果没有并且amended为true，则加锁从dirty读数据
2.然后再检验一次read中是否有数据，如果还没有，则从dirty中取数据，然后misses++，如果miss次数达到阈值之后，则将dirty的值赋值给read，阈值就是dirty的长度
3.然后如果取到则返回，没有取到则返回空
```


#### 1.6 sync.map store的大概逻辑？(4_2023_11_21)
1. 看key是否在read中存在，如果在read中存在，则通过cas修改read
2. 如果不存在，加锁 => 再次检验是否在read中存在，如果不存在则看在dirty中是否存在
3. 如果在dirty中存在，则直接修改dirty的值
4. 如果在dirty中也不存在，则走新增逻辑
5. 如果read和dirty相同，则把read的值复制给dirty,会过滤掉被删除的数据，因为相同的情况只有在初始化或者dirty赋值给read的时候
6. 然后添加数据数据


#### 1.7 sync.map del的大概逻辑？(4_2023_11_21)
1. 如果在read中，则将read置为nil
2. 如果在dirty中，则直接干掉


#### 1.8 sync.map 遍历的大概逻辑? (4_2023_11_21)
1. 先看是否存在不一样的数据，如果存在，加锁
2. 再检验一次，看是否存在不一样的数据，如果存在，将dirty的数据赋值给read
3. 然后释放锁，然后再遍历read


#### 1.9 sync.map的优缺点? (4_2023_11_21)
空间换时间

优点:
1. 读读无需加锁，提升并发性能

缺点:
1. 在写多读少的场景，可能存在长时间的自旋，浪费CPU资源
2. 典型的空间换时间操作，写多读少的场景，消耗过大的空间，并不会提升较多的性能
3. 在修改dirty的时候依然需要加锁


所以他只适用于读多写少的场景，写多读少的场景，性能可能不如加读写锁。


#### 参考资料:

1. [ ] [sync.pool底层](https://www.cnblogs.com/yinbiao/p/15838957.html)
