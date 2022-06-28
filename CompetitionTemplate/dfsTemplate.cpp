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

using namespace std;


const int MAXN = 1e3 + 5, MAXM = 1e3 +5;

int N, M;
char field[MAXN][MAXM];
void dfs(int x, int y) {
    field[x][y] = '.';
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int targetX = x + i, targetY = y + j;
            if (targetX >= 0 && targetX < N && targetY >= 0 && targetY < M && field[targetX][targetY] != '.') dfs(targetX,targetY);
        }
    }
}



void solve() {
    int ans = 0;
    for (int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            if (field[i][j] == 'W') {
                dfs(i,j);
                ans++;
            }
        }
    }
    printf("%d\n", ans);
}



int main() {
    scanf("%d %d", &N, &M);
    for (int i = 0; i < N; i++)scanf("%s", field[i]);
    solve();
}