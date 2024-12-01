//
// Created by i1i on 11/8/24.
//

#ifndef GOBANGBOARD_H
#define GOBANGBOARD_H

#include <QWidget>
#include <QImage>
#include <QPoint>
#include <QVector>
#include <QPixmap>
#include <QPushButton>
#include <QMessageBox>
#include <random>
#include "GobangRuleChecker.h"
#include "GobangAI.h"
#include <QTimer>


class GobangBoard : public QWidget {
    Q_OBJECT

public:
    explicit GobangBoard(QWidget *parent = nullptr);


protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

protected slots:
    void onStartButtonClicked();
    void onSurrenderButtonClicked();

private:
    QPixmap backgroundImage;
    QImage boardImage;         // 棋盘图片
    QPixmap   blackPieceImage;    // 黑棋图片
    QPixmap whitePieceImage;    // 白棋图片
    QPixmap   chessMaskImage;   // 棋子遮罩
    QPixmap hoverPieceImage;    // 预备落子图片
    QPixmap hoverMaskImage;
    QPixmap lihui;

    // 按钮
    QPixmap startButtonImg;
    QPixmap startButtonDisable;
    QPixmap startButtonMask;

    QPixmap surrenderButtonImg;
    QPixmap surrenderButtonDisable;
    QPixmap surrenderButtonMask;

    QPushButton *startButton;
    QPushButton *surrenderButton;

    QPixmap ruleBackground;


    int boardSize = 15;        // 棋盘大小 (15x15)
    int cellSize = 36;         // 单元格大小
    // 棋盘左上角坐标
    int basex = 247;
    int basey = 27;

    QVector<QVector<int>> board;  // 棋盘数组，0=空，1=黑棋，2=白棋
    QPoint hoverPos;              // 鼠标悬停位置
    bool showHoverPiece = false;  // 是否显示预备落子图片
    bool gameStarted = false;  // 用于标记游戏是否开始
    bool isMyTurn = false; // 是否轮到自己落子
    int chess = 1; // 1=黑棋，2=白棋
    GobangRuleChecker ruleChecker; // 禁手规则检查类
    GobangAI ai;

    void loadImages();
    void drawPieces(QPainter &painter);   // 绘制棋子
    void start();
    void boardReset();
    void winOrLose(bool win);
    void aiPlay();

};

#endif // GOBANGBOARD_H

