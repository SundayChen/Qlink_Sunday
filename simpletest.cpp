#include "simpletest.h"
#include "level.h"

SimpleTest::SimpleTest()
{

}

void SimpleTest::case1_testcase()
{
    //测试竖直直连的两个方块消除
    Level* level = new Level(12, 8);
    for (int i = 1; i <= 8; ++i) {
        for (int j = 1; j <= 12; ++j) level->map[i][j] = p[i][j];
    }
    int a, b, c, d;
    QVERIFY(level->isMatched(1, 1, 2, 1, a, b, c, d) == true);
}

void SimpleTest::case2_testcase()
{
    //测试水平直连的两个方块消除
    Level* level = new Level(12, 8);
    for (int i = 1; i <= 8; ++i) {
        for (int j = 1; j <= 12; ++j) level->map[i][j] = p[i][j];
    }
    int a, b, c, d;
    QVERIFY(level->isMatched(1, 5, 1, 6, a, b, c, d) == true);
}

void SimpleTest::case3_testcase()
{
    //测试通过一次转弯链接的两个方块消除
    Level* level = new Level(12, 8);
    for (int i = 1; i <= 8; ++i) {
        for (int j = 1; j <= 12; ++j) level->map[i][j] = p[i][j];
    }
    int a, b, c, d;
    QVERIFY(level->isMatched(3, 1, 4, 3, a, b, c, d) == true);
}

void SimpleTest::case4_testcase()
{
    //测试通过两次转弯链接的两个方块消除
    Level* level = new Level(12, 8);
    for (int i = 1; i <= 8; ++i) {
        for (int j = 1; j <= 12; ++j) level->map[i][j] = p[i][j];
    }
    int a, b, c, d;
    QVERIFY(level->isMatched(1, 2, 1, 5, a, b, c, d) == true);
}

void SimpleTest::case5_testcase()
{
    //当第一个方块的坐标超出范围时
    Level* level = new Level(12, 8);
    for (int i = 1; i <= 8; ++i) {
        for (int j = 1; j <= 12; ++j) level->map[i][j] = p[i][j];
    }
    int a, b, c, d;
    QVERIFY(level->isMatched(-1, 56, 5, 8, a, b, c, d) == false);
}

void SimpleTest::case6_testcase()
{
    //当第二个方块的坐标超出范围时
    Level* level = new Level(12, 8);
    for (int i = 1; i <= 8; ++i) {
        for (int j = 1; j <= 12; ++j) level->map[i][j] = p[i][j];
    }
    int a, b, c, d;
    QVERIFY(level->isMatched(1, 8, 2, 13, a, b, c, d) == false);
}

void SimpleTest::case7_testcase()
{
    //当指定的方块包含已消除的方块时
    Level* level = new Level(12, 8);
    for (int i = 1; i <= 8; ++i) {
        for (int j = 1; j <= 12; ++j) level->map[i][j] = p[i][j];
    }
    int a, b, c, d;
    QVERIFY(level->isMatched(4, 1, 5, 6, a, b, c, d) == false);
}

void SimpleTest::case8_testcase()
{
    //当选择的两个方块是同一个方块时
    Level* level = new Level(12, 8);
    for (int i = 1; i <= 8; ++i) {
        for (int j = 1; j <= 12; ++j) level->map[i][j] = p[i][j];
    }
    int a, b, c, d;
    QVERIFY(level->isMatched(6, 6, 6, 6, a, b, c, d) == false);
}

void SimpleTest::case9_testcase()
{
    //当选择的两个方块图案不一样时
    Level* level = new Level(12, 8);
    for (int i = 1; i <= 8; ++i) {
        for (int j = 1; j <= 12; ++j) level->map[i][j] = p[i][j];
    }
    int a, b, c, d;
    QVERIFY(level->isMatched(8, 12, 8, 11, a, b, c, d) == false);
}

void SimpleTest::case10_testcase()
{
    //当选择的两个方块需要三次及以上转弯才能消除时
    Level* level = new Level(12, 8);
    for (int i = 1; i <= 8; ++i) {
        for (int j = 1; j <= 12; ++j) level->map[i][j] = p[i][j];
    }
    int a, b, c, d;
    QVERIFY(level->isMatched(1, 1, 8, 3, a, b, c, d) == false);
}

//QTEST_MAIN(SimpleTest)
