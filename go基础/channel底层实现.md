# 1.channel底层实现
##### 1.1 channel是什么？(3_2023_10_17)
channel是golang协程间通信的重要方式之一

##### 1.2 channel的底层结构题包含哪些字段？ (2_2023_09_20)
```
type hchan struct {
	qcount   uint // 队列中元素个数
	dataqsiz uint // 循环队列的大小
	buf      unsafe.Pointer // 指向循环队列
	elemsize uint16 // 通道里面的元素大小
	closed   uint32 // 通道关闭的标志
	elemtype *_type // 通道元素的类型
	sendx    uint   // 待发送的索引，即循环队列中的队尾指针front
	recvx    uint   // 待读取的索引，即循环队列中的队头指针rear
	recvq    waitq  // 接收等待队列
	sendq    waitq  // 发送等待队列
	lock mutex // 互斥锁
}

type waitq struct {
	first *sudog
	last  *sudog
}

type sudog struct {
	...
	g *g // 当前阻塞的G
	...
	next     *sudog
	prev     *sudog
	elem     unsafe.Pointer
    ...
}
```

![image](https://github.com/Luozujian/architect/assets/27532970/b1ea84b6-872e-4814-bc7d-5fe30548c1b7)


##### 1.3 channel分为那两种类型？ (2_2023_09_20)
channel分为有缓冲通道和无缓冲通道


##### 1.3 makechan的主要逻辑？ (2_2023_09_20)
makechan的主要逻辑分为三类
1. 无缓冲: 只分配hchan内存空间
2. 有缓冲&元素无指针: 一次分配hchan和mem(数组)内存,数组内存和hchan内存在一起
3. 有缓冲&元素有指针: 先分配hchan、再分配mem内存，两者内存不在一起

初始化一些元素大小，类型，数组大小等值

```c++
func makechan(t *chantype, size int) *hchan {
	elem := t.elem

	// compiler checks this but be safe.
	if elem.size >= 1<<16 {
		throw("makechan: invalid channel element type")
	}
	if hchanSize%maxAlign != 0 || elem.align > maxAlign {
		throw("makechan: bad alignment")
	}

	mem, overflow := math.MulUintptr(elem.size, uintptr(size))
	if overflow || mem > maxAlloc-hchanSize || size < 0 {
		panic(plainError("makechan: size out of range"))
	}

	// Hchan does not contain pointers interesting for GC when elements stored in buf do not contain pointers.
	// buf points into the same allocation, elemtype is persistent.
	// SudoG's are referenced from their owning thread so they can't be collected.
	// TODO(dvyukov,rlh): Rethink when collector can move allocated objects.
	var c *hchan
	switch {
	case mem == 0:
		// Queue or element size is zero.
		c = (*hchan)(mallocgc(hchanSize, nil, true))
		// Race detector uses this location for synchronization.
		c.buf = c.raceaddr()
	case elem.ptrdata == 0:
		// Elements do not contain pointers.
		// Allocate hchan and buf in one call.
		c = (*hchan)(mallocgc(hchanSize+mem, nil, true))
		c.buf = add(unsafe.Pointer(c), hchanSize)
	default:
		// Elements contain pointers.
		c = new(hchan)
		c.buf = mallocgc(mem, elem, true)
	}

	c.elemsize = uint16(elem.size)
	c.elemtype = elem
	c.dataqsiz = uint(size)
	lockInit(&c.lock, lockRankHchan)

	if debugChan {
		print("makechan: chan=", c, "; elemsize=", elem.size, "; dataqsiz=", size, "\n")
	}
	return c
}
```

##### 1.4 发送数据到channel的逻辑？ (2_2023_09_20)
1. 如果读等待队列不为空，则将从从队首获取一个睡眠的G，并且将数据给他，然后将其唤醒
2. 如果循环数组未满，则将数据写入到队尾，返回
3. 如果循环数组满了，则阻塞，加入到sendq,并且挂起等待唤醒


##### 1.5 接收数据的大概逻辑？ (2_2023_09_20)
1. 判断channel是否初始化，没有初始化则panic
2. 加互斥锁
3. 如果通道已经关闭 & 通道缓冲没有元素，则直接返回
4. 如果sendq队列有数据，则取队首元素给recver，并且唤醒队首的G
5. 如果缓冲区还有元素，则取缓冲区队首元素，给recver
6. 否则阻塞，加入到recver队尾


##### 1.6 关闭channel的大概逻辑？  (2_2023_09_20)
1. 加锁
2. 关闭已经关闭的通道 => panic
3. 修改close字段值为1，表示已经关闭
4. 将sendq队列中sudog加入到 glist队列
5. 将glist队列中的G都唤醒

##### 1.7 channel是如何解决并发冲突的？  (2_2023_09_20)
使用互斥锁来解决冲突的，在操作之前都会加互斥锁

##### 1.8 









##### 参考资料：

1. [ ] [1.图解channel底层实现](https://i6448038.github.io/2019/04/11/go-channel/)
2. [ ] [2.Golang源码分析系列之Channel底层实现](https://juejin.cn/post/6898132039812431885)
3. [ ] [3.Go底层原理 - channel的底层实现原理？](https://juejin.cn/post/7129085275266875422)
4. [ ] [4.深入 Go 并发原语 — Channel 底层实现 写的非常详细](https://halfrost.com/go_channel/)
