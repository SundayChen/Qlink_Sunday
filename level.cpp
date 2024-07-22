#include "level.h"

Level::Level(int w, int h, bool is) : width(w), height(h), isCross(is)
{
    //初始化私有成员
    pause = selectChange = true;
    isSelected = isNeared = secondSelect = gameOver = isSettle = false;
    remainTime = 60100;
    score = 0;
    selectX = selectY = selectXX = selectYY = 0;

    propTime = 100;
    stopTime = 0;

    silentTime = skadiNum = skadiState = skadiTime = 0;
    isW = false;
    background.load(":/picture/Luo.png");
    ska1.load(":/picture/ska1.png");
    ska2.load(":/picture/ska2.png");
    ska3.load(":/picture/ska3.png");
    ska4.load(":/picture/ska4.png");
    ska5.load(":/picture/ska5.png");
    ska6.load(":/picture/ska6.png");

    //设置画线的笔
    linePen.setColor(QColor(0, 255, 127));
    linePen.setStyle(Qt::DotLine);
    linePen.setWidth(10);

    for (int i = 0; i < 8; ++i) linePoint[i] = -1;
    matchTime = 0;

    propType = 0;
    hintX = hintY = hintXX = hintYY = 0;
    hintTime = 0;

    //设置中英文字体
    QFont CNfont("SDK_SC_Web", 22);
    QFont ENfont("Bender", 40);

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

                out << "sjtu.edu.cn" << endl;
                out << remainTime << ' ' << stopTime << ' ' << endl;
                out << score << endl;
                out << x << ' ' << y << endl;
                out << propX << ' ' << propY << ' ' << propType  << ' ' << propTime << endl;
                out << silentTime  << ' ' << skadiNum  << ' ' << skadiState  << ' ' << skadiTime << ' '  << isW << endl;
                out << isCross << endl;
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
            if (QMessageBox::question(this, "退出单人游戏", "确定退出吗？未主动保存的游戏进度将失去！", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
            {
                this->close();
                emit this->exitGame();
            }
        });

    save->setFocusPolicy(Qt::NoFocus);
    load->setFocusPolicy(Qt::NoFocus);
    exit->setFocusPolicy(Qt::NoFocus);

    //设置窗口格式
    setWindowTitle("QLink (Single Play)");
    resize(1920, 1080);
    setWindowIcon(QPixmap(":/picture/Skadi.jpg"));

    //设置计时器
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&Level::update));
    timer->start(10);

    //时间、暂停、分数与是否能消除的显示
    pauseShow = new QLabel(this);
    pauseShow->setGeometry(QRect(1650, 10, 400, 100));
    pauseShow->setFont(CNfont);

    timeShow = new QLabel(this);
    timeShow->setGeometry(QRect(10, 10, 500, 70));
    timeShow->setFont(ENfont);

    addTimeShow = new QLabel(this);
    addTimeShow->setGeometry(QRect(100, 10, 500, 70));
    addTimeShow->setFont(ENfont);

    scoreWords = new QLabel(this);
    scoreWords->setGeometry(QRect(1650, 400, 200, 200));
    scoreWords->setText("分数");
    scoreWords->setFont(QFont("方正特雅宋_GBK", 45));

    scoreShow = new QLabel(this);
    scoreShow->setGeometry(QRect(1660, 520, 400, 200));
    scoreShow->setFont(QFont("Bender", 60));

    isShow = new QLabel(this);
    isShow->setGeometry(QRect(1620, 650, 400, 200));
    isShow->setFont(QFont("方正特雅宋_GBK", 30));

    //随机数生成准备
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    //随机人物生成位置
    int randNum = qrand() % 4;
    if (randNum == 0) {
        x = -20;
        y = qrand() % 900 - 20;
    }
    else if (randNum == 1) {
        x = 1280;
        y = qrand() % 900 - 20;
    }
    else if (randNum == 2) {
        y = -20;
        x = qrand() % 1300 - 20;
    }
    else if (randNum == 3) {
        y = 880;
        x = qrand() % 1300 - 20;
    }

    //随机生成地图
    map = new int* [height + 2];
    for (int i = 0; i <= height + 1; ++i) map[i] = new int[width + 2];

    for (int i = 0; i <= width + 1; ++i) map[0][i] = map[height + 1][i] = 0;
    for (int i = 0; i <= height + 1; ++i) map[i][0] = map[i][width + 1] = 0;

    for (int i = 1; i <= height; ++i) {
        for (int j = 1; j <= width; ++j) map[i][j] = qrand() % 6 + 1; //1,2,3,4,5,6分别代表不同种的箱子，0代表箱子已经被消除
    }
    if (!isSolvable()) gameOver = true;

    //处理按键
    keyTimer = new QTimer(this);
    connect(keyTimer, &QTimer::timeout, this, &Level::key);
}

