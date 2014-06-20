#include "gamewidget.h"
#include <ctime>
#include <cstring>

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedSize(QSize(270+100,270+100));

    init();
    //  newGame();
}

void GameWidget::init()
{
    this->wCount=this->hCount=9;
    this->MineCount=10;
    blockSize=30;
    update();

    for(int i=0;i<Max;i++)
    {
        for(int j=0;j<Max;j++)
        {
            map[i][j]=new Block(this);
            map[i][j]->setPoint(j,i);
            map[i][j]->setFixedSize(blockSize,blockSize);
            map[i][j]->move(50+30*j,50+30*i);
            map[i][j]->setVisible(false);
            map[i][j]->locked();
            connect(this,SIGNAL(setmk()),map[i][j],SLOT(setmk()));
        }
    }

}

void GameWidget::updateMap()
{
    this->mapSize.setWidth(wCount*blockSize);
    this->mapSize.setHeight(hCount*blockSize);

    this->setFixedSize(QSize(mapSize.width()+100,mapSize.height()+100));

}

///public
int rr[2][8]={{0,1,0,-1,1,1,-1,-1},{1,0,-1,0,1,-1,1,-1}};

void GameWidget::createGame()
{
    if(firstRun)
        firstRun=0;
    else
        return;
    int i,j;
    for(i=0;i<hCount;i++)
    {
        for(j=0;j<wCount;j++)
        {
            for(int k=0;k<8;k++)
            {
                if(cmp(i+rr[0][k],j+rr[1][k]))
                {
                    map[i][j]->setBrothers(map[i+rr[0][k]][j+rr[1][k]]);
                }
            }
            map[i][j]->setInit();
        }
    }

    blockSum=hCount*wCount;
    flagSum=0;

    Block *temp=qobject_cast<Block *>(sender());
    int lod;

    double q=(double)MineCount/(double)blockSum;
    if(q<0.5)
    {
        lod=MineCount;
        qsrand(time(NULL));

        while(lod>0)
        {
            int rw=qrand()%wCount;
            int rh=qrand()%hCount;
            if(!map[rh][rw]->isMine() &&
                    (rw!=temp->getPoint().x() && rh!=temp->getPoint().y()) &&
                    !(temp->isBrother(map[rh][rw]) ))
            {
                map[rh][rw]->setMine(1);
                lod--;
            }
        }
    }
    else
    {
        //qDebug()<<">=0.5";
        lod=blockSum-MineCount;

        bool dmap[Max][Max];
        memset(dmap,1,sizeof(dmap));
        qsrand(time(NULL));

        int tx=temp->getPoint().y();
        int ty=temp->getPoint().x();
        dmap[tx][ty]=0;
        for(int i=0;i<8;i++)
        {
            dmap[tx+rr[0][i]][ty+rr[1][i]]=0;
        }
        lod-=9;
        while(lod>0)
        {
            int rw=qrand()%wCount;
            int rh=qrand()%hCount;
            if(dmap[rh][rw])
            {
                dmap[rh][rw]=0;
                lod--;
            }
        }

        for(int i=0;i<hCount;i++)
        {
            for(int j=0;j<wCount;j++)
            {
                if(dmap[i][j])
                    map[i][j]->setMine(1);
            }
        }
    }
    emit start();

}

///slots

bool GameWidget::cmp(int i, int j)
{
    if(i<0 || j<0) return false;
    if(i>=hCount || j>=wCount) return false;

    return true;
}

void GameWidget::newGame()
{
    for(int i=0;i<Max;i++)
    {
        for(int j=0;j<Max;j++)
        {
            map[i][j]->setVisible(false);
            map[i][j]->reBrothers();
            map[i][j]->setInit();
        }
    }
    for(int i=0;i<hCount;i++)
    {
        for(int j=0;j<wCount;j++)
        {
            map[i][j]->setVisible(true);
            connect(map[i][j],SIGNAL(start()),this,SLOT(createGame()));
            connect(map[i][j],SIGNAL(isOpen()),this,SLOT(openBlock()));
            connect(map[i][j],SIGNAL(isBoom()),this,SLOT(gameMiss()));
            connect(map[i][j],SIGNAL(isFlag()),this,SLOT(isFlag()));
            connect(map[i][j],SIGNAL(disFlag()),this,SLOT(disFlag()));

            map[i][j]->unLocked();
        }
    }
    firstRun=1;
    emit mineSum(MineCount);
}

void GameWidget::gameMiss()
{
    emit miss();
}

void GameWidget::gamePause()
{
    for(int i=0;i<hCount;i++)
    {
        for(int j=0;j<wCount;j++)
        {
            map[i][j]->locked();
            map[i][j]->setVisible(false);
        }
    }
}

void GameWidget::gameStart()
{
    for(int i=0;i<hCount;i++)
    {
        for(int j=0;j<wCount;j++)
        {
            map[i][j]->unLocked();
            map[i][j]->setVisible(true);
        }
    }
}

void GameWidget::setLevel(int w,int h,int s)
{
    this->wCount=w;
    this->hCount=h;
    this->MineCount=s;
    newGame();
}

void GameWidget::openBlock()
{
    blockSum--;
    //   qDebug()<<blockSum;
    if(blockSum==MineCount)
    {
        for(int i=0;i<Max;i++)
        {
            for(int j=0;j<Max;j++)
            {
                if(map[i][j]->getStatus()==Block::Default)
                {
                    map[i][j]->setFlag();
                }
                map[i][j]->locked();
            }
        }
        emit win();
    }
}

void GameWidget::isFlag()
{
    flagSum++;
    emit mineSum(MineCount-flagSum);
}

void GameWidget::disFlag()
{
    flagSum--;
    if(flagSum<0)
        flagSum=0;
    emit mineSum(MineCount-flagSum);
}

void GameWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);

    QPen pen;
    pen.setWidth(2);
    pen.setColor(QColor(255,0,0));
    painter.setPen(pen);
    QRect rect(50-1,50-1,(blockSize)*wCount+2,(blockSize)*hCount+2);
    painter.drawRect(rect);

    painter.setFont(QFont("default",25));
    painter.drawText(rect,tr("暂停"));
    updateMap();
}

