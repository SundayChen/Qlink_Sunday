#include "mainwindow.h"
#include "load.h"
#include "doubleload.h"
#include <QInputDialog>
#include <QtFontDatabaseSupport/QtFontDatabaseSupport>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    //导入字体文件
    QFontDatabase::addApplicationFont(QStringLiteral(":/font/fztys.ttf"));
    QFontDatabase::addApplicationFont(QStringLiteral(":/font/bender.ttf"));
    QFontDatabase::addApplicationFont(QStringLiteral(":/font/genshin.ttf"));

    QFont CNfont("方正特雅宋_GBK", 30);

    //主菜单音乐
    musicPlayer = new QMediaPlayer(this);
    musicList = new QMediaPlaylist(this);
    musicList->addMedia(QUrl("qrc:/sound/Spring.mp3"));
    musicList->addMedia(QUrl("qrc:/sound/Summer.mp3"));
    musicList->addMedia(QUrl("qrc:/sound/Autumn.mp3"));
    musicList->addMedia(QUrl("qrc:/sound/Winter.mp3"));
    musicList->setPlaybackMode(QMediaPlaylist::Random);
    musicList->shuffle();
    musicPlayer->setPlaylist(musicList);
    musicPlayer->play();

    //游戏内音乐
    reachPlayer = new QMediaPlayer(this);
    reachList = new QMediaPlaylist(this);
    reachList->addMedia(QUrl("qrc:/sound/Reach (1).mp3"));
    reachList->addMedia(QUrl("qrc:/sound/Reach (2).mp3"));
    reachList->addMedia(QUrl("qrc:/sound/Reach (3).mp3"));
    reachList->addMedia(QUrl("qrc:/sound/Reach (4).mp3"));
    reachList->addMedia(QUrl("qrc:/sound/Reach (5).mp3"));
    reachList->addMedia(QUrl("qrc:/sound/Reach (6).mp3"));
    reachList->addMedia(QUrl("qrc:/sound/Reach (7).mp3"));
    reachList->setPlaybackMode(QMediaPlaylist::Random);
    reachList->shuffle();
    reachPlayer->setPlaylist(reachList);

    //游戏结束音乐
    endPlayer = new QMediaPlayer(this);
    endPlayer->setMedia(QUrl("qrc:/sound/Ending.mp3"));
    endPlayer->setVolume(35);

    //斯卡蒂音效
    skadiPlayer = new QMediaPlayer(this);
    skadiList = new QMediaPlaylist(this);
    skadiList->addMedia(QUrl("qrc:/sound/SkadiSea.mp3"));
    skadiPlayer->setPlaylist(skadiList);

    //设置窗口格式
    setWindowTitle("QLink MainWindow (Developed by Sunday)");
    resize(1920, 1080);
    setWindowIcon(QPixmap(":/picture/Skadi.jpg"));

    //定义主页面五个按钮
    QPushButton* singlePlay = new QPushButton(this);
    QPushButton* doublePlay = new QPushButton(this);
    QPushButton* load = new QPushButton(this);
    QPushButton* exit = new QPushButton(this);
    QPushButton* instructions = new QPushButton(this);

    QString styleString = "background-color: rgb(200, 200, 200)";
    //具体设置五个按钮的功能
    singlePlay->setParent(this);
    singlePlay->setStyleSheet(styleString);
    singlePlay->setIcon(QPixmap(":/picture/Rock1.png"));
    singlePlay->setIconSize(QSize(120,120));
    singlePlay->setText("单人模式");
    singlePlay->move(1200, 120);
    singlePlay->resize(500, 150);
    singlePlay->setFont(CNfont);
    connect(singlePlay, &QPushButton::clicked, this, &MainWindow::singleGame);

    doublePlay->setParent(this);
    doublePlay->setStyleSheet(styleString);
    doublePlay->setIcon(QPixmap(":/picture/Rock2.png"));
    doublePlay->setIconSize(QSize(120,120));
    doublePlay->setText("双人模式");
    doublePlay->move(1200, 360);
    doublePlay->resize(500, 150);
    doublePlay->setFont(CNfont);
    connect(doublePlay, &QPushButton::clicked, this, &MainWindow::doubleGame);

    load->setParent(this);
    load->setStyleSheet(styleString);
    load->setIcon(QPixmap(":/picture/Rock3.png"));
    load->setIconSize(QSize(120,120));
    load->setText("载入游戏");
    load->move(1200, 600);
    load->resize(500, 150);
    load->setFont(CNfont);
    connect(load, &QPushButton::clicked, this, &MainWindow::loadGame);

    exit->setParent(this);
    exit->setStyleSheet(styleString);
    exit->setIcon(QPixmap(":/picture/Rock4.png"));
    exit->setIconSize(QSize(120,120));
    exit->setText("退出游戏");
    exit->move(1200, 840);
    exit->resize(500, 150);
    exit->setFont(CNfont);
    connect(exit, &QPushButton::clicked, [=]()
        {
            if (QMessageBox::question(this, "退出QLink", "确认退出QLink吗？QAQ", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
                MainWindow::close();
        });

    instructions->setParent(this);
    instructions->setStyleSheet(styleString);
    instructions->setText("帮助");
    instructions->move(10, 960);
    instructions->resize(200, 100);
    instructions->setFont(CNfont);
    connect(instructions, &QPushButton::clicked, [=]()
        {
            QMessageBox::information(this, "游戏帮助", "本游戏使用RPG模式进行，你需要控制角色在地图上移动，选定能够通过两次以内的折线进行连接的相同方块，以消除它们\n\n"
                "倒计时结束或者没有可消除的方块对（包括全部消除）时游戏结束\n\n"
                "单人模式下支持道具、方块有特殊效果，可以通过捡起地图上的不同道具丰富游戏体验\n\n"
                "游戏暂停时可以进行存档。存档可以将游戏存档保存至本地，主菜单页面可读档进行游戏\n\n"
                "游戏分别通过W S A D控制人物上下左右移动，游戏过程中按P键暂停/继续。游戏开始时默认暂停\n\n"
                "靠近想消除的第一个方块后，按J键选中，第二个方块按K选中\n\n"
                "对于双人游戏，采用方向键控制人物移动，1 2键分别对应选择第一、第二个方块。双人模式下，多利为2分，其他方块为1分\n\n"
                "单人游戏可以在游戏开始前选择是否有碰撞体积，双人游戏有碰撞体积");
            QMessageBox::information(this, "方块与道具帮助", "注：下文中的所有特殊效果均只针对单人模式生效\n\n"
                                                      "道具刷新间隔为20s。若即将刷新时未拾起，则会在拾起后再刷新、开始冷却\n\n"
                                                      "赞：+15s剩余时间\n"
                                                      "绝好调：直接加分，数值为当前分数的个位数与十位数之和\n"
                                                      "犀利：随机选择一对可以被消除的方块，并圈起\n"
                                                      "掀桌：重新刷新所有未消除的方块\n\n"
                                                      "多利：消除一对后额外+1分\n"
                                                      "杰斯顿：消除一对后额外+2分，并-2s剩余时间\n"
                                                      "傀影：消除一对后，道具冷却时间-3s\n"
                                                      "“复仇者”：消除一对后额外+2分，+2s剩余时间，接下来10s内无法再触发此特效\n"
                                                      "伊莎玛拉：每局游戏限一次，累计消除6对后触发大群呼唤，10s内消除方块的得分翻倍\n"
                                                      "W：每局游戏限一次，消除一对后将-5s剩余时间，并引发两个3*3的爆炸，爆炸的方块不会触发额外效果，但是每个方块+1分");
        });
}

MainWindow::~MainWindow()
{
    delete musicList;
    delete musicPlayer;
    delete reachList;
    delete reachPlayer;
    delete skadiList;
    delete skadiPlayer;
    delete endPlayer;
}

void MainWindow::paintEvent(QPaintEvent*)
{
    //绘制主菜单
    QPainter painter(this);
    QPixmap pix;
    //painter.drawPixmap(0, 0, 1920, 1080, back);
    pix.load(":/picture/scene.png");
    painter.drawPixmap(0, 0, 1920, 1080, pix);
//    pix.load(":/picture/TexasImage.png");
//    painter.drawPixmap(-130, 100, 1080, 1080, pix);
    pix.load(":/picture/Texas2.png");
    painter.drawPixmap(-550, -600, 2300, 2300, pix);
}

void MainWindow::singleGame()
{
    //当用户选择单人游戏时，执行单人游戏

    int width, height;

    //要求用户输入宽、高
    bool ok;
    width = QInputDialog::getInt(this, "输入宽度", "请输入地图的宽度", 12, 1, 12, 1, &ok);
    if (!ok) return;
    height = QInputDialog::getInt(this, "输入高度", "请输入地图的高度", 8, 1, 8, 1, &ok);
    if (!ok) return;
    ok = false;
    if (QMessageBox::question(this, "碰撞体积", "游戏是否有碰撞体积？", QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
    {
        ok = true;
    }

    //初始化游戏界面
    Level* gaming = new Level(width, height, ok);
    musicPlayer->stop();
    this->close();
    gaming->show();
    reachList->next();
    reachPlayer->play();

    //监视游戏退出，并重新显示主界面
    connect(gaming, &Level::exitGame, [=]()
        {
            reachPlayer->stop();
            endPlayer->stop();
            skadiList->setCurrentIndex(0);
            skadiPlayer->stop();
            gaming->close();
            this->show();
            musicList->next();
            musicPlayer->play();
        });

    //监视载入游戏，此时关闭上一个游戏
    connect(gaming, &Level::loadGame, [=]()
        {
            QString path;
            int mode;
            if (load(path, mode)) {
                skadiList->setCurrentIndex(0);
                skadiPlayer->stop();
                reachPlayer->stop();
                gaming->close();
                gamingLoad(path, mode);
            }
            else return;
        });

    //监视游戏结束，此时播放结束音乐
    connect(gaming, &Level::end, [=]()
    {
        skadiList->setCurrentIndex(0);
        skadiPlayer->stop();
        reachPlayer->stop();
        endPlayer->play();
    });

    //监视斯卡蒂音效是否需要启动
    connect(gaming, &Level::skadiSound, [=]()
    {
        reachPlayer->pause();
        skadiPlayer->play();
    });

    //监视是否需要回到原本的音乐
    connect(gaming, &Level::backReach, [=]()
    {
        skadiList->setCurrentIndex(0);
        skadiPlayer->stop();
        reachPlayer->play();
    });
}

void MainWindow::doubleGame()
{
    //当用户选择双人游戏时，执行双人游戏

    int width, height;

    //要求用户输入宽、高
    bool ok;
    width = QInputDialog::getInt(this, "输入宽度", "请输入地图的宽度", 12, 1, 12, 1, &ok);
    if (!ok) return;
    height = QInputDialog::getInt(this, "输入高度", "请输入地图的高度", 8, 1, 8, 1, &ok);
    if (!ok) return;

    //初始化游戏界面
    DoubleLevel* gaming = new DoubleLevel(width, height);
    musicPlayer->stop();
    this->close();
    gaming->show();
    reachList->next();
    reachPlayer->play();

    //监视游戏退出，并重新显示主界面
    connect(gaming, &DoubleLevel::exitGame, [=]()
        {
            gaming->close();
            reachPlayer->stop();
            endPlayer->stop();
            this->show();
            musicList->next();
            musicPlayer->play();
        });

    //监视载入游戏，此时关闭上一个游戏
    connect(gaming, &DoubleLevel::loadGame, [=]()
        {
            QString path;
            int mode;
            if (load(path, mode)) {
                gaming->close();
                reachPlayer->stop();
                gamingLoad(path, mode);
            }
            else return;
        });

    //监视游戏结束，此时播放结束音乐
    connect(gaming, &DoubleLevel::end, [=]()
    {
        reachPlayer->stop();
        endPlayer->play();
    });
}

void MainWindow::loadGame()
{
    //当用户选择载入游戏时，先读取存档文件，再载入游戏存档

    QString path;
    int mode;
    if (load(path, mode)) gamingLoad(path, mode);
    else return;
}

bool MainWindow::load(QString& path, int& mode)
{
    //当游戏中途或主界面载入游戏时，先预读取存档文件的第一行，判断存档类型和是否存档成功
    //返回值表示读取存档是否成功
    //path值返回了读取文件的路径，mode返回了存档文件的类型（1代表单人，2代表双人）

    path = QFileDialog::getOpenFileName(this, "存档位置");
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray array = file.readLine();
    file.close();
    if (array == "sjtu.edu.cn\n") {
        mode = 1;
        return true;
    }
    else if (array == "canvas.sjtu.edu.cn\n") {
        mode = 2;
        return true;
    }
    else if (path == "") return false;
    else {
        QMessageBox::critical(this, "存档错误", "请选择正确格式的存档！");
        return false;
    }
}

void MainWindow::gamingLoad(const QString& path, int mode)
{
    //正式加载游戏，通过load函数的信息，判断实例化哪一个类

    if (mode == 1) {
        Load* gaming = new Load(path);
        musicPlayer->stop();
        this->close();
        gaming->show();
        reachList->next();
        reachPlayer->play();

        connect(gaming, &Load::exitGame, [=]()
            {
                gaming->close();
                reachPlayer->stop();
                endPlayer->stop();
                skadiList->setCurrentIndex(0);
                skadiPlayer->stop();
                this->show();
                musicList->next();
                musicPlayer->play();
            });

        //监视载入游戏，此时关闭上一个游戏
        connect(gaming, &Load::loadGame, [=]()
            {
                QString path;
                int mode;
                if (load(path, mode)) {
                    reachPlayer->stop();
                    skadiList->setCurrentIndex(0);
                    skadiPlayer->stop();
                    gaming->close();
                    gamingLoad(path, mode);
                }
                else return;
            });

        //监视游戏结束，此时播放结束音乐
        connect(gaming, &Load::end, [=]()
        {
            skadiList->setCurrentIndex(0);
            skadiPlayer->stop();
            reachPlayer->stop();
            endPlayer->play();
        });

        //监视斯卡蒂音效是否需要启动
        connect(gaming, &Load::skadiSound, [=]()
        {
            reachPlayer->pause();
            skadiPlayer->play();
        });

        //监视是否需要回到原本的音乐
        connect(gaming, &Load::backReach, [=]()
        {
            skadiList->setCurrentIndex(0);
            skadiPlayer->stop();
            reachPlayer->play();
        });
    }
    else if (mode == 2) {
        DoubleLoad* gaming = new DoubleLoad(path);
        musicPlayer->stop();
        this->close();
        gaming->show();
        reachList->next();
        reachPlayer->play();

        connect(gaming, &DoubleLoad::exitGame, [=]()
            {
                gaming->close();
                reachPlayer->stop();
                endPlayer->stop();
                this->show();
                musicList->next();
                musicPlayer->play();
            });

        //监视载入游戏，此时关闭上一个游戏
        connect(gaming, &DoubleLoad::loadGame, [=]()
            {
                QString path;
                int mode;
                if (load(path, mode)) {
                    gaming->close();
                    reachPlayer->stop();
                    gamingLoad(path, mode);
                }
                else return;
            });

        //监视游戏结束，此时播放结束音乐
        connect(gaming, &DoubleLoad::end, [=]()
        {
            reachPlayer->stop();
            endPlayer->play();
        });
    }
}