void Level::paintEvent(QPaintEvent*)
{
    //每10ms就刷新当前显示，此为主要的画图函数

    if (remainTime - stopTime <= 0) gameOver = true;
    if (gameOver) pause = true;  //时间到或无解时强制停止
    timeShow->setNum((remainTime - stopTime) / 1000);  //显示剩余时间
    scoreShow->setNum(score);  //显示分数

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
        propTime -= 10;
        pauseShow->setText("");
    }

    //绘画人物
    QPainter painter(this);
    painter.drawPixmap(0, 0, 1920, 1080, background);
    QPixmap playerPix;
    playerPix.load(":/picture/Texas.png");
    painter.drawPixmap(x, y, 270, 270, playerPix);

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

    prop();

    //人物实际和方块接触的位置
//    painter.drawRect(QRectF(x+180,y+110,10,10)); //右边的点
//    painter.drawRect(QRectF(x+120,y+180,10,10)); //下边的点
//    painter.drawRect(QRectF(x+50,y+120,10,10)); //左边的点
//    painter.drawRect(QRectF(x+120,y+40,10,10)); //上边的点

    //画出人物接近方块、选择方块的效果
    int rightX = (x + 90) / 100 - 1, rightY = (y + 60) / 100 - 1, downX = (x + 70) / 100 - 1, downY = (y + 90) / 100 - 1;
    int leftX = (x + 50) / 100 - 1, leftY = (y + 70) / 100 - 1, upX = (x + 70) / 100 - 1, upY = y / 100 - 1;

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

    if (isSelected) selected(selectX, selectY);

    //消除方块
    if (!selectChange && secondSelect) {
        int turnX, turnY, turnXX, turnYY;
        if (isMatched(selectY + 1, selectX + 1, selectYY + 1, selectXX + 1, turnY, turnX, turnYY, turnXX)) {
            matchTime = remainTime;
            clearBlock(map[selectY + 1][selectX + 1], selectY, selectX, selectYY, selectXX);
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

    if (matchTime && matchTime - remainTime <= 500) drawLine(linePoint[0], linePoint[1], linePoint[2], linePoint[3], linePoint[4], linePoint[5], linePoint[6], linePoint[7]);

    if (!isSolvable()) gameOver = true;

    if (skadiState == 1 && !gameOver && skadiTime - remainTime >= 10000) {
        skadiState = 2;
        emit this->backReach();
    }

    //结算画面
    if (gameOver && !isSettle) {
        scoreShow->hide();
        isShow->hide();
        scoreWords->hide();
        isSettle = true;
        Settlement *set = new Settlement(score, -1);
        set->show();
        emit this->end();
    }

    //斯卡蒂进度条
    QPainter skapainter(this);
    if (skadiNum >= 1) skapainter.drawPixmap(1500, 680, 300, 300, ska1);
    if (skadiNum >= 2) skapainter.drawPixmap(1600, 680, 300, 300, ska2);
    if (skadiNum >= 3) skapainter.drawPixmap(1700, 680, 300, 300, ska3);
    if (skadiNum >= 4) skapainter.drawPixmap(1500, 830, 300, 300, ska4);
    if (skadiNum >= 5) skapainter.drawPixmap(1620, 830, 300, 300, ska5);
    if (skadiNum >= 6) skapainter.drawPixmap(1700, 830, 300, 300, ska6);
}

void Level::clearBlock(int type,int selectY, int selectX, int selectYY, int selectXX)
{
    //此函数执行方块消除后的特殊效果

    switch (type) {
    case 1: ++score;
        if (skadiState == 1) ++score;
        ++skadiNum;
        if (skadiState) break;
        if (skadiNum == 6) {
            emit this->skadiSound();
            skadiState = 1;
            skadiTime = remainTime;
        } break;
    case 2: score += 3;
        if (skadiState == 1) score += 3;
        addTimeShow->setText("-2s");
        QTimer::singleShot(550, this, [=]()
            {
                addTimeShow->setText("");
                stopTime += 2000;
            });
        break;
    case 3: score += 2;
        if (skadiState == 1) score += 2;
        break;
    case 4: ++score;
        if (skadiState == 1) ++score;
        propTime -= 3000; break;
    case 5: ++score;
        if (skadiState == 1) ++score;
        if (isW) break;
        QTimer::singleShot(1000, this, [=]()
        {
            for (int i = 0; i <= 2; ++i) {
                for (int j = 0; j <= 2; ++j) {
                    if (i == 1 && j == 1) continue;
                    if (map[selectY + i][selectX + j]) {
                        ++score;
                        if (skadiState == 1) ++score;
                        map[selectY + i][selectX + j] = 0;
                    }
                    if (map[selectYY + i][selectXX + j]) {
                        ++score;
                        if (skadiState == 1) ++score;
                        map[selectYY + i][selectXX + j] = 0;
                    }
                }
            }
            addTimeShow->setText("-5s");
            QTimer::singleShot(550, this, [=]()
                {
                    addTimeShow->setText("");
                    stopTime += 5000;
                });
        });
        isW = true; break;
    case 6: ++score;
        if (skadiState == 1) ++score;
        if (silentTime && silentTime - remainTime <= 10000) break;
        silentTime = remainTime;
        score += 2;
        if (skadiState == 1) score += 2;
        addTimeShow->setText("+2s");
        QTimer::singleShot(550, this, [=]()
            {
                addTimeShow->setText("");
                stopTime -= 2000;
            });
        break;
    }
}

void Level::prop()
{
    //该函数执行与道具有关的效果

    QPainter painter(this);
    //产生道具
    if (propTime <= 0 && !propType) {
        propTime = 20000;
        propType = qrand() % 4 + 1;  //0代表场上不存在道具，1 2 3 4分别代表+1 Shuffle Hint Add
        int randNum = qrand() % 4;
        if (randNum == 0) {
            propX = 50;
            propY = qrand() % 900 + 50;
        }
        else if (randNum == 1) {
            propX = 1370;
            propY = qrand() % 900 + 50;
        }
        else if (randNum == 2) {
            propY = 50;
            propX = qrand() % 1300 + 50;
        }
        else if (randNum == 3) {
            propY = 970;
            propX = qrand() % 1300 + 50;
        }
    }

    //画出道具
    QPixmap propPix1, propPix2, propPix3, propPix4;
    propPix1.load(":/picture/+1.png");
    propPix2.load(":/picture/Shuffle.png");
    propPix3.load(":/picture/Hint.png");
    propPix4.load(":/picture/Add.png");
    if (propType) {
        switch (propType) {
        case 1:
            painter.drawPixmap(propX, propY, 90, 90, propPix1);
            break;
        case 2:
            painter.drawPixmap(propX, propY, 90, 90, propPix2);
            break;
        case 3:
            painter.drawPixmap(propX, propY, 90, 90, propPix3);
            break;
        case 4:
            painter.drawPixmap(propX, propY, 90, 90, propPix4);
            break;
        }
    }

    //检测人物是否触碰道具
    if ((x + 180 > propX && x + 180 < propX + 90 && y + 110 > propY && y + 110 < propY + 90)
        || (x + 120 > propX && x + 120 < propX + 90 && y + 180 > propY && y + 180 < propY + 90)
        || (x + 50 > propX && x + 50 < propX + 90 && y + 120 > propY && y + 120 < propY + 90)
        || (x + 120 > propX && x + 120 < propX + 90 && y + 40 > propY && y + 40 < propY + 90)) {
        if (propType == 1) {
            addTimeShow->setText("+15s");
            QTimer::singleShot(550, this, [=]()
                {
                    addTimeShow->setText("");
                    stopTime -= 15000;
                });
        }
        if (propType == 2) shuffle();
        if (propType == 3) hintTime = remainTime;
        if (propType == 4) score += score % 10 + score / 10 % 10;
        propType = 0;
    }

    //Hint处理
    if (hintTime - remainTime <= 10000 && hintTime) {
        if (!map[hintY][hintX] || !map[hintYY][hintXX]) hintFind();
        hintShow(hintY, hintX);
        hintShow(hintYY, hintXX);
    }
}

void Level::keyPressEvent(QKeyEvent* event)
{
    //监测按下键盘的操作，并将操作压入容器中
    if (gameOver) return;
    if (event->key() == Qt::Key_P) pause = 1 - pause;
    if (!pause) {
        if (!event->isAutoRepeat()) pressedKeys.insert(event->key());
        if (!keyTimer->isActive()) keyTimer->start(15);
    }
}

void Level::keyReleaseEvent(QKeyEvent* event)
{
    //监测释放键盘的操作，并将操作从容器中取出
    if (gameOver) return;
    if (!pause) {
        if (!event->isAutoRepeat()) pressedKeys.remove(event->key());
        if (pressedKeys.isEmpty()) keyTimer->stop();
    }
}

void Level::key()
{
    //处理容器中键盘操作，使得人物移动、方块选中等
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
    }
}

