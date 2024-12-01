//
// Created by i1i on 11/10/24.
//

#include "GobangAI.h"
#include <QPoint>

GobangAI::GobangAI(QVector<QVector<int>>& board, int chess)
    : board(board), chess(chess), ruleChecker(board, aiChess)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));  // 初始化随机数种子
}

// 获取最佳落子位置
QPoint GobangAI::getBestMove(int depth) {
    ruleChecker.chess = aiChess;
    long long bestScore = -100000;
    QVector<QPoint> bestMoves;

    for (int row = 0; row < board.size(); ++row) {
        for (int col = 0; col < board[row].size(); ++col) {
            if (board[row][col] == 0 && !ruleChecker.isForbiddenMove(row, col)) {
                // 临时落子
                board[row][col] = aiChess;

                long long score = minimax(depth - 1, -100000, 100000, false);

                // 恢复棋盘状态
                board[row][col] = 0;

                if (score > bestScore) {
                    bestScore = score;
                    bestMoves.clear();
                    bestMoves.push_back({row, col});
                } else if (score == bestScore) {
                    bestMoves.push_back({row, col});
                }
            }
        }
    }

    if (bestMoves.empty()) {
        qDebug() << "No valid moves found!";
        // 返回一个默认位置，或者处理没有可行落子点的情况
        return {-1, -1};  // 示例返回无效位置
    }
    // 随机选择一个最优落子位置
    return bestMoves[std::rand() % bestMoves.size()];
}

long long GobangAI::evaluateBoard() {
    long long score = 0;

    // 遍历棋盘，检测每个位置的棋型并加分
    for (int row = 0; row < board.size(); ++row) {
        for (int col = 0; col < board[row].size(); ++col) {
            if (board[row][col] != 0) {
                // 当前棋子的颜色
                int chess = board[row][col];
                long long currentScore = 0;

                // 检查当前棋子在四个方向上的棋型
                currentScore += evaluateLine(row, col, 1, 0, chess);  // 水平方向
                currentScore += evaluateLine(row, col, 0, 1, chess);  // 垂直方向
                currentScore += evaluateLine(row, col, 1, 1, chess);  // 主对角线
                currentScore += evaluateLine(row, col, 1, -1, chess); // 副对角线

                // AI方得分增加，对手得分减少
                if (chess == aiChess) {
                    score += currentScore;
                } else {
                    score -= currentScore;
                }
            }
        }
    }
    return score;
}

// 辅助函数，用于评估某一方向上的棋型
long long GobangAI::evaluateLine(int row, int col, int dx, int dy, int chess) {
    int count = 1;       // 连续同色棋子数
    int openEnds = 0;    // 两端的空位数
    long long score = 0;

    // 向一个方向遍历
    int x = row + dx;
    int y = col + dy;
    while (isInBounds(x, y) && board[x][y] == chess) {
        count++;
        x += dx;
        y += dy;
    }
    if (isInBounds(x, y) && board[x][y] == 0) openEnds++;

    // 向相反方向遍历
    x = row - dx;
    y = col - dy;
    while (isInBounds(x, y) && board[x][y] == chess) {
        count++;
        x -= dx;
        y -= dy;
    }
    if (isInBounds(x, y) && board[x][y] == 0) openEnds++;

    // 根据棋型和空位数给分
    if (count >= 5) {
        score = WIN;
    } else if (count == 4) {
        score = openEnds == 2 ? LIVE_FOUR : RUSHED_FOUR;
    } else if (count == 3) {
        score = openEnds == 2 ? LIVE_THREE : SLEEP_THREE;
    } else if (count == 2) {
        score = openEnds == 2 ? LIVE_TWO : 0;
    }

    // 对禁手位置进行特殊处理
    if (chess == 1 && ruleChecker.isForbiddenMove(row, col)) {
        score = -10000;  // 禁手位的分数
    }

    return score;
}

// 辅助函数，判断坐标是否在棋盘范围内
bool GobangAI::isInBounds(int x, int y) const{
    return x >= 0 && x < board.size() && y >= 0 && y < board[0].size();
}

long long GobangAI::minimax(int depth, long long alpha, long long beta, bool isMaximizingPlayer) {
    if (depth == 0 || ruleChecker.judge()) {
        return evaluateBoard();
    }

    long long bestScore = isMaximizingPlayer ? -100000 : 100000;

    for (int row = 0; row < board.size(); ++row) {
        for (int col = 0; col < board[row].size(); ++col) {
            if (board[row][col] == 0 && !ruleChecker.isForbiddenMove(row, col)) {
                // 临时落子
                board[row][col] = isMaximizingPlayer ? aiChess : chess;

                long long score = minimax(depth - 1, alpha, beta, !isMaximizingPlayer);

                // 恢复棋盘状态
                board[row][col] = 0;

                // 更新最佳得分
                if (isMaximizingPlayer) {
                    bestScore = std::max(bestScore, score);
                    alpha = std::max(alpha, score);
                } else {
                    bestScore = std::min(bestScore, score);
                    beta = std::min(beta, score);
                }

                if (beta <= alpha) {
                    return bestScore;
                }
            }
        }
    }

    return bestScore;
}
