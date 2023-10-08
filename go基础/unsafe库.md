#### 1. Unsafe库的使用
##### 1.1 什么是unsafe库？(3_2023_10_08)
类型安全的指针不提供对地址进行算术运算、任意两个类型指针相互交互存在许多限制，因此官方提供非安全指针，避开诸多限制，在某些情况下可以写出更加优雅的代码。

##### 1.2 什么是类型安全指针，什么是非类型安全指针? (3_2023_10_08)
类型安全指针就是平时使用的 *int、*string等等，而非类型安全指针就是unsafe库下的指针，安全与非安全体现在是否受到Go1兼容性保证，如果不受到兼容性保证，可能低版本代码
在高版本就无法编译通过。因此业务代码不建议使用unsafe库的相关功能

##### 1.3 unsafe库包含哪几个函数？(3_2023_10_08)
```c++
// 表示任意类型
type ArbitraryType int
type Pointer *ArbitraryType

主要包含如下三个函数:
1. 返回一个变量占用的内存字节数
func SizeOf(x ArbitraryType) uintptr

2. 返回结构体某字段的地址相对于此结构体起始地址的偏移量
func Offsetof(x ArbitraryType) uintptr

3. 返回对齐系数
func alignOf(x ArbitraryType) uintptr
```

##### 1.4 指针占多少字节？(3_2023_10_08)
指针指向的是地址，计算机的地址需要几个字节记录，指针就占几个字节，所以32位的计算机占4个字节，而64位的计算机占8个字节


##### 1.5 什么是uintptr类型? (3_2023_10_08)
uintptr 官网的介绍足以存储指针所指向地址的值，便于地址


##### 1.6 如何利用unsafe下面的指针进行类型转换？  (3_2023_10_08)
1. 任意类型的指针可以转为 Pointer
2. Pointer 可以转为任意类型的指针
3. uintptr 可以转为 Pointer
4. Pointer 可以转为 uintptr

```c++
type Student struct {
	Name string
	Age int
}
type Teacher struct {
	Name string
	Age int
}
func main() {
	student := &Student{
		Name: "lqdsadasssssssssssssssssssssssssqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqssssssssssssssssss",
		Age: 1,
	}
	age := *(*int)(unsafe.Pointer(uintptr(unsafe.Pointer(student)) + unsafe.Offsetof(student.Age)))
	fmt.Printf("age = %+v\n", age)
}
```

#### 1.7 




##### 参考资料:
1. [ ] [基础使用](https://juejin.cn/post/7083853142403579911)
