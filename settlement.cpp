#include "settlement.h"
#include <QPainter>
#include <QTimer>

Settlement::Settlement(int s,int s2):score(s), score2(s2)
{
    //窗口设置
    if (score2 == -1) setWindowTitle("游戏结束！你的分数是：" + QString::number(score));
    else if (score > score2) setWindowTitle("游戏结束！P1胜利！");
    else if (score < score2) setWindowTitle("游戏结束！P2胜利！");
    else if (score == score2) setWindowTitle("游戏结束！平局！");
    resize(1100, 850);
    setWindowIcon(QPixmap(":/picture/Skadi.jpg"));

    isDraw = false;

    //图片加载
    surtr.load(":/picture/Surtr.jpg");
    if (score >= 80) maj.load(":/picture/6001.png");
    if (score > 40 && score < 80) maj.load(":/picture/6007.png");
    if (score <= 40) maj.load(":/picture/6009.png");

    //label初始化
    number.setParent(this);
    number.setGeometry(740, 200, 400, 200);
    number.setFont(QFont("Bender", 70));
    number2.setParent(this);
    number2.setGeometry(740, 400, 400, 200);
    number2.setFont(QFont("Bender", 70));
    words.setParent(this);
    words.setGeometry(650, 0, 450, 200);
    words.setFont(QFont("方正特雅宋_GBK", 45));
    words.setText("最终分数");
    compare.setParent(this);
    compare.setGeometry(650, 600, 450, 200);
    compare.setFont(QFont("Bender", 40));

    //设置定时器
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&Settlement::update));
    timer->start(100);
}

void Settlement::paintEvent(QPaintEvent *)
{
    //画出图像，并有一定时延

    QPainter painter(this);
    painter.drawPixmap(0, 0, 600, 850, surtr);
    QTimer::singleShot(1000, this, [=]()
    {
        number.setNum(score);
        if (score2 >= 0) number2.setNum(score2);
    });
    QTimer::singleShot(2000, this, [=]()
    {
        if (score2 >= 0) {
            if (score > score2) compare.setText("   P1 Wins!");
            else if (score < score2) compare.setText("   P2 Wins!");
            else compare.setText("Draw Game!");
            return;
        }
        isDraw = true;
    });
    if (isDraw) {
        QPainter drawMaj(this);
        drawMaj.drawPixmap(790, 520, 120, 120, maj);
    }
}
