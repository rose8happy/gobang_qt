//
// Created by i1i on 11/8/24.
//
#include "GobangBoard.h"
#include <QPainter>
#include <QMouseEvent>
#include <QLabel>
#include <QDebug>
#include <QBitmap>


void GobangBoard::loadImages()
{
    backgroundImage.load(":/images/interface/background.png");

    boardImage.load(":/images/棋盘/木质棋盘.png");

    blackPieceImage.load(":/images/棋盘/大黑棋.png");
    whitePieceImage.load(":/images/棋盘/大白棋.png");
    chessMaskImage.load(":/images/棋盘/大棋子遮罩.png");
    hoverPieceImage.load(":/images/棋盘/追踪显示期望.png");
    hoverMaskImage.load(":/images/棋盘/追踪显示遮罩.png");
    lihui.load(":/images/interface/立绘01.png");
    if (hoverPieceImage.isNull() || whitePieceImage.isNull() || blackPieceImage.isNull()
        || backgroundImage.isNull() || boardImage.isNull() || chessMaskImage.isNull()
        ||hoverMaskImage.isNull()) {
        qDebug() << "棋盘图片加载失败";
        }

    // 加载按钮
    startButtonImg.load(":/images/interface/准备.png");
    startButtonDisable.load(":/images/interface/准备_禁用.png");
    startButtonMask.load(":/images/interface/准备遮罩.png");
    surrenderButtonImg.load(":/images/interface/认输_启用.png");
    surrenderButtonDisable.load(":/images/interface/认输02.png");
    surrenderButtonMask.load(":/images/interface/认输01.png");
    if (startButtonImg.isNull() || surrenderButtonImg.isNull() || startButtonMask.isNull()
        || surrenderButtonDisable.isNull() || startButtonDisable.isNull()
        || surrenderButtonMask.isNull()) {
        qDebug() << "按钮图片加载失败";
    }
    // ruleBackground.load(":/images/interface/规则背景.png");
    // if (ruleBackground.isNull())
    // {
    //     qDebug() << "规则背景图片加载失败";
    // }
}

GobangBoard::GobangBoard(QWidget *parent) : QWidget(parent), ruleChecker(board, chess), ai(board,chess)
{
    loadImages();
    // 使用黑白图片生成QBitmap
    QBitmap chessMaskBitmap = chessMaskImage.createMaskFromColor(Qt::white);
    // 将遮罩应用到棋子
    blackPieceImage.setMask(chessMaskBitmap);
    whitePieceImage.setMask(chessMaskBitmap);

    QBitmap hoverMaskBitmap = hoverMaskImage.createMaskFromColor(Qt::white);
    hoverPieceImage.setMask(hoverMaskBitmap);

    // 初始化棋盘数组为 0（空）
    board = QVector<QVector<int>>(boardSize, QVector<int>(boardSize, 0));

    // 设置窗口大小为背景图片大小
    setFixedSize(backgroundImage.size());

    setMouseTracking(true);

    QBitmap startButtonMaskBitmap = startButtonMask.createMaskFromColor(Qt::white);
    startButtonImg.setMask(startButtonMaskBitmap);
    startButtonDisable.setMask(startButtonMaskBitmap);

    QBitmap surrenderButtonMaskBitmap = surrenderButtonMask.createMaskFromColor(Qt::white);
    surrenderButtonImg.setMask(surrenderButtonMaskBitmap);
    surrenderButtonDisable.setMask(surrenderButtonMaskBitmap);

    startButton = new QPushButton(this);
    startButton->setIcon(QIcon(startButtonImg));
    startButton->move(5,413);
    startButton->setStyleSheet("border: none; background: transparent;");
    startButton->setIconSize(startButtonImg.size());
    startButton->setFixedSize(startButtonImg.size());
    // 连接点击事件
    connect(startButton, SIGNAL(clicked()), this, SLOT(onStartButtonClicked()));
    surrenderButton = new QPushButton(this);
    surrenderButton->setIcon(QIcon(surrenderButtonImg));
    surrenderButton->move(5,493);
    surrenderButton->setStyleSheet("border: none; background: transparent;");
    surrenderButton->setIconSize(surrenderButtonImg.size());
    surrenderButton->setFixedSize(surrenderButtonImg.size());
    connect(surrenderButton, SIGNAL(clicked()), this, SLOT(onSurrenderButtonClicked()));

}

void GobangBoard::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    // 绘制背景
    painter.drawPixmap(rect(),backgroundImage);
    // 绘制立绘
    painter.drawPixmap(0,0,lihui);
    // 绘制棋盘
    painter.drawImage(basex,basey, boardImage);
    // 绘制棋子
    drawPieces(painter);

    // 绘制预备落子图片
    if (showHoverPiece) {
        painter.drawPixmap(hoverPos.x() * cellSize+ basex, hoverPos.y() * cellSize+ basey, hoverPieceImage);
    }
}


