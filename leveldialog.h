#ifndef LEVELDIALOG_H
#define LEVELDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QLayout>

class LevelDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LevelDialog(QWidget *parent = 0);

signals:
    void getLevel(int w,int h,int m);

public slots:
    void updateMineSpin();
    void clickSlot();

private:
    QLabel *wLabel;
    QSpinBox *wSpin;

    QLabel *hLabel;
    QSpinBox *hSpin;

    QLabel *mineLabel;
    QSpinBox *mineSpin;

    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // LEVELDIALOG_H
