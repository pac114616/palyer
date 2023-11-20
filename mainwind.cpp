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

int MainWind::OutputVideo(const Frame *frame)
{
    return ui->showWind->Draw(frame);
}