bool Level::isMovable(int keyType, int x, int y)
{
    //检测是否可以移动，实现边界以及碰撞体积，参数1 2 3 4分别对应W S A D
    int xx, yy;
    if (keyType == 1) {
        if (y < -30) return false;
        if (isCross) return true;
        xx = (x + 70) / 100;
        yy = (y + 30) / 100;
        if (xx >= 0 && xx <= width && yy >= 0 && yy <= height && map[yy][xx]) return false;
        return true;
    }
    else if (keyType == 2) {
        if (y > 900) return false;
        if (isCross) return true;
        xx = (x + 70) / 100;
        yy = (y + 80) / 100;
        if (xx >= 0 && xx <= width && yy >= 0 && yy <= height && map[yy][xx]) return false;
        return true;
    }
    else if (keyType == 3) {
        if (x < -40) return false;
        if (isCross) return true;
        xx = (x + 60) / 100;
        yy = (y + 70) / 100;
        if (xx >= 0 && xx <= width && yy >= 0 && yy <= height && map[yy][xx]) return false;
        return true;
    }
    else if (keyType == 4) {
        if (x > 1320) return false;
        if (isCross) return true;
        xx = (x + 90) / 100;
        yy = (y + 60) / 100;
        if (xx >= 0 && xx <= width && yy >= 0 && yy <= height && map[yy][xx]) return false;
        return true;
    }
    else return false;
}

