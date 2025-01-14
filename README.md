## 项目简介
这是一个使用 **Qt** 框架、基于 **C++** 编写的小游戏。它出自本人的大作业项目，而又在大作业的要求上添加了一些私货。

## 项目启动
项目最好使用 **Qt Creator** 打开（打开`simple.pro`），本人使用的版本为：

```
Qt Creator 4.11.1
Based on Qt 5.14.1 (MSVC 2017, 32 bit)
```

最好不要用更低的版本启动。


## 发布版本
如果对源码不感兴趣，可以直接下载Release（暂只支持 **Windows** 系统）。

## 游戏帮助
**_（以下内容均可以在游戏内帮助找到）_**

1. 本游戏使用RPG模式进行，你需要控制角色在地图上移动，选定能够通过两次以内的折线进行连接的相同方块，以消除它们
2. 倒计时结束或者没有可消除的方块对（包括全部消除）时游戏结束
3. 单人模式下支持道具、方块有特殊效果，可以通过捡起地图上的不同道具丰富游戏体验
4. 游戏暂停时可以进行存档。存档可以将游戏存档保存至本地，主菜单页面可读档进行游戏
5. 游戏分别通过W S A D控制人物上下左右移动，游戏过程中按P键暂停/继续。游戏开始时默认暂停
6. 靠近想消除的第一个方块后，按J键选中，第二个方块按K选中
7. 对于双人游戏，采用方向键控制人物移动，1 2键分别对应选择第一、第二个方块。双人模式下，多利为2分，其他方块为1分
8. 单人游戏可以在游戏开始前选择是否有碰撞体积，双人游戏有碰撞体积

## 游戏内方块与道具
**_（以下内容均可以在游戏内帮助找到，本节内容只适用于单人模式）_**

1. 道具刷新间隔为20s。若即将刷新时未拾起，则会在拾起后再刷新、开始冷却
2. 赞：+15s剩余时间
3. 绝好调：直接加分，数值为当前分数的个位数与十位数之和
4. 犀利：随机选择一对可以被消除的方块，并圈起
5. 掀桌：重新刷新所有未消除的方块
6. 多利：消除一对后额外+1分
7. 杰斯顿：消除一对后额外+2分，并-2s剩余时间
8. 傀影：消除一对后，道具冷却时间-3s
9. “复仇者”：消除一对后额外+2分，+2s剩余时间，接下来10s内无法再触发此特效
10. 伊莎玛拉：每局游戏限一次，累计消除6对后触发大群呼唤，10s内消除方块的得分翻倍
11. W：每局游戏限一次，消除一对后将-5s剩余时间，并引发两个3*3的爆炸，爆炸的方块不会触发额外效果，但是每个方块+1分