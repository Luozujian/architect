//
// Created by Administrator on 2022-06-06.
//
#include <cstdio>
#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;


// 值类型和引用类型
void test2(string &s1) {
    s1 = "2";
}


//测试传值和传地址
void test1() {
    string s1 = "12312312";
    // 打印string类型
    printf("%s\n", s1.c_str());
    test2(s1);
    printf("%s\n", s1.c_str());
}


//string初始化
void test3() {
    string s = "lalalal";
}


//获取长度
void test4() {
    string s = "lalalal";
    //C++ 11开始 string s.size()的时间复杂度恒定O(1)
    //规范用法，别在for循环里面使用s.size()，放到外面计算
    printf("%d\n", s.size());
    printf("%d\n", s.length());
}

//字符串赋值
void test5() {
    string s1 = "lalala";
    string s2 = s1;
    printf("%s\n", s1.c_str());
    s2[0] = 'p';
    printf("%s\n", s2.c_str());
    printf("%s\n", s1.c_str());
}


// 字符串拼接append的开销会比s1 + s2这种写法的开销小很多
void test6() {
    //字符串append
    string s1 = "aasd";
    string s2 = "asdasdas";
    //o(n)的时间复杂度，可能不需要创建新对象
    s1.append(s2);
    printf("%s\n", s1.c_str());
    // 往末尾添加已给字符，比字符串拼接会稍微快一点点
    s1.push_back('s');
}

// 字符串比较
void test7() {
    string s1 = "a";
    string s2 = "ab";
    //相等输出0,s1的字典序 > s2则输出1，否则输出-1
    int result = s1.compare(s2);
    printf("%d\n", result);
}


// 获取子串
void test8() {
    string s2 = "abasdfasd";
    // 大于字符串的长度只会取到最后
    // O(n)的时间复杂度
    printf("%s\n", s2.substr(0, 23).c_str());
    // length = 0相当于不取
    printf("%s\n", s2.substr(0, 0).c_str());
}


void test9() {
    // 基本上可以认为就是O(n^2)的时间复杂度
    string s1 = "safasdfsadfsad";
    string s2 = "af";
    int pos = s1.find(s2, 0);
    //没找到就是string::npos
    if (pos == string::npos) {
        printf("notfind");
    } else {
        printf("find");
    }
}



void Get_Next(char *ttr,int *next,int tlen)
{
    next[0]=-1;
    for(int i=1,j=-1;i<tlen;i++)
    {
        while(j>-1 && ttr[j+1]!=ttr[i])
            j=next[j];
        if(ttr[j+1]==ttr[i])
            j++;

        next[i]=j;
    }
}

int Index_KMP(char *str,int slen,char *ttr,int tlen)
{
    int *next=new int[tlen];
    Get_Next(ttr,next,tlen);

    for(int i=0,j=-1;i<slen;i++)
    {
        while(j>-1 && ttr[j+1]!=str[i])
            j=next[j];
        if(ttr[j+1]==str[i])
            j++;

        if(j==tlen-1)
            return i-tlen+1;
    }

    return -1;
}

int Count_KMP(char *str,int slen,char *ttr,int tlen)
{
    int cnt=0;
    int *next=new int[tlen];
    Get_Next(ttr,next,tlen);

    for(int i=0,j=-1;i<slen;i++)
    {
        while(j>-1 && ttr[j+1]!=str[i])
            j=next[j];
        if(ttr[j+1]==str[i])
            j++;

        if(j==tlen-1)
            cnt++;
    }

    return cnt;
}



// kmp算法
void test10() {
    char x[] = "asdfasdfsad";
    char y[] = "asdf";
    int ans = Count_KMP(x, strlen(x), y, strlen(y));
    printf("%d\n", ans);
};



// string转char 数组，char转string
void test11() {
    string s1 = "hello";
    char s2[s1.length()];
    // o(n)的时间复杂度
    strcpy(s2,s1.c_str());
    printf("%s %d\n", s2, strlen(s2));

    string s3(s2);
    printf("%s\n", s3.c_str());
}


//double 转stringl类型
void test12() {
    double d = 3.1415;
    stringstream ss;
    ss << setprecision(2) << d;
    string s = ss.str();
    printf("%s\n", s.c_str());


    char s1[20];
    sprintf(s1, "%.1f", d);
    printf("%s\n", s1);

    //内存也会回复成0
    ss.str("");

    //不保留精度
    string s4 = to_string(d);


    // stringstream sss;
    int d1 = 12312312;
    ss << d1;
    cout << ss.str() << endl;
}

//string转其他类型
void test13() {
    string s = "31415231111";
    //cout << stoi(s) << endl;

    //string to double
    cout << stod(s) << endl;

    // string to long long
    cout << stoll(s) << endl;
}




bool cmp(char a, int b) {
    return b <= a;
}

// 对字符串排序
void test14 () {
    string s1 = "asfsadfsdfgsadfg";
    sort(s1.begin(), s1.end(), cmp);
    printf("%s\n", s1.c_str());
}


void test15() {
    string s1 = "asfsadfsdfgsadfg";
    lower_bound(s1.begin(),s1.end(),'a');
}


int main() {
    // test1();
    // test5();
    // test6();
    // test7();
    // test8();
    // test9();
    // test10();
    // test11();
    // test12();
    // test13();
    test14();
    return 0;
}
