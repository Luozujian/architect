//
// Created by Administrator on 2022-06-11.
//

#include <cstdio>
#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <list>
#include <set>


using namespace std;


struct Obj {
    int age;
    bool operator < (const Obj & B) const {
        return age < B.age;
    }
};

//set默认按照key升序，也可以降序
void test0() {
    // key升序
    set<int> q{1,2,3,4};
    //key降序
    set<int, greater<int>> st;

    set<Obj> s;
    //也可以自定义排序

}

//测试自定义排序,去重，只需要保障相同的元素的比较返回false即可 (nlogn，底层使用红黑树)
void test1() {
    set<Obj> s;
    s.insert(Obj{1});
    s.insert(Obj{2});
    s.insert(Obj{2});
    printf("%d\n", s.size());
}


//删除元素, O(logn)
void test2() {
    // 如何比较两个元属是否相等，就是A比较B返回false,然后B比较A也返回false，那就是相等了
    set<Obj> s;
    s.insert(Obj{1});
    s.insert(Obj{2});
    s.insert(Obj{2});
    int result = s.erase(Obj{1});
    printf("%d\n", result);
}



// 查找元属 O(logn); 没查到了的话，就返回s.end();
void test3() {
    set<Obj> s;
    s.insert(Obj{1});
    s.insert(Obj{2});
    s.insert(Obj{2});
    if (s.find(Obj{1}) != s.end()) {
        printf("lala");
    }
}

//对key的lower_bound,upper_bound
void test4() {
    set<Obj> s;
    s.insert(Obj{1});
    s.insert(Obj{2});
    s.insert(Obj{2});
    auto iter = s.upper_bound(Obj{1});
    if (iter != s.end()) {
        cout << iter->age << endl;
    }
}

// 遍历set
void test5() {
    set<Obj> s;
    s.insert(Obj{1});
    s.insert(Obj{2});
    s.insert(Obj{2});
    // 遍历
    for (auto obj : s) {
        cout << obj.age << endl;
    }
}

// 查询元素,o(lgn的时间复杂度)
void test6() {
    set<Obj> s;
    s.insert(Obj{1});
    s.insert(Obj{2});
    s.insert(Obj{2});

    //存在返回1，不存在返回0
    int result = s.count(Obj{1});
    printf("%d\n", result);

    auto iter = s.find(Obj{1});
    s.erase(iter, s.end());
    printf("%d\n", s.size());
}



//  删除一个区间的元素
void test7() {




}




int main() {
//    test1();
//    test2();
//    test3();
//    test4();
//    test5();
    test6();
}