void Level::neared(int x, int y)
{
    //绘画人物接近方块时，方块的状态（蓝色边框）
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 255, 255));
    painter.save();
    painter.drawRect(QRectF(x * 100 + 150, y * 100 + 150, 100, 10));
    painter.drawRect(QRectF(x * 100 + 150, y * 100 + 150, 10, 100));
    painter.drawRect(QRectF(x * 100 + 150, y * 100 + 240, 100, 10));
    painter.drawRect(QRectF(x * 100 + 240, y * 100 + 150, 10, 100));
    painter.restore();
}

void Level::selected(int x, int y)
{
    //绘画方块被选中时，方块的状态（红色边框）
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255, 0, 0));
    painter.save();
    painter.drawRect(QRectF(x * 100 + 150, y * 100 + 150, 100, 10));
    painter.drawRect(QRectF(x * 100 + 150, y * 100 + 150, 10, 100));
    painter.drawRect(QRectF(x * 100 + 150, y * 100 + 240, 100, 10));
    painter.drawRect(QRectF(x * 100 + 240, y * 100 + 150, 10, 100));
    painter.restore();
}

bool Level::vertical(int y1, int x1, int y2, int x2)
{
    //方块是否可以通过竖直直接连接，从而被消除
    if (x1 != x2) return false;
    int startY = y1 > y2 ? y2 : y1;
    int endY = y1 > y2 ? y1 : y2;
    for (int i = startY + 1; i < endY; ++i) {
        if (map[i][x1]) return false;
    }
    return true;
}

