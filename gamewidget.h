#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QtGui>
#include <QPainter>
#include "block.h"

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = 0);


protected:
    void paintEvent(QPaintEvent *);

signals:
    void win();
    void miss();
    void start();
    void mineSum(int sum);                                                ///让主程序更新雷数显示
    void setmk();

public slots:
    void newGame();
    void createGame();
    void gameMiss();
    void gamePause();
    void gameStart();
    void setLevel(int w, int h, int s);                 ///设置等级 w横向个数 h纵向个数 s雷数
    void openBlock();                                               ///方块被翻开
    void isFlag();
    void disFlag();


private:
    void init();                                                                    ///初始化地图  只在游戏开始时执行 默认产生9x9 10个雷的游戏
    void updateMap();                                                       ///更新窗口和定界框大小
    bool cmp(int i,int j);

private:
    Block *b;
    Block *b2;
    int blockWidth,blockHeigth;
    int wCount,hCount;
    QSize mapSize;
    int blockSize;
    enum{Max=35};
    Block *map[Max][Max];
    int MineCount;          ///地雷个数
    bool firstRun;

    int blockSum;
    int flagSum;


};

#endif // GAMEWIDGET_H
