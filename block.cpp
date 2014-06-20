#include "block.h"
#include <QDebug>

Block::Block(QWidget *parent) :
    QWidget(parent)
{
    pm[0].load(tr(":/images/block.png"));
    pm[1].load(tr(":/images/c1.png"));
    pm[2].load(tr(":/images/c2.png"));
    pm[3].load(tr(":/images/c3.png"));
    pm[4].load(tr(":/images/c4.png"));
    pm[5].load(tr(":/images/c5.png"));
    pm[6].load(tr(":/images/c6.png"));
    pm[7].load(tr(":/images/c7.png"));
    pm[8].load(tr(":/images/c8.png"));
    pm[9].load(tr(":/images/re.png"));

    zg[0].load(tr(":/images/button.png"));
    zg[1].load(tr(":/images/press.png"));
    zg[2].load(tr(":/images/flag.png"));
    zg[Boom].load(tr(":/images/boom.png"));

    this->setGeometry(0,0,50,50);
    this->setInit();
    connect(this,SIGNAL(clicked(int,int)),this,SLOT(action(int,int)));


}


void Block::setInit()
{
    status=Block::Default;
    type=false;
    num=0;
    count=0;
    mk=0;
    button=0;
    unLocked();
    update();
}

void Block::setNum(int s)
{
    if(s>8)
        s=8;
    this->num=s;
    this->count=this->num;
    update();
}

int Block::getNum()
{
    return this->num;
}

void Block::setMine(bool m)
{
    type=m;
    num=Mine;
    explore();
}

void Block::setBrothers(Block *b)
{
    brothers.append(b);
    // qDebug()<<b->pos().x()<<" "<<b->pos().y();
    //  qDebug()<<b->isMine();
    connect(this,SIGNAL(clicked(int,int)),b,SLOT(action(int,int)));
    connect(this,SIGNAL(isFlag()),b,SLOT(Flags()));
    connect(this,SIGNAL(disFlag()),b,SLOT(reFlags()));
    connect(this,SIGNAL(isBoom()),b,SLOT(miss()));

    // qDebug()<<brothers.size();
}

void Block::locked()
{
    this->enabled=false;
}

void Block::unLocked()
{
    this->enabled=true;
}

void Block::explore()
{

    QListIterator<Block *>i(brothers);
    while(i.hasNext())
    {
        Block *temp=i.next();
        //  qDebug()<<temp->pos().x()<<" "<<temp->pos().y();
        if(temp->isMine()==false)
        {
            temp->setNum(temp->getNum()+1);
        }
    }
}

bool Block::isBrother(Block *t)
{
    QListIterator<Block *>i(brothers);
    while(i.hasNext())
    {
        Block *temp=i.next();
        if(temp->getPoint()==t->getPoint())
        {
            return true;
        }
    }
    return false;
}

bool Block::isMine()
{
    return type;
}

void Block::action(int i, int n)
{
    if(status!=Open && status!=Flag && status!=Boom)
    {
        if(i==1)
        {
            status=Press;
        }
        else if(i==2)
        {
            this->status=Default;
            if(n<=0)
            {
                if(isMine())
                {
                    status=Boom;
                    this->locked();
                    emit isBoom();
                }
                else
                {
                    this->status=Open;
                    emit isOpen();
                    emit clicked(2,this->count);
                }
            }
//            if(isMine())
//            {
//                qDebug()<<this->p;
//            }
            //            else
            //            {
            //                if(this->num==0)
            //                {
            //                    this->status=Open;
            //                    emit isOpen();
            //                    emit clicked(2,0);
            //                }
            //            }
        }
        else if(i==3)
        {
            status=Default;
        }
        update();
    }
}

void Block::Flags()
{
    if(!isMine())
    {
        this->count-=1;
//        if(this->count<0)
//            this->count=0;
    }
}

void Block::reFlags()
{
    if(!isMine())
    {
        this->count+=1;
        if(this->count>num)
            this->count=num;
    }
}

int Block::getStatus()
{
    return status;
}

void Block::miss()
{
    if(this->enabled)
    {
        if(this->isMine())
        {
            //    qDebug()<<this->pos()<<"miss";
            this->status=Block::Open;

        }

        this->locked();
        update();
        emit isBoom();
    }

}

bool Block::isEnabled()
{
    return enabled;
}

void Block::reBrothers()
{
    while(!brothers.isEmpty())
    {
        brothers.pop_back();
    }
    disconnect(this,0,0,0);

}

void Block::setPoint(int i,int j)
{
    p.setX(i);
    p.setY(j);
}

QPoint Block::getPoint() const
{
    return p;
}

void Block::setFlag()
{
    if(status==Default )
    {
        status=Flag;
        emit isFlag();
    }

}

void Block::setmk()
{
    mk=!mk;
    update();
}

Block::~Block()
{
    while(!brothers.isEmpty())
        brothers.pop_back();
}

void Block::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    int side=qMin(this->width(),this->height());
    painter.setViewport((this->width()-side)/2,(this->height()-side)/2,side,side);

    painter.setWindow(0,0,50,50);

    painter.drawPixmap(0,0,50,50,pm[num]);
    
    if(status!=Open)
    {
        painter.drawPixmap(0,0,50,50,zg[status]);
    }

    if(mk==1 && isMine())
    {
        painter.setBrush(QBrush(Qt::blue));
        painter.drawEllipse(20,20,10,10);
    }
}


void Block::mousePressEvent(QMouseEvent *event)
{
    if(enabled)
    {
        if(event->button()==Qt::LeftButton && status!=Open && status!=Flag)
        {
            status=Press;
            button=1;

        }
        else if(event->button()==Qt::RightButton && status!=Open)
        {
            if(status==Flag)
            {
                status=Default;
                emit disFlag();
            }
            else
            {
                status=Flag;
                emit isFlag();
            }
        }
        else if((event->button()==Qt::LeftButton || event->button()==Qt::MidButton)&& status==Open )
        {
            emit clicked(1,count);
            button=2;
        }

        update();
    }
}

void Block::mouseReleaseEvent(QMouseEvent *event)
{
    if(enabled && button)
    {
        if((event->pos().x()>=0 && event->pos().x()<this->width()) &&
                (event->pos().y()>=0 && event->pos().y()<this->height()))
        {
            if(button==1)
            {
                if(type)
                {
                    status=Boom;
                    this->locked();
                    emit isBoom();
                }
                else
                {
                    emit start();
                    status=Open;
                    emit isOpen();
                    emit clicked(2,count);
                }
            }
            else if(button==2)
            {
                qDebug()<<"this c:"<<count;
                emit clicked(2,count);
            }

            button=0;
        }
        update();
    }
}

void Block::mouseMoveEvent(QMouseEvent *event)
{
    if(enabled && button)
    {
        if((event->pos().x()>=0 && event->pos().x()<this->width()) &&
                (event->pos().y()>=0 && event->pos().y()<this->height()))
        {
            if(button==1)
                this->status=Press;
            else
            {
                emit clicked(1,count);
            }
        }
        else
        {
            if(button==1)
                this->status=Default;
            else
                emit clicked(3,count);
        }
        update();
    }
}

