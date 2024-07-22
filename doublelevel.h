#pragma once

#include "level.h"

class DoubleLevel : public Level
{
    Q_OBJECT
public:
    DoubleLevel(int w, int h);
    ~DoubleLevel();

protected:
    void paintEvent(QPaintEvent*);
    void key();

    //双人模式下，与选择有关的布尔值与分数、人物坐标等都需要第二个，scoreShow等三个QLabel则需要改变位置和大小，重新定义
    bool isSelected2, isNeared2, selectChange2, secondSelect2;
    int score2;
    int x2, y2, selectX2, selectY2, selectXX2, selectYY2;
    int matchTime2, linePoint2[8];
    QLabel* scoreShow2, * isShow2;
};
