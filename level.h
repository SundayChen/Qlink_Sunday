#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QSet>
#include <QTimer>
#include <QPainter>
#include <QKeyEvent>
#include <QPushButton>
#include <QMessageBox>
#include <QPixmap>
#include <QTime>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QtDebug>
#include "settlement.h"

class Level : public QMainWindow
{
    Q_OBJECT
public:
    Level(int w, int h, bool is = false);
    ~Level();

    //map（用于储存地图）和isMatched函数（判断消除函数）需要在单元测试中被其他类调用，所以必须设置为public
    int** map;
    bool isMatched(int y1, int x1, int y2, int x2, int& turnY, int& turnX, int& turnYY, int& turnXX);

protected:
    void paintEvent(QPaintEvent*) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void key();
    void neared(int x, int y);
    void selected(int x, int y);
    void drawLine(int y1, int x1, int y2, int x2, int turnY, int turnX, int turnYY, int turnXX);
    bool horizon(int y1, int x1, int y2, int x2);
    bool vertical(int y1, int x1, int y2, int x2);
    bool turnOnce(int y1, int x1, int y2, int x2, int& turnY, int& turnX);
    bool turnTwice(int y1, int x1, int y2, int x2, int& turnY, int& turnX, int& turnYY, int& turnXX);
    bool isSolvable();
    bool isMovable(int keyType, int x, int y);

    bool pause, isSelected, isNeared, selectChange, secondSelect, gameOver, isSettle;
    int remainTime, score;
    int x, y, selectX, selectY, selectXX, selectYY;
    int width, height;
    int matchTime, linePoint[8];
    QLabel* pauseShow, * timeShow, * scoreWords, * isShow, * addTimeShow, * scoreShow;
    QSet<int> pressedKeys;
    QTimer* keyTimer;
    QPen linePen;
    QPushButton* save, * load, * exit;
    QPixmap background, ska1, ska2, ska3, ska4, ska5, ska6;

    void clearBlock(int type, int, int, int, int);
    int silentTime, skadiNum, skadiState, skadiTime;
    bool isW;

    void prop();
    void shuffle();
    void hintFind();
    void hintShow(int y, int x);
    int propX, propY, propType, hintX, hintY, hintXX, hintYY, hintTime, propTime;
    int stopTime;

    bool isCross;

signals:
    //发送信号，分别给主菜单表明即将退出游戏/加载游戏
    void exitGame();
    void loadGame();
    void end();
    void skadiSound();
    void backReach();
};
