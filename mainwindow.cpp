#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowIcon(QIcon(tr(":/images/ico.png")));

    menuBar=new QMenuBar(this);
    gameMenu=new QMenu(tr("游戏(&g)"),this);
    menuBar->addMenu(gameMenu);
    newAction=new QAction(tr("新游戏"),this);
    newAction->setShortcut(tr("F2"));
    gameMenu->addAction(newAction);
    connect(newAction,SIGNAL(triggered()),this,SLOT(newGame()));

    startAction=new QAction(tr("开始"),this);
    startAction->setShortcut(tr("P"));
    gameMenu->addAction(startAction);
    connect(startAction,SIGNAL(triggered()),this,SLOT(gameStart()));
    startAction->setEnabled(false);

    pauseAction=new QAction(tr("暂停"),this);
    pauseAction->setShortcut(tr("P"));
    gameMenu->addAction(pauseAction);
    connect(pauseAction,SIGNAL(triggered()),this,SLOT(gamePause()));

    rankingAction=new QAction(tr("最高纪录"),this);
    rankingAction->setShortcut(tr("Ctrl+R"));
    gameMenu->addAction(rankingAction);
    connect(rankingAction,SIGNAL(triggered()),this,SLOT(showRanking()));

    quitAction=new QAction(tr("退出"),this);
    quitAction->setShortcut(QKeySequence::Quit);
    gameMenu->addSeparator();
    gameMenu->addAction(quitAction);
    connect(quitAction,SIGNAL(triggered()),this,SLOT(close()));


    levelMenu=new QMenu(tr("难度(&l)"),this);
    menuBar->addMenu(levelMenu);
    aLAction=new QAction(tr("9x9,10个雷"),this);
    aLAction->setData("a");
    levelMenu->addAction(aLAction);

    bLAction=new QAction(tr("16x16,40个雷"),this);
    bLAction->setData("b");
    levelMenu->addAction(bLAction);

    cLAction=new QAction(tr("30x16,99个雷"),this);
    cLAction->setData("c");
    levelMenu->addAction(cLAction);

    dLAction=new QAction(tr("自定义"),this);
    dLAction->setData("d");
    levelMenu->addAction(dLAction);

    connect(aLAction,SIGNAL(triggered()),this,SLOT(setLevel()));
    connect(bLAction,SIGNAL(triggered()),this,SLOT(setLevel()));
    connect(cLAction,SIGNAL(triggered()),this,SLOT(setLevel()));
    connect(dLAction,SIGNAL(triggered()),this,SLOT(setLevel()));

    menuBar->addSeparator();

    helpMenu=new QMenu(tr("帮助(&h)"),this);
    menuBar->addMenu(helpMenu);

    aboutAction=new QAction(tr("关于"),this);
    helpMenu->addAction(aboutAction);
    connect(aboutAction,SIGNAL(triggered()),this,SLOT(about()));

    commButton=new QPushButton(this);
    // commButton->setText(tr("新游戏"));
    commButton->setIconSize(QSize(30,30));
    commButton->setFixedSize(40,40);
    commButton->setIcon(QIcon(tr(":/images/face1.png")));

    timeLabel=new QLabel(this);
    timeLabel->setPixmap(QPixmap(tr(":/images/clock.png")));
    timeLabel->setFixedSize(30,30);
    mineLabel=new QLabel(this);
    mineLabel->setPixmap(QPixmap(tr(":/images/mine.png")));
    timeLCD=new QLCDNumber(this);
    mineLCD=new QLCDNumber(this);
    QPalette pl;
    pl.setColor(QPalette::WindowText,QColor(255,0,0));
    timeLCD->setPalette(pl);
    mineLCD->setPalette(pl);

    QHBoxLayout *timeLayout=new QHBoxLayout;
    timeLayout->addWidget(timeLabel);
    timeLayout->addWidget(timeLCD);

    QHBoxLayout *mineLayout=new QHBoxLayout;
    mineLayout->addWidget(mineLabel);
    mineLayout->addWidget(mineLCD);

    QHBoxLayout *topLayout=new QHBoxLayout;
    topLayout->addLayout(timeLayout);
    topLayout->addStretch();
    topLayout->addWidget(commButton);
    topLayout->addStretch();
    topLayout->addLayout(mineLayout);

    gameWidget = new GameWidget(this);

    QVBoxLayout *mainLayout=new QVBoxLayout;
    mainLayout->addWidget(menuBar);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(gameWidget);

    this->setLayout(mainLayout);
    layout()->setSizeConstraint(QLayout::SetFixedSize);


    tTimer=new QTimer(this);
    connect(tTimer,SIGNAL(timeout()),this,SLOT(updateTime()));

    oldTime=QDateTime::currentDateTime();
    tTimer->start(10);
    run=false;

    connect(gameWidget,SIGNAL(win()),this,SLOT(gameWin()));
    connect(gameWidget,SIGNAL(miss()),this,SLOT(gameMiss()));
    connect(commButton,SIGNAL(clicked()),this,SLOT(newGame()));
    connect(gameWidget,SIGNAL(start()),this,SLOT(gameStart()));
    connect(gameWidget,SIGNAL(mineSum(int)),this,SLOT(updateMineSum(int)));

    isZb=false;
    level=0;
    readRank();
    newGame();

    this->move(200,100);

    levelDialog=new LevelDialog(this);
    connect(levelDialog,SIGNAL(getLevel(int,int,int)),this,SLOT(setWHM(int,int,int)));
}

