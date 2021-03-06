/**
 * Copyright (c) 2019 MMMMMMoSky All rights reserved.
 */

#include "RandomWalk.h"

using namespace std;

/***********/
/* 核心函数 */
/***********/
void randomWalk(int N, bool &ok, int &length)
{
    static const int dx[4] = {0, 1, -1, 0};
    static const int dy[4] = {1, 0, 0, -1};

    int x = N / 2; // 初始位置
    int y = N / 2;
    length = 0;

    // 使用一个二维数组记录一个点是否访问过
    vector<vector<bool>> visited(N, vector<bool>(N, false));
    visited[x][y] = true;

    int validDirections[4];
    int cnt;

    while (true)
    {

        // 将可选的方向放入数组中
        // 注意: 边界之外可选, 只有已走过的位置不可以选
        cnt = 0;
        for (int i = 0; i < 4; i++)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx < 0 || nx >= N ||
                ny < 0 || ny >= N || !visited[nx][ny])
            {
                validDirections[cnt++] = i;
            }
        }

        if (cnt == 0)  // 无路可走
        {
            ok = false;
            return;
        }

        // 随机选取方向
        int rnd = Random::randint(0, cnt - 1);
        x += dx[validDirections[rnd]];
        y += dy[validDirections[rnd]];
        length++;

        if (x < 0 || x >= N || y < 0 || y >= N) // 走出边界, 成功
        {
            ok = true;
            return;
        }

        visited[x][y] = true;
    }
}

void repeatTest(
    int N, int T,
    double &failed,               // 失败概率
    double &averLength,           // 平均路径长度
    double &averFailedLength,     // 失败时平均路径长度
    double &averSucceededLength)  // 成功时平均路径长度
{
    failed = 0;
    averLength = 0;
    averFailedLength = 0;
    averSucceededLength = 0;
    for (int i = 0; i < T; i++)
    {
        bool ok;
        int len;
        randomWalk(N, ok, len);
        averLength += len;
        if (ok)
        {
            averSucceededLength += len;
        }
        else
        {
            averFailedLength += len;
            failed += 1;
        }
    }
    averSucceededLength /= T;
    averFailedLength /= failed;
    averLength /= T - failed;
    failed /= T;
}

/* 累计用时 20min */