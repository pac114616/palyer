#ifndef IJKMEDIAPLAYER_H
#define IJKMEDIAPLAYER_H
#include<iostream>
#include <functional>
#include <mutex>
#include <thread>
//#include "ffp
#include "ff_ffplayer.h"
#include "ffmsg_queue.h"
enum mpState{
    MP_STATE_IDLE = 0,
    MP_STATE_INITIALIZED,
    MP_STATE_ASYNC_PREPARING,
    MP_STATE_PREPARED,
    MP_STATE_STARTED,
    MP_STATE_PAUSED,
    MP_STATE_COMPLETED,
    MP_STATE_STOPPED,
    MP_STATE_ERROR,
    MP_STATE_END
};
class IjkMediaPlayer
{
public:
    IjkMediaPlayer();
    ~IjkMediaPlayer();
    int ijkmp_create(std::function<int(void*)>msg_loop);
    int ijkmp_destroy();
    //设置要播放的url
    int ijkmp_data_set_data_source(const char *url);
    //准备播放
    int ijkmp_prepare_async();
    //触发播放
    int ijkmp_start();
    //暂停
    int ijkmp_pause();
    //停止
    int ijkmp_stop();
    //seek到指定位置
    int ijkmp_seek_to(long msec);
    //获取播放状态
    int ijkmp_get_state();
    //是不是播放中
    bool ijkmp_is_playing();
    //当前播放位置
    long ijkmp_get_current_position();
    //总长度
    long ijkmp_get_duration();
    //已经播放的长度
    long ijkmp_get_playable_duration();
    //设置循环播放
    void ijkmp_get_set_loop(int loop);
    //获取是否循环播放
    int ijkmp_get_loop();
    //读取消息
    int ijkmp_get_msg(AVMessage *msg,int block);
    //设置音量
    void ijkmp_set_playback_volume(float volume);
    //
    int ijkmp_msg_loop(void *arg);
    void AddVideoRefreshCallBack(std::function<int (const Frame *)>callback);
private:
    //互斥量
    std::mutex mutex_;
    //真正的播放器
    FFPlayer *ffplayer_ = NULL;
    //函数指针，只想创建的Message_loop,即消息循环函数
    std::function<int(void *)>msg_loop_ = NULL; //ui处理消息的循环
    //消息机制线程
    std::thread *msg_thread_;//执行msg_loop
    //SDL_THread _msg_thread;
    //字符串，就是一个播放Url
    char *data_source_;
    //播放器状态，例如prepared,resumed,error,completed等
    int mp_state_;//播放状态

};

#endif // IJKMEDIAPLAYER_H
