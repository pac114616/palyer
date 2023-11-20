#include "ijkmediaplayer.h"

IjkMediaPlayer::IjkMediaPlayer()
{

}
IjkMediaPlayer::~IjkMediaPlayer()
{
    std::cout << " ~IjkMediaPlayer()\n ";
}
int IjkMediaPlayer::ijkmp_start()
{
    ffp_notify_msg1(ffplayer_, FFP_REQ_START);
    return 0;
}

int IjkMediaPlayer::ijkmp_stop()
{
    int retval =  ffplayer_->ffp_stop_l();
    if(retval<0)
        return retval;
}
int IjkMediaPlayer::ijkmp_create(std::function<int (void *)> msg_loop)
{
    int ret = 0;
    ffplayer_ = new FFPlayer();
    if(!ffplayer_)
    {
        std::cout<<"new FFPlayer() failed\n";
        return -1;
    }
    msg_loop_ = msg_loop;
    ret = ffplayer_->ffp_create();
    if(ret<0)
    {
        return -1;
    }
    return 0;
}
int IjkMediaPlayer::ijkmp_get_msg(AVMessage *msg,int block)
{
    while(1)
    {
        int continue_wait_next_msg = 0;
        //取消息，如果没有消息则阻塞
        int reval = msg_queue_get(&ffplayer_->msg_queue_,msg,block);
        if(reval<0)
            return reval;
        switch(msg->what)
        {
        case FFP_MSG_PREPARED:
            std::cout<<__FUNCTION__<<"FFP_MSG_PREPARED"<<std::endl;
            break;
        case FFP_REQ_START:
            std::cout<<__FUNCTION__<<"FFP_REQ_START"<<std::endl;
            continue_wait_next_msg = 1;
            break;
        default:
            std::cout<<__FUNCTION__<<"default"<<msg->what<<std::endl;
            break;
        }
        if(continue_wait_next_msg)
        {
            msg_free_res(msg);
            continue;
        }
        return reval;
    }
    return -1;
}
int IjkMediaPlayer::ijkmp_destroy()
{
    ffplayer_->ffp_destroy();
    return 0;
}
//该方法来自于Android mediaplayer，本意是
//方便重载接口
int IjkMediaPlayer::ijkmp_data_set_data_source(const char *url)
{
    if(!url)
        return -1;
    //分配内存，加拷贝字符串
    data_source_ = strdup(url);
    return 0;
}
int IjkMediaPlayer::ijkmp_msg_loop(void *arg)
{
    msg_loop_(arg);
    return 0;
}

void IjkMediaPlayer::AddVideoRefreshCallBack(std::function<int (const Frame *)> callback)
{
    ffplayer_->AddVideoRefreshCallback(callback);
}
int IjkMediaPlayer::ijkmp_prepare_async()
{
    //判断状态

    //正在准备中
    mp_state_ = MP_STATE_ASYNC_PREPARING;

    //启用消息队列
    msg_queue_start(&ffplayer_->msg_queue_);

    // 创建循环线程  将IjkMediaPlayer 类的 ijkmp_msg_loop 成员函数作为线程的入口点
    //线程将在后台执行 ijkmp_msg_loop 函数中的代码，直到该函数返回或线程被显式终止。
    msg_thread_ = new std::thread(&IjkMediaPlayer::ijkmp_msg_loop,this,this);
    //调用ffplayer
    int ret = ffplayer_->ffp_prepare_async_l(data_source_);
    if(ret<0)
    {
        mp_state_ = MP_STATE_ERROR;
        return -1;
    }
    return 0;
}
