#include "libyuvscale.h"

libyuvScale::libyuvScale()
{

}

libyuvScale::~libyuvScale()
{
}

int libyuvScale::I420_to_RGB(const Frame *src_frame, VideoFrame *dst_frame)
{
//    int I420ToRGB24(const uint8_t* src_y,
//                    int src_stride_y,
//                    const uint8_t* src_u,
//                    int src_stride_u,
//                    const uint8_t* src_v,
//                    int src_stride_v,
//                    uint8_t* dst_rgb24,
//                    int dst_stride_rgb24,
//                    int width,
//                    int height);
//    int ret = libyuv::I420ToABGR(src_frame->frame->data[0],src_frame->frame->linesize[0],
//                       src_frame->frame->data[1],src_frame->frame->linesize[1],
//                       src_frame->frame->data[2],src_frame->frame->linesize[2],
//                        dst_frame->data[0],dst_frame->linesize[0],
//                        dst_frame->width,dst_frame->height);//libyuv::
    int ret = -1;
//    temp->width = src_frame->frame->width;
//    temp->height = src_frame->frame->height;
//    temp->width &= 0xfffc;
//    temp->height &= 0xfffc;
//    temp->data[0]=(uint8_t*)av_malloc(temp->width*temp->height);
//    temp->data[1]=(uint8_t*)av_malloc(temp->width*temp->height/4);
//    temp->data[1]=(uint8_t*)av_malloc(temp->width*temp->height/4);
//    ret =  libyuv::I420Scale(src_frame->frame->data[0],
//                              src_frame->frame->linesize[0],
//                              src_frame->frame->data[1],
//                              src_frame->frame->linesize[1],
//                              src_frame->frame->data[2],
//                              src_frame->frame->linesize[2],
//                              src_frame->frame->width,
//                              src_frame->frame->height,
//                              temp->data[0],
//                              temp->width,
//                              temp->data[1],
//                              temp->width/2,
//                              temp->data[2],
//                              temp->width/2,
//                              temp->width,
//                              temp->height,
//                              libyuv::kFilterBox);

    ret = libyuv::I420ToRGB24(src_frame->frame->data[0],src_frame->frame->linesize[0],
                              src_frame->frame->data[1],src_frame->frame->linesize[1],
                              src_frame->frame->data[2],src_frame->frame->linesize[2],
                              dst_frame->data[0],dst_frame->linesize[0],
                              dst_frame->width,dst_frame->height);

//    ret = libyuv::
    return ret;
}