bool Level::horizon(int y1, int x1, int y2, int x2)
{
    //方块是否可以通过水平直接连接，从而被消除
    if (y1 != y2) return false;
    int startX = x1 > x2 ? x2 : x1;
    int endX = x1 > x2 ? x1 : x2;
    for (int i = startX + 1; i < endX; ++i) {
        if (map[y1][i]) return false;
    }
    return true;
}

bool Level::turnOnce(int y1, int x1, int y2, int x2, int& turnY, int& turnX)
{
    //方块是否可以通过拐弯一次的折线连接，从而被消除
    //返回引用的是拐角的坐标
    int c_y = y2, c_x = x1;
    if (!map[c_y][c_x]) {
        if (vertical(y1, x1, c_y, c_x) && horizon(c_y, c_x, y2, x2)) {
            turnY = c_y;
            turnX = c_x;
            return true;
        }
    }
    int d_y = y1, d_x = x2;
    if (!map[d_y][d_x]) {
        if (horizon(y1, x1, d_y, d_x) && vertical(d_y, d_x, y2, x2)) {
            turnY = d_y;
            turnX = d_x;
            return true;
        }
    }
    return false;
}

bool Level::turnTwice(int y1, int x1, int y2, int x2, int& turnY, int& turnX, int& turnYY, int& turnXX)
{
    //方块是否可以通过拐弯两次的折线连接，从而被消除
    //返回引用的是两个拐角的坐标
    for (int i = 0; i <= height + 1; ++i) {
        for (int j = 0; j <= width + 1; ++j) {
            if (i != y1 && i != y2 && j != x1 && j != x2) continue;
            if ((i == y1 && j == x1) || (i == y2 && j == x2)) continue;
            if (map[i][j]) continue;
            if (turnOnce(y1, x1, i, j, turnY, turnX) && (horizon(i, j, y2, x2) || vertical(i, j, y2, x2))) {
                turnYY = i;
                turnXX = j;
                return true;
            }
            if (turnOnce(i, j, y2, x2, turnYY, turnXX) && (horizon(y1, x1, i, j) || vertical(y1, x1, i, j))) {
                turnY = i;
                turnX = j;
                return true;
            }
        }
    }
    return false;
}

bool Level::isMatched(int y1, int x1, int y2, int x2, int& turnY, int& turnX, int& turnYY, int& turnXX)
{
    //综合判断两个方块是否可以被消除
    //返回引用的是至多两个拐角的坐标，如果实际拐角数量小于2，则多余的拐角为默认值（-1）
    if (y1 <= 0 || y1 > height || y2 <= 0 || y2 > height) return false;
    if (x1 <= 0 || x1 > width || x2 <= 0 || x2 > width) return false;
    if (!map[y1][x1] || !map[y2][x2]) return false;
    if (map[y1][x1] != map[y2][x2] || (x1 == x2 && y1 == y2)) return false;
    turnY = turnX = turnYY = turnXX = -1;
    if (horizon(y1, x1, y2, x2) || vertical(y1, x1, y2, x2)) return true;
    if (turnOnce(y1, x1, y2, x2, turnY, turnX)) return true;
    if (turnTwice(y1, x1, y2, x2, turnY, turnX, turnYY, turnXX)) return true;
    return false;
}

