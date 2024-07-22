#pragma once

#include <QMainWindow>
#include "level.h"
#include "doublelevel.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTimer>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent*) override;
    void singleGame();
    void doubleGame();
    void loadGame();
    bool load(QString& path, int& mode);
    void gamingLoad(const QString& path, int mode);

    QMediaPlaylist *musicList, *reachList, *skadiList;
    QMediaPlayer *musicPlayer, *reachPlayer, *endPlayer, *skadiPlayer;
};
