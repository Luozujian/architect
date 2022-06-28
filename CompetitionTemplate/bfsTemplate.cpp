//
// Created by Administrator on 2022-06-20.
//
#include <cstdio>
#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <list>
#include <queue>
using namespace std;
const int MAXN = 1e3 + 5;
char field[MAXN][MAXN];
int sx,sy,tx,ty;
int n, m;
int to[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
struct Node {
    int x, y, step;
};


int solve() {
    queue<Node> que;
    Node sNode;
    sNode.x = sx;
    sNode.y = sy;
    sNode.step = 0;
    que.push(sNode);
    while(!que.empty()) {
        sNode = que.front();que.pop();
        if (sNode.x == tx && sNode.y == ty) {
            return sNode.step;
        }

        for (int i = 0; i < 4; i++) {
            int tempX = sNode.x + to[i][0], tempY = sNode.y + to[i][1];
            if (tempX >= 0 && tempX < n && tempY >= 0 && tempY < m && field[tempX][tempY] != '#') {
                que.push(Node{tempX,tempY,sNode.step + 1});
            }
        }
    }
    return -1;
}


int main() {
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; i++) scanf("%s", field[i]);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (field[i][j] == 'S') {
                sx = i;
                sy = j;
            }
            if (field[i][j] == 'E') {
                tx = i;
                ty = j;
            }
        }
    }
    int minStep = solve();
    printf("%d\n", minStep);
}