void GobangBoard::onStartButtonClicked() {
    if (!gameStarted) {
        start();
        startButton->setIcon(QIcon(startButtonDisable));  // 切换为灰色按钮
        startButton->setIconSize(startButtonDisable.size()); // 确保设置大小
        gameStarted = true;
    }
}

void GobangBoard::start()
{
    boardReset();
    // 使用随机设备获取随机种子
    std::random_device rd;
    std::mt19937 gen(rd());  // 使用 Mersenne Twister 引擎
    std::uniform_int_distribution<> dis(1, 2);  // 设置 1 或 2 的分布
    // 生成 1 或 2 的随机数
    chess = dis(gen);
    ai.chess = chess;
    if (chess == 1) {
        isMyTurn = true;
        QMessageBox::information(this, "提示", QString::fromUtf8("You play black and go first"));
    } else if (chess==2)
    {
        isMyTurn = false;
        QMessageBox::information(this, "提示", QString::fromUtf8("You play white and go second"));
        aiPlay();
    } else
    {
        QMessageBox::information(this, "提示", QString::fromUtf8("Error"));
    }
}

void GobangBoard::onSurrenderButtonClicked()
{
    if (gameStarted) winOrLose(false);
}

void GobangBoard::boardReset()
{
    for (int i=0;i<boardSize;i++)
    {
        board[i].fill(0);
    }
    update();
}

void GobangBoard::winOrLose(bool win)
{
    if (win)
    {
        QMessageBox::information(this, "提示", QString::fromUtf8("You win!"));
    } else
    {
        QMessageBox::information(this, "提示", QString::fromUtf8("You lose!"));
    }
    gameStarted = false;
    isMyTurn = false;
    startButton->setIcon(QIcon(startButtonImg));
}

void GobangBoard::drawPieces(QPainter &painter) {
    for (int row = 0; row < boardSize; ++row) {
        for (int col = 0; col < boardSize; ++col) {
            int x = col * cellSize + basex;
            int y = row * cellSize + basey;
            if (board[row][col] == 1) {  // 黑棋
                painter.drawPixmap(x, y, blackPieceImage);
            } else if (board[row][col] == 2) {  // 白棋
                painter.drawPixmap(x, y, whitePieceImage);
            }
        }
    }
}

void GobangBoard::mousePressEvent(QMouseEvent *event) {
    int col = (event->x()-basex) / cellSize;
    int row = (event->y()-basey) / cellSize;
    //qDebug() << "点击了坐标：" << event->x() << "," << event->y();
    //qDebug() << "行列：" << row << "," << col;

    if (!isMyTurn) return;

    // 在自己的回合才能下棋
    if (col >= 0 && col < boardSize && row >= 0 && row < boardSize && board[row][col] == 0) {
        // 检查禁手，禁止落子
        //ruleChecker.board = board;
        ruleChecker.chess = chess;
        if (ruleChecker.isForbiddenMove(row, col))
        {
            QMessageBox::information(this, "提示", QString::fromUtf8("You can not put a piece here!"));
            return;
        }
        isMyTurn = false; // 在AI下完棋后置回true
        board[row][col] = chess;  // 示例：黑棋放置
        update();  // 更新界面显示
        int res = ruleChecker.judge();
        if (res)
        {
            winOrLose(res==chess);
            return;
        }

        aiPlay();
    }
}

void GobangBoard::mouseMoveEvent(QMouseEvent *event) {
    //计算判断落子靠近哪个点位，在该点位显示红色框框
    int col = (event->x()-basex) / cellSize;
    int row = (event->y()-basey) / cellSize;

    if (col >= 0 && col < boardSize && row >= 0 && row < boardSize && board[row][col] == 0) {
        hoverPos = QPoint(col, row);
        showHoverPiece = true;
        update();
    } else {
        showHoverPiece = false;
        update();
    }
}

void GobangBoard::aiPlay()
{
    // 使用C++11随机数库生成一个1到2秒之间的随机延迟
    std::random_device rd;
    std::mt19937 gen(rd()); // 使用随机种子生成器
    std::uniform_int_distribution<> dis(0, 1000); // 生成1000到2000毫秒的随机数
    int delay = dis(gen); // 获取随机延迟
    // 使用QTimer单次延迟调用
    QTimer::singleShot(0, [this]() {
        ai.chess=chess;
        //ai.board=board;
        auto bestMove = ai.getBestMove();
        int ai_chess = 3-chess;
        board[bestMove.x()][bestMove.y()] = ai_chess;
        update();
        int res = ruleChecker.judge();
        //qDebug() << "res = " << res;
        if (res)
        {
            winOrLose(res==chess);
            return;
        }
        isMyTurn = true;
    });

}

