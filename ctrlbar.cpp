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


void CtrlBar::on_listVisible_clicked()
{

    _listVisible = !_listVisible;
    emit setListVisible(_listVisible);
}


void CtrlBar::on_cut_clicked()
{

}


void CtrlBar::on_quickForward_clicked()
{

}


void CtrlBar::on_quickBackward_clicked()
{

}


void CtrlBar::on_comboBox_currentTextChanged(const QString &arg1)
{

}


