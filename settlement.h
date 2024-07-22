#pragma once

#include <QMainWindow>
#include <QLabel>

class Settlement : public QMainWindow
{
    Q_OBJECT
public:
    Settlement(int s,int s2);

protected:
    void paintEvent(QPaintEvent *) override;
    int score, score2;
    bool isDraw;
    QPixmap surtr, maj;
    QLabel number, number2, words, compare;
};