MainWindow::~MainWindow()
{

}

void MainWindow::newGame()
{
    commButton->setIcon(QIcon(tr(":/images/face1.png")));

    timeLCD->display(0);
    secs=0;
    isMiss=1;
    run=false;
    startAction->setEnabled(false);
    pauseAction->setEnabled(true);
    gameWidget->newGame();
}

void MainWindow::gameWin()
{
    run=false;
    commButton->setIcon(QIcon(tr(":/images/face3.png")));

    QString str;
    if(!isZb){
        if(level==0)
            str="9x9,10个雷";
        else if(level==1)
            str="16x16,40个雷";
        else if(level==2)
            str="30x16,99个雷";
        else
            str="自定义";

        if(level<3 && rank[level]>secs)
        {
            rank[level]=secs;
            saveRank();

            QMessageBox::about(this,tr("新纪录"),tr("恭喜您创造新纪录:\n"
                                                 "模式%1,得分:%2s.").arg(str).arg(secs));
        }
        else
        {
            if(level<3)
                QMessageBox::about(this,tr("胜利"),tr("恭喜您完成游戏:\n"
                                                    "模式%1,得分:%2s\n"
                                                    "该模式的最高记录:%3s.").arg(str).arg(secs).arg(rank[level]));
            else
                QMessageBox::about(this,tr("胜利"),tr("恭喜您完成游戏:\n"
                                                    "模式%1,得分:%2s\n").arg(str).arg(secs));
        }
    }
    else
    {
        QMessageBox::about(this,tr("测试模式"),tr("测试模式成绩:\n"
                                             "模式%1,得分:%2s.").arg(str).arg(secs));
    }

    isZb=false;
}

void MainWindow::gameMiss()
{
    run=false;
    isMiss=1;
    isZb=false;
    commButton->setIcon(QIcon(tr(":/images/face2.png")));
}

void MainWindow::gameStart()
{
    isMiss=0;
    if(isMiss)
        return;

    gameWidget->gameStart();

    run=true;
    startAction->setEnabled(false);
    pauseAction->setEnabled(true);
}

void MainWindow::gamePause()
{
    if(isMiss)
        return;

    run=false;
    gameWidget->gamePause();
    startAction->setEnabled(true);
    pauseAction->setEnabled(false);
}

void MainWindow::updateTime()
{
    if(run)
    {
        if(oldTime.secsTo(QDateTime::currentDateTime())!=0)
        {
            secs+=oldTime.secsTo(QDateTime::currentDateTime());
            oldTime=QDateTime::currentDateTime();
        }

        if(secs>9999)
            secs=9999;
        timeLCD->display(secs);
    }
    else
    {
        oldTime=QDateTime::currentDateTime();
    }

}

void MainWindow::updateMineSum(int sum)
{
    mineLCD->display(sum);
}

void MainWindow::readRank()
{
    QFile file("rank.db");

    if(!file.open(QIODevice::ReadOnly))
    {
        rank[0]=rank[1]=rank[2]=9999;
        saveRank();
    }
    else
    {
        QDataStream in(&file);
        in>>rank[0]>>rank[1]>>rank[2];
    }
}

void MainWindow::saveRank()
{
    QFile file("rank.db");

    if(file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        out<<rank[0]<<rank[1]<<rank[2];
    }

}

void MainWindow::setLevel()
{
    QAction *temp=qobject_cast<QAction *>(sender());

    if(temp->data()=="a")
    {
        wCount=hCount=9;
        mineCount=10;
        level=0;
        gameWidget->setLevel(wCount,hCount,mineCount);
        newGame();
    }
    else if(temp->data()=="b")
    {
        wCount=hCount=16;
        mineCount=40;
        level=1;
        gameWidget->setLevel(wCount,hCount,mineCount);
        newGame();
    }
    else if(temp->data()=="c")
    {
        wCount=30;
        hCount=16;
        mineCount=99;
        level=2;
        gameWidget->setLevel(wCount,hCount,mineCount);
        newGame();
    }
    else if(temp->data()=="d")
    {
        this->gamePause();
        levelDialog->show();
        levelDialog->raise();
        levelDialog->activateWindow();
        level=3;
    }


}

void MainWindow::setWHM(int w, int h, int m)
{
    wCount=w;
    hCount=h;
    mineCount=m;
    gameWidget->setLevel(wCount,hCount,mineCount);
    newGame();
}

void MainWindow::showRanking()
{
    QMessageBox::about(this,tr("最高纪录"),tr("9x9,10个雷:  \t%1s\n"
                                          "16x16,40个雷: \t%2s\n"
                                          "30x16,99个雷: \t%3s").arg(rank[0]).arg(rank[1]).arg(rank[2]));
}

void MainWindow::about()
{
    QMessageBox::about(this,tr("gxinMine"),tr("<h1>gxinMine(扫雷)v1.0</h1>\n"
                                              "<font color='#FF0000'>By: Gxin</font>    2014<br />"
                                              "自由软件,可随意拷贝，不得用于商业用途。"));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_U)
    {
        emit gameWidget->setmk();
        isZb=true;
    }
}
