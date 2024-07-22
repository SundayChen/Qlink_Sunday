#include "doublelevel.h"

DoubleLevel::DoubleLevel(int w, int h) : Level(w, h)
{
    //初始化私有成员
    selectChange2 = true;
    isSelected2 = isNeared2 = secondSelect2 = false;
    selectX2 = selectY2 = selectXX2 = selectYY2 = 0;
    score2 = 0;

    for (int i = 0; i < 8; ++i) linePoint2[i] = -1;
    matchTime2 = 0;

    //设置中英文字体
    QFont CNfont("SDK_SC_Web", 22);
    QFont ENfont("Bender", 30);

    //初始化三个按钮
    QPushButton* save = new QPushButton(this);
    QPushButton* exit = new QPushButton(this);
    QPushButton* load = new QPushButton(this);

    save->setParent(this);
    save->setText("保存游戏");
    save->move(1600, 110);
    save->resize(300, 100);
    save->setFont(CNfont);
    connect(save, &QPushButton::clicked, [=]()
        {
            if (gameOver) return;
            pause = true;

            QString path = QFileDialog::getSaveFileName(this, "存档位置", "C:", "File(*.bin)");
            QFile file(path);
            if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
                QTextStream out(&file);

                out << "canvas.sjtu.edu.cn" << endl;
                out << remainTime << ' ' << endl;
                out << score << ' ' << score2 << endl;
                out << x << ' ' << y << endl;
                out << x2 << ' ' << y2 << endl;
                out << height << ' ' << width << endl;
                for (int i = 1; i <= height; ++i) {
                    for (int j = 1; j <= width; ++j) out << map[i][j] << ' ';
                    out << endl;
                }

                file.close();
            }
        });

    load->setParent(this);
    load->setText("载入游戏");
    load->move(1600, 220);
    load->resize(300, 100);
    load->setFont(CNfont);
    connect(load, &QPushButton::clicked, [=]()
        {
            if (gameOver) return;
            pause = true;
            emit this->loadGame();
        });

    exit->setParent(this);
    exit->setText("退出游戏");
    exit->move(1600, 330);
    exit->resize(300, 100);
    exit->setFont(CNfont);
    connect(exit, &QPushButton::clicked, [=]()
        {
            pause = true;
            if (QMessageBox::question(this, "退出双人游戏", "确定退出吗？未主动保存的游戏进度将失去！", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
            {
                this->close();
                emit this->exitGame();
            }
        });

    //让键盘的focus不会在按钮上，防止方向键对应到按钮而不是人物移动
    save->setFocusPolicy(Qt::NoFocus);
    load->setFocusPolicy(Qt::NoFocus);
    exit->setFocusPolicy(Qt::NoFocus);

    //设置窗口格式
    setWindowTitle("QLink (Double Play)");

    //设置计时器
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&DoubleLevel::update));
    timer->start(10);

    //时间、暂停、分数与是否能消除的显示
    scoreShow2 = new QLabel(this);
    scoreShow2->setGeometry(QRect(1660, 780, 400, 200));
    scoreShow2->setFont(QFont("Bender", 60));

    isShow2 = new QLabel(this);
    isShow2->setGeometry(QRect(1620, 890, 400, 200));
    isShow2->setFont(QFont("方正特雅宋_GBK", 30));

    //随机生成人物位置
    int randNum2 = qrand() % 4;
    if (randNum2 == 0) {
        x2 = -20;
        y2 = qrand() % 900 - 20;
    }
    else if (randNum2 == 1) {
        x2 = 1280;
        y2 = qrand() % 900 - 20;
    }
    else if (randNum2 == 2) {
        y2 = -20;
        x2 = qrand() % 1300 - 20;
    }
    else if (randNum2 == 3) {
        y2 = 880;
        x2 = qrand() % 1300 - 20;
    }

    //处理按键
    keyTimer = new QTimer(this);
    connect(keyTimer, &QTimer::timeout, this, &DoubleLevel::key);
}

