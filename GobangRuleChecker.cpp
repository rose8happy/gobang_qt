//
// Created by i1i on 11/10/24.
//
#include "GobangRuleChecker.h"


GobangRuleChecker::GobangRuleChecker(QVector<QVector<int>>& board, int chess)
    : board(board), chess(chess) {}

// 判断某个位置是否为禁手
bool GobangRuleChecker::isForbiddenMove(int x, int y) {
    if (isEmpty(x, y)) {
        return checkForbidden(x, y);
    }
    return false;
}

// 检查禁手的相关规则
bool GobangRuleChecker::checkForbidden(int x, int y) {
    if (chess == 2)
    {
        // 白棋禁手，白棋第一手，只能下在下半区
        for (auto & i : board)
        {
            for (int j : i)
            {
                if (j == 2)
                {
                    return false;
                }
            }
        }
        return x >= board.size() / 2;
    }
    if (chess == 1)
    {
        // 检查活三、四子以及五子连珠的禁手
        if (checkThreeAndFour(x, y, 1)) {
            return true; // 黑棋禁手
        }

        if (checkOverline(x, y, 1)) {
            return true; // 黑棋禁手
        }
    } else
    {
        qDebug() << "Error: Invalid chess type";
        qDebug() << chess;
    }


    return false;
}

// 检查三三、四四的禁手
bool GobangRuleChecker::checkThreeAndFour(int x, int y, int color) {
    int jd = judge();
    if (jd == 1)
    {
        return false;
    }
    // 检查四个方向
    int directions[4][2] = {
        {1, 0}, // 水平右
        {0, 1}, // 垂直下
        {1, 1}, // 主对角线（右下）
        {1, -1} // 副对角线（右上）
    };

    int countThree = 0; // 记录找到的活三个数
    int countFour = 0;  // 记录找到的活四个数

    for (auto & direction : directions) {
        int dx = direction[0];
        int dy = direction[1];

        // 检查当前方向上的活三和活四
        int count = 1; // 当前棋子已算上

        // 向一个方向延伸
        int nx = x + dx;
        int ny = y + dy;
        while (nx >= 0 && nx < board.size() && ny >= 0 && ny < board[0].size() && board[nx][ny] == color) {
            count++;
            nx += dx;
            ny += dy;
        }

        // 向相反方向延伸
        nx = x - dx;
        ny = y - dy;
        while (nx >= 0 && nx < board.size() && ny >= 0 && ny < board[0].size() && board[nx][ny] == color) {
            count++;
            nx -= dx;
            ny -= dy;
        }

        // 判断活三和活四
        if (count == 3) {
            // 如果在当前方向上形成活三，检查两端是否有空位
            if ((x + dx * 3 >= 0 && x + dx * 3 < board.size() && y + dy * 3 >= 0 && y + dy * 3 < board[0].size() && board[x + dx * 3][y + dy * 3] == 0) &&
                (x - dx >= 0 && x - dx < board.size() && y - dy >= 0 && y - dy < board[0].size() && board[x - dx][y - dy] == 0)) {
                countThree++;
                }
        }

        if (count == 4) {
            // 如果在当前方向上形成活四，检查两端是否有空位
            if ((x + dx * 4 >= 0 && x + dx * 4 < board.size() && y + dy * 4 >= 0 && y + dy * 4 < board[0].size() && board[x + dx * 4][y + dy * 4] == 0) &&
                (x - dx >= 0 && x - dx < board.size() && y - dy >= 0 && y - dy < board[0].size() && board[x - dx][y - dy] == 0)) {
                countFour++;
                }
        }
    }

    // 如果找到两个活三或两个活四，返回true，表示禁手
    return (countThree >= 2 || countFour >= 2);
}


// 检查长连的禁手
bool GobangRuleChecker::checkOverline(int x, int y, int color) {
    // 检查四个方向
    int directions[4][2] = {
        {1, 0}, // 水平右
        {0, 1}, // 垂直下
        {1, 1}, // 主对角线（右下）
        {1, -1} // 副对角线（右上）
    };

    for (auto & direction : directions) {
        int dx = direction[0];
        int dy = direction[1];

        int count = 1; // 当前棋子已算上

        // 向一个方向延伸
        int nx = x + dx;
        int ny = y + dy;
        while (nx >= 0 && nx < board.size() && ny >= 0 && ny < board[0].size() && board[nx][ny] == color) {
            count++;
            nx += dx;
            ny += dy;
        }

        // 向相反方向延伸
        nx = x - dx;
        ny = y - dy;
        while (nx >= 0 && nx < board.size() && ny >= 0 && ny < board[0].size() && board[nx][ny] == color) {
            count++;
            nx -= dx;
            ny -= dy;
        }

        // 检查是否超过五个连续
        if (count > 5) {
            return true; // 超过五个，属于长连
        }
    }

    return false; // 没有长连
}

int GobangRuleChecker::judge() {
    // 遍历整个棋盘，检查每个位置的棋子是否形成五连珠
    for (int x = 0; x < board.size(); ++x) {
        for (int y = 0; y < board[0].size(); ++y) {
            int color = board[x][y];
            if (color != 0) { // 如果这个位置有棋子
                // 检查四个方向是否形成五连珠
                if (checkFiveInLine(x, y, 1, 0, color) || // 水平
                    checkFiveInLine(x, y, 0, 1, color) || // 垂直
                    checkFiveInLine(x, y, 1, 1, color) || // 右下对角线
                    checkFiveInLine(x, y, 1, -1, color))  // 左下对角线
                {
                    return color; // 如果找到五连珠，返回获胜方的颜色（1=黑棋，2=白棋）
                }
            }
        }
    }
    return 0; // 如果没有找到五连珠，返回0表示无人获胜
}

// 辅助函数：检查某个方向上是否形成五连珠
bool GobangRuleChecker::checkFiveInLine(int x, int y, int dx, int dy, int color) const {
    int count = 1;
    // 正向方向检查
    for (int i = 1; i < 5; ++i) {
        int nx = x + i * dx, ny = y + i * dy;
        if (nx >= 0 && nx < board.size() && ny >= 0 && ny < board[0].size() && board[nx][ny] == color) {
            ++count;
        } else {
            break;
        }
    }
    // 反向方向检查
    for (int i = 1; i < 5; ++i) {
        int nx = x - i * dx, ny = y - i * dy;
        if (nx >= 0 && nx < board.size() && ny >= 0 && ny < board[0].size() && board[nx][ny] == color) {
            ++count;
        } else {
            break;
        }
    }
    return count >= 5; // 如果五子连珠，返回true
}


