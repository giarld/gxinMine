#include "leveldialog.h"

LevelDialog::LevelDialog(QWidget *parent) :
    QDialog(parent)
{

    this->setWindowTitle(tr("自定义"));

    wLabel=new QLabel(tr("宽度(5-33):"),this);
    wSpin=new QSpinBox(this);
    wSpin->setRange(5,33);

    hLabel=new QLabel(tr("高度(5-33):"),this);
    hSpin=new QSpinBox(this);
    hSpin->setRange(5,33);

    mineLabel=new QLabel(tr("雷数(4-15):"),this);
    mineSpin=new QSpinBox(this);
    mineSpin->setRange(4,15);

    okButton=new QPushButton(tr("确定"),this);
    cancelButton=new QPushButton(tr("取消"),this);

    QHBoxLayout *wLayout=new QHBoxLayout;
    wLayout->addWidget(wLabel);
    wLayout->addWidget(wSpin);

    QHBoxLayout *hLayout=new QHBoxLayout;
    hLayout->addWidget(hLabel);
    hLayout->addWidget(hSpin);

    QHBoxLayout *mineLayout=new QHBoxLayout;
    mineLayout->addWidget(mineLabel);
    mineLayout->addWidget(mineSpin);

    QHBoxLayout *buttonLayout=new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout=new QVBoxLayout;
    mainLayout->addLayout(wLayout);
    mainLayout->addLayout(hLayout);
    mainLayout->addLayout(mineLayout);
    mainLayout->addLayout(buttonLayout);

    this->setLayout(mainLayout);

    connect(wSpin,SIGNAL(valueChanged(int)),this,SLOT(updateMineSpin()));
    connect(hSpin,SIGNAL(valueChanged(int)),this,SLOT(updateMineSpin()));
    connect(okButton,SIGNAL(clicked()),this,SLOT(clickSlot()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
}


void LevelDialog::updateMineSpin()
{
    int max=wSpin->value()*hSpin->value()-10;
    mineSpin->setRange(4,max);
    mineLabel->setText(tr("雷数(4-%1):").arg(max));
}

void LevelDialog::clickSlot()
{
    emit getLevel(wSpin->value(),hSpin->value(),mineSpin->value());
    close();
}
