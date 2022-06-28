//
// Created by Administrator on 2022-06-07.
//
#include <cstdio>
#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
using namespace std;


//vector的复制 o(n)的时间复杂度
void test1() {
    vector<int> vec{1,2,3};
    vector<int> ve{4,5};
    ve = vec;
    printf("%d\n", ve.size());
    vec.push_back(4);
    printf("%d\n", ve.size());
}

//求大小，时间复杂度并不一定是o(1)
void test2() {
    vector<int> vec{1,23,3,45,6};
    printf("%d\n", vec.size());
}

// 删除最后一个元素
void test3() {
    vector<int> vec{1,23,3,45,6};
    vec.pop_back();
}

//vector比较 o(n)
void test4() {
    vector<int> vec1 = {1,2,3};
    vector<int> vec2 = {1,2,3};
    printf("%d\n", vec1 == vec2);
}


//vector比较，以字典序进行比较 o(n)
void test5() {
    vector<int> vec1 = {1,2,3};
    vector<int> vec2 = {2,2,3};
    printf("%d\n", vec1 < vec2);
}


//使用迭代器进行遍历,o(n)
void test6() {
    vector<int> vec1 = {1,2,3};
    auto iter = vec1.begin();
    while (iter != vec1.end()) {
        cout << *iter << endl;
        iter++;
    }
}

//插入时间复杂度O(n)
void test7() {
    vector<int> a{1, 2, 3};
    auto it1 = a.begin();  // 返回一个迭代器类型，一般来说我们并不关心迭代器具体的数据类型
    auto it2 = a.insert(it1 + 1, {3,3});
    it2 = a.begin();
    while(it2 != a.end()) {
        cout << *it2 << endl;
        it2++;
    }
}

//删除元素 O(n)
void test8() {
    vector<int> a{1, 2, 3};
    auto it1 = a.begin();
    a.erase(it1 + 1, it1 + 2);
    printf("%d\n", a[0]);
}


//swap() 交换数据
void test9 () {
    vector<int> a{1, 2, 3};
    vector<int> a1{2, 1};
    swap(a, a1);
    printf("%d\n", a1[0]);
}

//vector反转
void test10 () {
    vector<int> a{1, 2, 3};
    reverse(a.begin(),a.end());
    printf("%d\n", a[0]);
}

//求vector最大值
void  test11() {
    vector<int> a{1, 2, 3};
    int b[5] = {3,2,1,4,5};
    auto iter = max_element(a.begin(),a.end());
    cout << *iter << endl;
    int index = max_element(b, b + 5) - b;
    iter = min_element(a.begin(),a.end());
    cout << *iter << " " << index << endl;
}


// 二分求值
void test12() {
    vector<int> a{1, 2, 3, 4};
    // 大于等于value的下标
    auto iter = lower_bound(a.begin(), a.end(), 4);
    cout << iter - a.begin() << endl;

    //大于value的下标
    auto iter1 = upper_bound(a.begin(), a.end(), 5);
    cout << iter1 - a.begin() << endl;

    int b[4] = {1,2,3,4};
    cout << lower_bound(b, b + 4, 4) - b << endl;

    int tempA = 2, tempB = 3;
    swap(tempA, tempB);
    printf("%d %d\n", tempA, tempB);

}



// unique去重
void test13() {
    vector<int> a{1, 2, 3, 8, 8, 8};
    int iter = unique(a.begin(),a.end()) - a.begin();
    cout << iter << endl;
}



int main() {
    // test1();
    // test4();
    // test6();
    // test5();
//    test7();
//    test8();
//    test9();
//    test10();
//    test11();
//    test12();
    test13();
}
