#ifndef LIBYUVSCALE_H
#define LIBYUVSCALE_H
#include "ijkmediaplayer.h"
extern "C"{
#include "libyuv.h"
}
#include "imagescaler.h"
//typedef struct VideoFrame
//{
//    uint8_t *data[8] = {NULL};         // 类似FFmpeg的buf, 如果是
//    int32_t linesize[8] = {0};
//    int32_t width;
//    int32_t height;
//    int format = AV_PIX_FMT_YUV420P;
//}VideoFrame;
class libyuvScale
{
public:
    libyuvScale();
    ~libyuvScale();
    int I420_to_RGB(const Frame *src_frame,VideoFrame *dst_frame);
//    VideoFrame *temp;
private:
    int u_width_=0;
    int v_width_=0;
    int x_=0;
    int y_=0;

};

#endif // LIBYUVSCALE_H
