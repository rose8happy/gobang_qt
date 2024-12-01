//
// Created by i1i on 11/10/24.
//

#ifndef GOBANGAI_H
#define GOBANGAI_H

#include <QVector>
#include "GobangRuleChecker.h"
#include <QPoint>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// 定义不同棋型的分数
const int WIN = 10000;
const int LIVE_FOUR = 1000;
const int RUSHED_FOUR = 500;
const int LIVE_THREE = 100;
const int SLEEP_THREE = 50;
const int LIVE_TWO = 10;

#define aiChess (3-chess)

class GobangAI
{
public:
    explicit GobangAI(QVector<QVector<int>>& board, int chess);
    QVector<QVector<int>>& board; // 棋盘状态
    int chess;                   // 当前执棋，1=黑棋，2=白棋
    // 计算最佳下棋位置
    QPoint getBestMove(int depth=2);

private:

    GobangRuleChecker ruleChecker; // 禁手规则检查类

    //bool isFirstMoveForWhite();
    //QPoint getFirstMoveForWhite();
    //QPoint findFirstBlackMove();

    long long evaluateBoard();
    long long minimax(int depth, long long alpha, long long beta, bool isMaximizingPlayer);
    long long evaluateLine(int row, int col, int dx, int dy, int chess);
    bool isInBounds(int x, int y) const;

};

#endif // GOBANGAI_H

