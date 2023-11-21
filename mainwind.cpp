#include "mainwind.h"
#include "qdebug.h"
#include "ui_mainwind.h"

MainWind::MainWind(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWind)
{
    ui->setupUi(this);
    //设置播放图标
//    QIcon icon_play()
//    ui->ctrlBarWind->PlayOrPauseBtn

//    connect(ui->ctrlBarWind,SIGNAL(SigPlayOrPause()),this,SLOT(on_play_or_pause()));
    connect(ui->ctrlBarWind,&CtrlBar::SigPlayOrPause,this,&MainWind::on_play_or_pause);
    connect(ui->ctrlBarWind,&CtrlBar::SigStop,this,&MainWind::on_stop);
    connect(ui->ctrlBarWind,SIGNAL(setListVisible(bool)),this,SLOT(getListVisible(bool)));
}

MainWind::~MainWind()
{
    delete ui;
}

int MainWind::msg_loop(void *arg)
{
    qDebug()<<"message_loop into";
    while(1)
    {
        AVMessage msg;

        int ret = mp_->ijkmp_get_msg(&msg,1);

        if(ret<0)
            break;
        switch(msg.what)
        {
        case FFP_MSG_FLUSH:
            qDebug()<<__FUNCTION__<<" FFP_MSG_FLUSH";
            break;
        case FFP_MSG_PREPARED:
            std::cout<<__FUNCTION__<<"FFP_MSG_PREPARED";
            mp_->ijkmp_start();
            break;
        default:
            qDebug()<<__FUNCTION__<<"default"<<msg.what;
            break;
        }
        msg_free_res(&msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    qDebug()<<"message_loop leave";
    return 0;
}

void MainWind::on_play_or_pause()
{
    qDebug()<<"OnPlayOrPause begin";
    int ret = 0;
    if(!mp_)
    {
        mp_ = new IjkMediaPlayer();
        //1.1 创建      将MainWind成员函数message_loop及第一个参数绑定
        //然后 赋值给 IjkMediaPlayer对象的std::function<int(void *)> msg_loop_
        ret = mp_->ijkmp_create(std::bind(&MainWind::msg_loop,this,std::placeholders::_1));
        if(ret <0) {
            qDebug() << "IjkMediaPlayer create failed";
            delete mp_;
            mp_ = NULL;
            return;
        }
        mp_->AddVideoRefreshCallBack(std::bind(&MainWind::OutputVideo,this,
                                               std::placeholders::_1));
        ret = mp_->ijkmp_data_set_data_source("test_1920x1080.mp4");
        if(ret <0) {
            qDebug() << "ijkmp_data_set_data_source failed";
            delete mp_;
            mp_ = NULL;
            return;
        }
        mp_->ijkmp_prepare_async();
        if(ret <0) {
            qDebug() << "ijkmp_prepare_async failed";
            delete mp_;
            mp_ = NULL;
            return;
        }
    }else
    {

    }
}

void MainWind::on_stop()
{
    qDebug()<<"begin stop";
    if(mp_)
    {
        mp_->ijkmp_stop();
        mp_->ijkmp_destroy();
        delete mp_;
        mp_ = NULL;
    }
}

void MainWind::resizeUINOFullScreen()
{
    int width = this->width();
    int playListWidth = ui->playList->width();
//    int playListWidth = ui->playList->width();
//    int ctrlBarHeight = ui->ctrlBarWind->height();
    int ctrlBarHeight = 90;
    int height = this->height();
//    if(_isListHide)


//    QRect ctlBar = ui->ctrlBarWind->geometry();
//    ctlBar.setHeight(ctrlBarHeight);

//    ctlBar.setY(height-ui->menuBar->height()-ctrlBarHeight);//767
//    ctlBar.setY(height-ctrlBarHeight);//767
//    ctlBar.setWidth(width);
//    ctlBar.setX(0);
//    ctlBar.setHeight(ctrlBarHeight);
    qDebug()<<"----------";
    qDebug()<<"width"<<width;
//    qDebug() << "ctrlBar rect: " << ctlBar;
//    QRect playCtl = ui->showWind->geometry();
    qDebug()<<"height-"<<height<<"ctrlBar Heigth"<<ctrlBarHeight;
    if(_isListVisible)
    {
        width = this->width()-playListWidth;
    }

//    playCtl.setY(height-ui->menuBar->height());

    height = this->height() - ui->menuBar->height()-ctrlBarHeight;
//    ui->showWind->setGeometry(0,ui->menuBar->height(),width,height);
    ui->showWind->setGeometry(0,0,width,height);
//    ui->playList->setGeometry(ui->showWind->width(),ui->menuBar->height(),playListWidth,height);
    ui->playList->setGeometry(ui->showWind->width(),0,playListWidth,height);
    ui->ctrlBarWind->setGeometry(0,height,this->width(),ctrlBarHeight);

    qDebug() << "show rect: " << ui->showWind->geometry();
    qDebug() << "playList rect: " << ui->playList->geometry();
    qDebug() << "ctrlBar rect: " << ui->ctrlBarWind->geometry();
}

void MainWind::resizeEvent(QResizeEvent *event)
{
    resizeUINOFullScreen();
}

void MainWind::getListVisible(bool visible)
{
    _isListVisible = visible;
    if(_isListVisible)
        ui->playList->show();
    else
        ui->playList->hide();
    resizeUINOFullScreen();
}

int MainWind::OutputVideo(const Frame *frame)
{
    return ui->showWind->Draw(frame);
}
