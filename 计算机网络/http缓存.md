# http 缓存


![image](https://github.com/Luozujian/architect/assets/27532970/16a70757-fd22-43af-b116-5e833c5e53da)



#### 1. 什么是http缓存？ (3_2024_12_09)
某些资源变更频率很低，如果每次获取都要请求服务端获取数据，效率较低，浪费网络、服务器资源。

为解决上述问题，http协议提供相关字段来支持客户端缓存。


#### 2. HTTP缓存的分类？  (3_2024_12_09)
按照是否需要请求服务器可分为强缓存和协商缓存，强缓存指的是缓存有效则无需请求服务器，协商缓存指的是，无论如何都需要请求服务器，服务器判断缓存有效，则返回304
表示缓存可用，如果返回200，则表示缓存需要更新。

按照缓存存储的位置又可分别公共缓存和私有缓存，公共缓存允许代理服务器去缓存，比如nginx，而私有缓存只允许客户端进行缓存


#### 3. 强缓存种类？  (3_2024_12_09)
响应头: 
1. expire 过期时间，需要注意客户端时间和服务器时间的不一致性
2. cache-control max-age 缓存多长时间


![image](https://github.com/Luozujian/architect/assets/27532970/68a5d323-731f-4393-912e-f0ffda37512f)



#### 4. 协商缓存字段? (3_2024_12_09)
缓存没过期则返回304，缓存过期则返回200

响应头
1. etag/last-modify

请求头:
1. etag-not-match/last-modify-since



#### 5. 游览器缓存有哪些？(3_2024_12_09)
cookie、sessionStorage、localStorage

cookie，每次请求服务器都会带上，大多数游览器大小限制是4KB
sessionStorage，单次回话有效，大多数游览器大小限制是5MB+
localStorage，不删就一直在，大多数游览器大小限制是5MB+






