#ifndef BLOCK_H
#define BLOCK_H

#include <QWidget>
#include <QtGui>
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QPoint>
#include <QList>
#include <QListIterator>


class Block : public QWidget
{
    Q_OBJECT
public:
    explicit Block(QWidget *parent = 0);
    ~Block();

    void setMine(bool m);           ///设置成雷　当传入true
    void setInit();                         ///游戏开始时应该第一个执行的初始化在状态函数
    void setBrothers(Block *b);             ///传入兄弟方格　也就是身边的方格(共计要传８次)
    bool isBrother(Block *t);
    void locked();                              ///锁定 使用初始化或unLocked解锁
    void unLocked();                       ///解锁
    void reBrothers();                      ///解除兄弟
    void setPoint(int i,int j);
    QPoint getPoint() const;

    void setNum(int s);
    int getNum();
    int getStatus();

    bool isMine();
    bool isEnabled();

    void setFlag();

public:
    enum{Default=0,                                         //默认遮盖状态
         Press,                                                          //按下鼠标时的状态
         Flag,                                                                //插旗状态(对立状态是Default)
         Boom,                                                           //爆炸
         Open };                                                  //翻开后的状态
    enum{Mine=9};


protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void clicked(int i,int n);                                             //探测时的信号   i=1 按下，i=2 抬起，i=3 探测状态的抬起，n是上一个计数源
    void isFlag();                                                  //插旗	发给计分器
    void disFlag();                                                 //去旗	发给计分器
    void isBoom();                                                  //爆炸 发给计分器
    void start();
    void isOpen();                                               //被翻开的信号 发给计分器

public slots:
    void action(int i,int n);                                              //触发探测
    void Flags();
    void reFlags();
    void miss();
    void setmk();

private:
    void open();                                                ///翻起方块 将显示下方内容，可能意味着踩到雷
    void explore();                         ///触发当前Block计算身边的雷数

private:
    bool enabled;                                                       //全局可控性 当游戏结束时 值为false

    QPoint p;
    bool type;                                                            ///Mine or !Mine
    int num;                                                                 ///0~Mine;
    int count;
    int status;                                                             //上层的当前状态
    QPixmap pm[10];                                                   ///0,1,2,3,4,5,6,7,8,Mine
    QPixmap zg[4];                                                      ///Default,Press,Flag,Boom
    QList<Block *> brothers;                                    //兄弟方块被存储在链表中

    int button;
    int mk;
};

#endif // BLOCK_H
