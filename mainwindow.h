#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QtGui>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QPushButton>
#include <QLCDNumber>
#include <QLabel>
#include <QLayout>
#include <QTimer>
#include <QDateTime>
#include <QDataStream>
#include <QMessageBox>

#include "gamewidget.h"
#include "leveldialog.h"


class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void gameWin();
    void gameMiss();
    void newGame();
    void updateTime();
    void gameStart();
    void gamePause();
    void updateMineSum(int sum);
    void setLevel();
    void setWHM(int w,int h,int m);
    void showRanking();
    void about();

private:
    void readRank();
    void saveRank();


private:
    GameWidget *gameWidget;
    QToolBar *mainToolBar;
    QPushButton *commButton;
    QLCDNumber *timeLCD;
    QLCDNumber *mineLCD;
    QLabel *timeLabel;
    QLabel *mineLabel;


    QMenuBar *menuBar;
    QMenu *gameMenu;
    QAction *newAction;
    QAction *startAction;
    QAction *pauseAction;
    QAction *rankingAction;
    QAction *quitAction;

    QMenu *levelMenu;
    QAction *aLAction;
    QAction *bLAction;
    QAction *cLAction;
    QAction *dLAction;

    QMenu *helpMenu;
    QAction *aboutAction;

    QTimer *tTimer;
    QDateTime oldTime;
    bool run;

    LevelDialog *levelDialog;

    int secs;
    bool isMiss;
    bool isWin;

    bool isZb;        //

    int level;
    int rank[3];
    int wCount,hCount,mineCount;
};

#endif // MAINWINDOW_H