void DoubleLevel::paintEvent(QPaintEvent*)
{
    //画图函数，由于与基类逻辑不同（添加了一个人物），需要另外覆写

    if (remainTime <= 0) gameOver = true;
    if (gameOver) pause = true;  //时间到或无解时强制停止
    timeShow->setNum(remainTime / 1000);  //显示剩余时间
    scoreShow->setNum(score);  //显示分数
    scoreShow2->setNum(score2);  //显示2p分数

    //暂停与游戏结束
    if (pause && !gameOver) {
        switch ((QTime::currentTime().second() % 2 * 1000 + QTime::currentTime().msec()) / 500) {
        case 0:
            pauseShow->setText("暂停中");break;
        case 1:
            pauseShow->setText("暂停中.");break;
        case 2:
            pauseShow->setText("暂停中..");break;
        case 3:
            pauseShow->setText("暂停中...");break;
        };
    }
    else if (gameOver) pauseShow->setText("游戏结束！");
    else {
        remainTime -= 10;
        pauseShow->setText("");
    }

    //绘画人物
    QPainter painter(this);
    QPainter painter2(this);
    painter.drawPixmap(0, 0, 1920, 1080, background);
    QPixmap playerPix, playerPix2;
    playerPix.load(":/picture/Texas.png");
    playerPix2.load(":/picture/Yato.png");
    painter.drawPixmap(x, y, 270, 270, playerPix);
    painter2.drawPixmap(x2, y2, 270, 270, playerPix2);

    //绘画箱子
    QPixmap boxPix1, boxPix2, boxPix3, boxPix4, boxPix5, boxPix6;
    boxPix1.load(":/picture/skd.png");
    boxPix2.load(":/picture/jsd.png");
    boxPix3.load(":/picture/dl.png");
    boxPix4.load(":/picture/ky.png");
    boxPix5.load(":/picture/w.png");
    boxPix6.load(":/picture/hdg.png");
    for (int i = 1; i <= height; ++i) {
        for (int j = 1; j <= width; ++j) {
            if (map[i][j] == 1) painter.drawPixmap(j * 100 + 50, i * 100 + 50, 100, 100, boxPix1);
            if (map[i][j] == 2) painter.drawPixmap(j * 100 + 50, i * 100 + 50, 100, 100, boxPix2);
            if (map[i][j] == 3) painter.drawPixmap(j * 100 + 50, i * 100 + 50, 100, 100, boxPix3);
            if (map[i][j] == 4) painter.drawPixmap(j * 100 + 50, i * 100 + 50, 100, 100, boxPix4);
            if (map[i][j] == 5) painter.drawPixmap(j * 100 + 50, i * 100 + 50, 100, 100, boxPix5);
            if (map[i][j] == 6) painter.drawPixmap(j * 100 + 50, i * 100 + 50, 100, 100, boxPix6);
        }
    }
    //人物实际和方块接触的位置
    int rightX = (x + 90) / 100 - 1, rightY = (y + 60) / 100 - 1, downX = (x + 70) / 100 - 1, downY = (y + 90) / 100 - 1;
    int leftX = (x + 50) / 100 - 1, leftY = (y + 70) / 100 - 1, upX = (x + 70) / 100 - 1, upY = y / 100 - 1;

    int rightX2 = (x2 + 90) / 100 - 1, rightY2 = (y2 + 60) / 100 - 1, downX2 = (x2 + 70) / 100 - 1, downY2 = (y2 + 90) / 100 - 1;
    int leftX2 = (x2 + 50) / 100 - 1, leftY2 = (y2 + 70) / 100 - 1, upX2 = (x2 + 70) / 100 - 1, upY2 = y2 / 100 - 1;

    //画出人物接近方块、选择方块的效果
    isNeared = false;
    if (rightX >= 0 && rightX < width && rightY >= 0 && rightY < height && map[rightY + 1][rightX + 1]) {
        isNeared = true;
        neared(rightX, rightY);
        if (isSelected && selectChange) {
            selectX = rightX;
            selectY = rightY;
            selectChange = false;
        }
        if (!selectChange && secondSelect) {
            selectXX = rightX;
            selectYY = rightY;
        }
    }
    else if (downX >= 0 && downX < width && downY >= 0 && downY < height && map[downY + 1][downX + 1]) {
        isNeared = true;
        neared(downX, downY);
        if (isSelected && selectChange) {
            selectX = downX;
            selectY = downY;
            selectChange = false;
        }
        if (!selectChange && secondSelect) {
            selectXX = downX;
            selectYY = downY;
        }
    }
    else if (leftX >= 0 && leftX < width && leftY >= 0 && leftY < height && map[leftY + 1][leftX + 1]) {
        isNeared = true;
        neared(leftX, leftY);
        if (isSelected && selectChange) {
            selectX = leftX;
            selectY = leftY;
            selectChange = false;
        }
        if (!selectChange && secondSelect) {
            selectXX = leftX;
            selectYY = leftY;
        }
    }
    else if (upX >= 0 && upX < width && upY >= 0 && upY < height && map[upY + 1][upX + 1]) {
        isNeared = true;
        neared(upX, upY);
        if (isSelected && selectChange) {
            selectX = upX;
            selectY = upY;
            selectChange = false;
        }
        if (!selectChange && secondSelect) {
            selectXX = upX;
            selectYY = upY;
        }
    }

    isNeared2 = false;
    if (rightX2 >= 0 && rightX2 < width && rightY2 >= 0 && rightY2 < height && map[rightY2 + 1][rightX2 + 1]) {
        isNeared2 = true;
        neared(rightX2, rightY2);
        if (isSelected2 && selectChange2) {
            selectX2 = rightX2;
            selectY2 = rightY2;
            selectChange2 = false;
        }
        if (!selectChange2 && secondSelect2) {
            selectXX2 = rightX2;
            selectYY2 = rightY2;
        }
    }
    else if (downX2 >= 0 && downX2 < width && downY2 >= 0 && downY2 < height && map[downY2 + 1][downX2 + 1]) {
        isNeared2 = true;
        neared(downX2, downY2);
        if (isSelected2 && selectChange2) {
            selectX2 = downX2;
            selectY2 = downY2;
            selectChange2 = false;
        }
        if (!selectChange2 && secondSelect2) {
            selectXX2 = downX2;
            selectYY2 = downY2;
        }
    }
    else if (leftX2 >= 0 && leftX2 < width && leftY2 >= 0 && leftY2 < height && map[leftY2 + 1][leftX2 + 1]) {
        isNeared2 = true;
        neared(leftX2, leftY2);
        if (isSelected2 && selectChange2) {
            selectX2 = leftX2;
            selectY2 = leftY2;
            selectChange2 = false;
        }
        if (!selectChange2 && secondSelect2) {
            selectXX2 = leftX2;
            selectYY2 = leftY2;
        }
    }
    else if (upX2 >= 0 && upX2 < width && upY2 >= 0 && upY2 < height && map[upY2 + 1][upX2 + 1]) {
        isNeared2 = true;
        neared(upX2, upY2);
        if (isSelected2 && selectChange2) {
            selectX2 = upX2;
            selectY2 = upY2;
            selectChange2 = false;
        }
        if (!selectChange2 && secondSelect2) {
            selectXX2 = upX2;
            selectYY2 = upY2;
        }
    }

    if (isSelected) selected(selectX, selectY);
    if (isSelected2) selected(selectX2, selectY2);

    //消除方块
    if (!selectChange && secondSelect) {
        int turnX, turnY, turnXX, turnYY;
        if (isMatched(selectY + 1, selectX + 1, selectYY + 1, selectXX + 1, turnY, turnX, turnYY, turnXX)) {
            matchTime = remainTime;
            if (map[selectY + 1][selectX + 1] == 3) score += 2;
            else ++score;
            map[selectY + 1][selectX + 1] = 0;
            map[selectYY + 1][selectXX + 1] = 0;
            isShow->setText("消除成功");

            //画线准备
            linePoint[0] = selectY + 1;
            linePoint[1] = selectX + 1;
            linePoint[2] = selectYY + 1;
            linePoint[3] = selectXX + 1;
            linePoint[4] = turnY;
            linePoint[5] = turnX;
            linePoint[6] = turnYY;
            linePoint[7] = turnXX;
        }
        else isShow->setText("不可消除");
        isSelected = false;
        selectChange = true;
        secondSelect = false;
    }
    if (secondSelect && !isSelected) secondSelect = false;

    if (!selectChange2 && secondSelect2) {
        int turnX2, turnY2, turnXX2, turnYY2;
        if (isMatched(selectY2 + 1, selectX2 + 1, selectYY2 + 1, selectXX2 + 1, turnY2, turnX2, turnYY2, turnXX2)) {
            matchTime2 = remainTime;
            if (map[selectY2 + 1][selectX2 + 1] == 3) score2 += 2;
            else ++score2;
            map[selectY2 + 1][selectX2 + 1] = 0;
            map[selectYY2 + 1][selectXX2 + 1] = 0;
            isShow2->setText("消除成功");

            //画线准备
            linePoint2[0] = selectY2 + 1;
            linePoint2[1] = selectX2 + 1;
            linePoint2[2] = selectYY2 + 1;
            linePoint2[3] = selectXX2 + 1;
            linePoint2[4] = turnY2;
            linePoint2[5] = turnX2;
            linePoint2[6] = turnYY2;
            linePoint2[7] = turnXX2;
        }
        else isShow2->setText("不可消除");
        isSelected2 = false;
        selectChange2 = true;
        secondSelect2 = false;
    }
    if (secondSelect2 && !isSelected2) secondSelect2 = false;

    if (matchTime && matchTime - remainTime <= 500) drawLine(linePoint[0], linePoint[1], linePoint[2], linePoint[3], linePoint[4], linePoint[5], linePoint[6], linePoint[7]);
    if (matchTime2 && matchTime2 - remainTime <= 500) drawLine(linePoint2[0], linePoint2[1], linePoint2[2], linePoint2[3], linePoint2[4], linePoint2[5], linePoint2[6], linePoint2[7]);

    //消除后若游戏无解，则游戏结束
    if (!isSolvable()) gameOver = true;

    //结算画面
    if (gameOver && !isSettle) {
        scoreShow->hide();
        isShow->hide();
        scoreWords->hide();
        isSettle = true;
        Settlement *set = new Settlement(score, score2);
        set->show();
        emit this->end();
    }
}

