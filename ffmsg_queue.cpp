#include "ffmsg_queue.h"
#include "ffmsg.h"

void msg_free_res(AVMessage *msg)
{
    if(!msg||!msg->obj)
        return ;
    msg->free_l(msg->obj);
    msg->obj = NULL;
}

//消息队列内部重新构建 AVMessage(重新申请AVMessage,或者来自于recycle_msg)
//新消息插入到尾部
int msg_queue_put_private(MessageQueue *q, AVMessage *msg)
{
    AVMessage *msg1;
    if(q->abort_request)
        return -1;
    //重新分配或者回收
    msg1 = q->recycle_msg;
    if(msg1)
    {
        q->recycle_msg = msg1->next;
        q->recycle_count++;
    }else
    {
        q->alloc_count++;
        msg1 = (AVMessage *)av_malloc(sizeof(AVMessage));
    }
    //
    *msg1 = *msg;
//        *msg = *msg1;
    msg1->next = NULL;

    if(!q->first_msg)
    {
        q->first_msg = msg1;
    }else
    {
        q->last_msg->next = msg1;
    }
    q->last_msg = msg1;
    q->nb_message++;
    //唤醒条件变量
    SDL_CondSignal(q->cond);
    return 0;
}
//读取消息 从头部取消息
//return<0 if aborted,0 if no msg and > 0 if msg.
int msg_queue_get(MessageQueue *q,AVMessage *msg,int block)
{
    AVMessage *msg1;
    int ret;

    SDL_LockMutex(q->mutex);

    for(;;)
    {
        if(q->abort_request)
        {
            ret = -1;
            break;
        }
        //获取消息
        msg1 = q->first_msg;
        if(msg1)
        {
            q->first_msg->next = msg1->next;
            msg1->next = NULL;
            if(!q->first_msg)
                q->last_msg = NULL;
            q->nb_message--;
            *msg = *msg1;
            msg1->obj = NULL;
            msg1->next = q->recycle_msg;
            q->recycle_msg = msg1;
            ret = 1;
            break;
        }else if(!block)//采用非阻塞方式
        {
            ret = 0;
            break;
        }else//采用阻塞方式
        {
            //先解锁，等待条件变量唤醒，唤醒后枷锁
            SDL_CondWait(q->cond,q->mutex);
        }

    }
    SDL_UnlockMutex(q->mutex);
    return ret;
}
int msg_queue_put(MessageQueue *q, AVMessage *msg)
{
    int ret;
    SDL_LockMutex(q->mutex);
    ret = msg_queue_put_private(q,msg);
    SDL_UnlockMutex(q->mutex);
    return ret;
}

void msg_init_msg(AVMessage *msg)
{
    memset(msg,0,sizeof(AVMessage));

}
//插入简单消息，只带消息类型，不带参数
void msg_queue_put_simple1(MessageQueue *q,int what)
{
    AVMessage msg;
    msg_init_msg(&msg);
    msg.what = what;
    msg_queue_put(q,&msg);
}
//插入简单消息，只带消息类型，只带1个参数
void msg_queue_put_simple2(MessageQueue *q,int what,int arg1)
{
    AVMessage msg;
    msg_init_msg(&msg);
    msg.what = what;
    msg.arg1 = arg1;
    msg_queue_put(q,&msg);
}
//插入简单消息，只带消息类型，带两个参数
void msg_queue_put_simple3(MessageQueue *q,int what,int arg1,int arg2)
{
    AVMessage msg;
    msg_init_msg(&msg);
    msg.what = what;
    msg.arg1 = arg1;
    msg.arg2 = arg2;
    msg_queue_put(q,&msg);
}
//释放msg的obj资源
void msg_obj_free_l(void *obj)
{
    av_free(obj);
}
//插入消息，带消息类型，带两个参数，带obj
void msg_queue_put_simple4(MessageQueue *q,int what,int arg1,int arg2,void *obj,int obj_len)
{
    AVMessage msg;
    msg_init_msg(&msg);
    msg.what = what;
    msg.arg1 = arg1;
    msg.arg2 = arg2;
    msg.obj = av_malloc(obj_len);
    memcpy(msg.obj,obj,obj_len);
    msg.free_l = msg_obj_free_l;
    msg_queue_put(q,&msg);
}
//消息队列初始化
void msg_queue_init(MessageQueue *q)
{
    memset(q,0,sizeof(MessageQueue));
    q->mutex = SDL_CreateMutex();
    q->cond = SDL_CreateCond();
    q->abort_request = 1;
}
//消息队列flush,清空所有消息,放入回收链表
void msg_queue_flush(MessageQueue *q)
{
    AVMessage *msg,*msg1;
    SDL_LockMutex(q->mutex);
    for(msg = q->first_msg;msg!=NULL;msg = msg1)
    {
        msg1 = msg->next;
        msg->next = q->recycle_msg;
        q->recycle_msg = msg;
    }
    q->first_msg = NULL;
    q->last_msg = NULL;
    SDL_UnlockMutex(q->mutex);
}
//消息销毁
void msg_queue_destroy(MessageQueue *q)
{
    msg_queue_flush(q);
    SDL_LockMutex(q->mutex);
    while(q->recycle_msg)
    {
        AVMessage *msg = q->recycle_msg;
        if(msg)
            q->recycle_msg = msg->next;
        msg_free_res(msg);
        av_free(&msg);
    }
    SDL_UnlockMutex(q->mutex);
    SDL_DestroyMutex(q->mutex);
    SDL_DestroyCond(q->cond);
}
//消息队列终止
void msg_queue_abort(MessageQueue *q)
{
    SDL_LockMutex(q->mutex);
    q->abort_request = 1;
    SDL_CondSignal(q->cond);
    SDL_UnlockMutex(q->mutex);
}
//启用消息队列
void msg_queue_start(MessageQueue *q)
{
    SDL_LockMutex(q->mutex);
    q->abort_request = 0;
    //插入消息
    AVMessage msg;
    msg_init_msg(&msg);
    msg.what = FFP_MSG_FLUSH;
    msg_queue_put_private(q,&msg);
    SDL_UnlockMutex(q->mutex);
}

//消息删除 把队列同一消息类型的消息全删除掉
void msg_queue_remove(MessageQueue *q,int what)
{
    AVMessage **p_msg,*msg,*last_msg;
    SDL_LockMutex(q->mutex);
    last_msg = q->first_msg;
    if(!q->abort_request && q->first_msg)
    {
        p_msg = &q->first_msg;
        while(*p_msg)
        {
            msg = *p_msg;
            if(msg->what == what)
            {
                *p_msg = msg->next;
                msg_free_res(msg); //释放obj
                msg->next = q->recycle_msg;
                q->recycle_msg = msg;
                q->nb_message--;
            }else
            {
                //记录最后一个消息
                last_msg = msg;
                p_msg = &msg->next;
            }
        }
        if(q->first_msg)
        {
            q->last_msg = last_msg;
        }else
            q->last_msg = NULL;
    }
    SDL_UnlockMutex(q->mutex);
}
