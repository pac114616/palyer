#include "ctrlbar.h"
#include "ui_ctrlbar.h"
#include <QDebug>
CtrlBar::CtrlBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CtrlBar)
{
    ui->setupUi(this);
//    full_screen;
//    ui->PlayOrPauseBtn->setIcon((QIcon)(":icon/icon/pause.png"));
}

CtrlBar::~CtrlBar()
{
    delete ui;
}

void CtrlBar::on_PlayOrPauseBtn_clicked()
{
    qDebug()<<"on_playOrPauseBtn_clicked";
    emit SigPlayOrPause();
}


void CtrlBar::on_stopBtn_clicked()
{
    qDebug()<<"on_stopBtn_clicked";
    emit SigStop();
}

