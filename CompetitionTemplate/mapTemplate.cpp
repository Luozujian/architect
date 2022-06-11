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
#include <map>
#include <unordered_map>

using namespace std;

// map初始化操作，底层使用红黑树
void test() {
    map<int, int> dict;
}

//插入元素
void test1() {
    map<int, int> dict;
    dict[3] = 2;
    dict[4] = 5;
}

//遍历
void test2() {
    map<int, int> dict;
    dict[3] = 2;
    dict[4] = 5;

    for (auto iter = dict.begin(); iter != dict.end(); iter++) {
        cout << iter->first << " " << iter->second << endl;
    }

    for (auto item : dict) {
        cout << item.first << " " << item.second << endl;
    }

}

// 查找元属
void test3() {
    map<pair<int, int>, int> dict;
    dict[make_pair(1,1)] = 1;
    cout << dict[make_pair(1,1)] << endl;
}


//删除元属ologn
void test4() {
    map<pair<int, int>, int> dict;
    dict[make_pair(1,1)] = 1;
    dict[make_pair(1,2)] = 1;
    // 删除成功返回0，否则返回1
    int result = dict.erase(make_pair(1,2));
    cout << result << endl;
}



// unordered_map插是o(1)，如果不想利用有序性，使用unordered_map替换普通的map
void test5() {
    unordered_map<int, int> dict;
    dict[1] = 1;
    dict[2] = 1;
    dict[3] = 1;
    dict[4] = 1;
    for (auto item : dict) {
        cout << item.first << " " << item.second << endl;
    }
}



int main() {
//    test2();
//    test3();
//    test4();
    test5();
}




