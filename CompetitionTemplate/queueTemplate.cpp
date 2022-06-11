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
#include <queue>
#include <deque>

using namespace std;



struct Node {
    int age;
    bool operator < (const Node& B) const {
        return age < B.age;
    }
};


// queue初始化，先进先出
void test() {
    queue<int> que;
}


// queue 添加元素
void test1() {
    queue<int> que;
    que.push(1);
    que.push(2);
    que.push(3);
}


// queue 访问队首，弹出队首
void test2() {
    queue<int> que;
    que.push(1);
    que.push(2);
    que.push(3);
    int result = que.front();
    que.pop();
    cout << result << endl;
}

//queue 访问队尾
void test3() {
    queue<int> que;
    que.push(1);
    que.push(2);
    que.push(3);
    int result = que.back();
    cout << result << endl;
}

// 优先队列初始化
void test4() {
    //默认是vector存储的大顶堆 less<>大顶堆， greater小顶堆
    priority_queue<int, vector<int>, greater<>> que;
    que.push(1);
    que.push(2);
    que.push(3);
    que.push(4);
    int result = que.top();
    cout << result << endl;
}


// 自定义排序，优先队列优先取的是最后一个元素 Nlog(n)，插入一个数是logn，取出一个数是O(1)的。
void test5() {
    priority_queue<Node> que;
    que.push(Node{1});
    que.push(Node{2});
    que.push(Node{3});
    que.push(Node{4});
    cout << que.top().age << endl;
}


void test6() {
    priority_queue<Node> que;
    que.push(Node{1});
    que.push(Node{2});
    que.push(Node{3});
    que.push(Node{4});
    que.pop();
}

//双端队列初始化
void test7() {
    deque<int> qu;
}


//双端队列插入元素
void test8() {
    deque<int> qu;
    qu.push_front(1);
    qu.push_back(2);
    cout << qu.front() << endl;
    cout << qu.back() << endl;
}


//双端队列遍历
void test9() {
    deque<int> qu;
    qu.push_front(1);
    qu.push_back(2);
    for(int & iter : qu) {
        cout << iter << endl;
    }
}

//双端队列删除一个元素 可以看成就是链表取实现的
void test10() {
    deque<int> qu;
    qu.push_front(1);
    qu.push_back(2);
    qu.erase(qu.begin()); //删除队头和队尾是O(1)的，查找是o(n)
}

//双端队列取元素
void test11() {
    deque<int> qu;
    qu.push_front(1);
    qu.push_back(2);
    qu.pop_back();
    qu.pop_front();
}



int main() {
//    test2();
//    test3();
//    test4();
//    test5();
//    test8();
    test9();
}
