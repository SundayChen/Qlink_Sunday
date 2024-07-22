#include "doubleload.h"

DoubleLoad::DoubleLoad(const QString& path) :DoubleLevel(1, 1)
{
    //载入存档文件
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);

    //读取存档文件的信息，并赋予相应的变量
    QString line = in.readLine();
    line = in.readLine();
    QStringList list = line.split(" ");
    remainTime = list[0].toInt();
    line = in.readLine();
    list = line.split(" ");
    score = list[0].toInt();
    score2 = list[1].toInt();
    line = in.readLine();
    list = line.split(" ");
    x = list[0].toInt();
    y = list[1].toInt();
    line = in.readLine();
    list = line.split(" ");
    x2 = list[0].toInt();
    y2 = list[1].toInt();
    line = in.readLine();
    list = line.split(" ");
    height = list[0].toInt();
    width = list[1].toInt();

    //载入地图
    map = new int* [height + 2];
    for (int i = 0; i <= height + 1; ++i) map[i] = new int[width + 2];
    for (int i = 0; i <= width + 1; ++i) map[0][i] = map[height + 1][i] = 0;
    for (int i = 0; i <= height + 1; ++i) map[i][0] = map[i][width + 1] = 0;

    for (int i = 1; i <= height; ++i) {
        line = in.readLine();
        list = line.split(" ");
        for (int j = 1; j <= width; ++j) {
            map[i][j] = list[j - 1].toInt();
        }
    }
    file.close();

    //基类执行构造函数时采用默认1行1列，必然会无解，此时要消除这个假无解
    gameOver = false;

    //若存档无解则游戏终止
    if (!isSolvable()) isShow->setText("游戏无解");
}
