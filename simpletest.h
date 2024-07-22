#pragma once

#include <QtTest/QtTest>

class SimpleTest : public QObject
{
    Q_OBJECT

private slots:
    void case1_testcase();
    void case2_testcase();
    void case3_testcase();
    void case4_testcase();
    void case5_testcase();
    void case6_testcase();
    void case7_testcase();
    void case8_testcase();
    void case9_testcase();
    void case10_testcase();

public:
    SimpleTest();

    //对于测试，需要指定一个地图，p负责储存这个指定的地图
    int p[10][14] = {
        {0},
        {0,4,1,3,2,1,1,2,6,3,4,6,3,0},
        {0,4,3,2,2,3,1,5,1,6,6,5,6,0},
        {0,4,3,1,2,4,4,5,5,3,4,6,6,0},
        {0,0,0,4,2,1,3,6,6,3,1,3,4,0},
        {0,4,3,6,2,5,2,4,2,2,3,3,2,0},
        {0,5,1,2,5,5,4,4,3,1,3,6,4,0},
        {0,4,1,2,4,1,4,6,1,5,6,5,2,0},
        {0,5,1,4,4,3,1,3,1,5,1,4,2,0},
        {0}
    };
};