bool Level::isSolvable()
{
    //检测当前地图是否有解
    int a, b, c, d;
    for (int i = 1; i <= height; ++i) {
        for (int j = 1; j <= width; ++j) {
            if (!map[i][j]) continue;
            for (int ii = 1; ii <= height; ++ii) {
                for (int jj = 1; jj <= width; ++jj) {
                    if (!map[ii][jj]) continue;
                    if (isMatched(i, j, ii, jj, a, b, c, d)) return true;
                }
            }
        }
    }
    return false;
}

void Level::drawLine(int y1, int x1, int y2, int x2, int turnY, int turnX, int turnYY, int turnXX)
{
    //绘画两个被消除方块的连线
    QPainter linePainter(this);
    linePainter.setPen(linePen);
    linePainter.save();
    if (turnY < 0 && turnYY < 0) {
        linePainter.drawLine(x1 * 100 + 100, y1 * 100 + 100, x2 * 100 + 100, y2 * 100 + 100);
        linePainter.restore();
        return;
    }
    else if (turnY > 0 && turnYY < 0) {
        linePainter.drawLine(x1 * 100 + 100, y1 * 100 + 100, turnX * 100 + 100, turnY * 100 + 100);
        linePainter.drawLine(turnX * 100 + 100, turnY * 100 + 100, x2 * 100 + 100, y2 * 100 + 100);
        linePainter.restore();
        return;
    }
    else {
        linePainter.drawLine(x1 * 100 + 100, y1 * 100 + 100, turnX * 100 + 100, turnY * 100 + 100);
        linePainter.drawLine(turnX * 100 + 100, turnY * 100 + 100, turnXX * 100 + 100, turnYY * 100 + 100);
        linePainter.drawLine(turnXX * 100 + 100, turnYY * 100 + 100, x2 * 100 + 100, y2 * 100 + 100);
        linePainter.restore();
        return;
    }
}

void Level::shuffle()
{
    //刷新地图的道具
    for (int i = 1; i <= height; ++i) {
        for (int j = 1; j <= width; ++j) {
            if (map[i][j]) map[i][j] = qrand() % 6 + 1;
        }
    }
    if (!isSolvable()) {
        gameOver = true;
        isShow->setText("游戏无解");
    }
}

void Level::hintFind()
{
    //为了hint道具，找到两个可以被消除的方块
    int a, b, c, d;
    int i, j;
    do {
        i = qrand() % height + 1;
        j = qrand() % width + 1;
    } while (!map[i][j]);

    int first = qrand() % height + 1;
    for (int ii = first; ii < first + height; ++ii) {
        for (int jj = 1; jj <= width; ++jj) {
            if (!map[ii % height + 1][jj]) continue;
            if (isMatched(i, j, ii % height + 1, jj, a, b, c, d)) {
                hintY = i;
                hintX = j;
                hintYY = ii % height + 1;
                hintXX = jj;
                return;
            }
        }
    }
}

void Level::hintShow(int y, int x)
{
    //hint道具生效时，被提示方块的特效，表现为被紫色圆框住
    QPen hintPen;
    QPainter painter(this);
    hintPen.setColor(QColor(160, 32, 240));
    hintPen.setWidth(8);
    painter.setPen(hintPen);
    painter.save();
    painter.drawEllipse(QPoint(x * 100 + 100, y * 100 + 100), 50, 50);
    painter.restore();
}

Level::~Level()
{
    //析构函数，释放动态变量
    delete timeShow;
    delete pauseShow;
    delete scoreShow;
    delete isShow;
    delete addTimeShow;
    delete save;
    delete load;
    delete exit;
    delete keyTimer;
    for (int i = 0; i < height; ++i) delete[] map[i];
    delete[] map;
}
