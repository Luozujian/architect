//
// Created by Administrator on 2022-06-08.
//
#include <cstdio>
#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <list>

using namespace std;


// 自定义排序,这里的age是第一个，B是第二个
struct student {
    int age;
    bool operator < (const student& B) {
        return age <= B.age;
    }
};

// list初始化O(n)时间复杂度
void test() {
    int b[5] = {1,3,2,4,6};
    list<int> lt{1,2,3};
    list<int> lt1(b, b + 5);
}


// list遍历
void test1() {
    list<int> lt{1,2,3};
    for (int & iter : lt) {
        cout << iter << endl;
    }
}


bool cmp(int a, int b) {
    return a > b;
}

// 排序，时间复杂度nlog(n)
void test2() {
    list<student> lt;
//    student student1; student1.age = 1;
    lt.push_back(student{1});
    lt.push_back(student{2});
    lt.push_back(student{3});
    lt.sort();
    for (student & item : lt) {
        printf("%d\n", item);
    }
}

// 插入，弹出最后一个元素
void test3() {
    list<student> lt;
//    student student1; student1.age = 1;
    lt.push_back(student{1});
    lt.push_back(student{2});
    lt.push_back(student{3});
    auto iter = lt.begin();
    lt.erase(iter);
    cout << iter ->age << endl;
    cout << lt.size() << endl; // o(1)
    //lt.erase();
}

// list去重 O(n)的时间复杂度
void test4() {
    list<int> lt = {1,2,2,3,4,5};
    auto item = unique(lt.begin(),lt.end());
    lt.erase(item, lt.end());
    for (int &item :lt) {
        printf("%d\n",item);
    }
}


//list删除元素
void test5() {
    list<int> lt = {1,2,2,3,4,5};
    auto iter = lt.begin();
    lt.erase(iter++);
    cout << * iter << endl;
}


//list指定位置插入数据
void test6() {
    list<int> lt = {1,2,2,3,4,5};
    auto iter = lt.begin();
    lt.erase(iter++);
    cout << * iter << endl;
    lt.insert(iter, 7);
    cout << * (--iter) << endl;
    iter = lt.begin();
    while(iter != lt.end()) {
        cout << *iter << endl;
        iter++;
    }
}


bool judge(int a) {
    return a <= 3;
}

//按条件删除元素
void test7() {
    list<int> lt = {1,2,2,3,4,5};
    lt.remove_if(judge);
    auto iter = lt.begin();
    while(iter != lt.end()) {
        cout << *iter << endl;
        iter++;
    }
}



int main() {
//    test2();
//    test3();
//    test4();
//    test5();
//    test6();
    test7();
}