void DoubleLevel::key()
{
    //负责处理由两个KeyEvent函数捕捉的按键信息，由于与基类不同，添加了一个人物，故覆写

    foreach(int key, pressedKeys) {
        if (key == Qt::Key_W && isMovable(1, x, y)) y -= 10;
        if (key == Qt::Key_S && isMovable(2, x, y)) y += 10;
        if (key == Qt::Key_A && isMovable(3, x, y)) x -= 10;
        if (key == Qt::Key_D && isMovable(4, x, y)) x += 10;
        if (key == Qt::Key_J && isNeared) {
            isSelected = true;
            if (!isSelected) selectChange = true;
        }
        if (key == Qt::Key_K) secondSelect = true;

        if (key == Qt::Key_Up && isMovable(1, x2, y2)) y2 -= 10;
        if (key == Qt::Key_Down && isMovable(2, x2, y2)) y2 += 10;
        if (key == Qt::Key_Left && isMovable(3, x2, y2)) x2 -= 10;
        if (key == Qt::Key_Right && isMovable(4, x2, y2)) x2 += 10;
        if (key == Qt::Key_1 && isNeared2) {
            isSelected2 = true;
            if (!isSelected2) selectChange2 = true;
        }
        if (key == Qt::Key_2) secondSelect2 = true;
    }
}

DoubleLevel::~DoubleLevel()
{
    delete scoreShow2;
    delete isShow2;
}
