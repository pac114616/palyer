#include "displaywind.h"
#include "ui_displaywind.h"
#include <QPainter>
//#include "libyuv.h"
DisplayWind::DisplayWind(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DisplayWind)
{
    ui->setupUi(this);
    memset(&dst_video_frame_, sizeof(VideoFrame), 0);
}

DisplayWind::~DisplayWind()
{
    delete ui;
    if(dst_video_frame_.data[0])
        free(dst_video_frame_.data[0]);
    if(img_scaler_) {
        delete img_scaler_;
        img_scaler_ = NULL;
    }
}
void DisplayWind::DeInit()
{
    if(dst_video_frame_.data[0])
        free(dst_video_frame_.data[0]);
    if(libyuv_scaler_)
    {
        delete libyuv_scaler_;
        libyuv_scaler_ = NULL;
    }
    if(img_scaler_) {
        delete img_scaler_;
        img_scaler_ = NULL;
    }
}
int DisplayWind::Draw(const Frame *frame)
{
    QMutexLocker locker(&m_mutex);
    if(!img_scaler_||is_display_size_change_)
    {
        if(img_scaler_)
            DeInit();
        int win_width = width();
        int win_height = height();
        video_height = frame->height;
        video_width = frame->width;
        img_scaler_ = new ImageScaler();
        double video_aspect_ratio = frame->width *1.0 /frame->height;
        double win_aspect_ratio = win_width *1.0 /win_height;
        if(win_aspect_ratio > video_aspect_ratio) {
            //此时应该是调整x的起始位置，以高度为基准
            img_height = win_height;
            img_height &= 0xfffc;
            if(img_height %2 != 0) {
                img_height -= 1;
            }

            img_width = img_height*video_aspect_ratio;
            img_width &= 0xfffc;
            if(img_width %2 != 0) {
                img_width -= 1;
            }
            y_ = 0;
            x_ = (win_width - img_width) / 2;
        } else {
            //此时应该是调整y的起始位置，以宽度为基准
            img_width = win_width;
            img_width &= 0xfffc;
            if(img_width %2 != 0) {
                img_width -= 1;
            }
            img_height = img_width / video_aspect_ratio;
            img_height &= 0xfffc;
            if(img_height %2 != 0) {
                img_height -= 1;
            }
            x_ = 0;
            y_ = (win_height - img_height) / 2;
        }
        img_scaler_->Init(video_width, video_height, frame->format,
                          img_width, img_height, AV_PIX_FMT_RGB24);
        memset(&dst_video_frame_, 0, sizeof(VideoFrame));
        dst_video_frame_.width = img_width;
        dst_video_frame_.height = img_height;
        dst_video_frame_.format = AV_PIX_FMT_RGB24;
        dst_video_frame_.data[0] = (uint8_t*)malloc(img_width * img_height * 3);
        dst_video_frame_.linesize[0] = img_width * 3; // 每行的字节数
    }
    img_scaler_->Scale3(frame, &dst_video_frame_);

    QImage imageTmp =  QImage((uint8_t *)dst_video_frame_.data[0],
            img_width, img_height, QImage::Format_RGB888);
    img = imageTmp.copy(0, 0, img_width, img_height);

    update();
//    repaint();
    return 0;
}

//int DisplayWind::Draw(const Frame *frame)
//{
//    if(!libyuv_scaler_||is_display_size_change_)
//    {
//        if(!libyuv_scaler_)
//            DeInit();

//        libyuv_scaler_ = new libyuvScale();
//        is_display_size_change_ = false;
//        win_width = width();
//        win_height = height();
//        video_height = frame->height;
//        video_width = frame->width;
//        //video宽高比
//        double video_aspect_ratio = frame->width *1.0 /frame->height;
//        //从窗口宽高比
//        double win_aspect_ratio = win_width *1.0 /win_height;
//        if(video_aspect_ratio==win_aspect_ratio)
//        {
//            video_width = win_width;
////            video_width &= 0xfffc;
//            video_width &= 0xfffe;
//            video_height = win_height;
////            video_height &= 0xfffc;
//            video_height &= 0xfffe;
//            x_=0;
//            y_=0;
//        }
//        else if(video_aspect_ratio>win_aspect_ratio)
//        {
//            video_width = win_width;
//            video_width &= 0xfffc;
////            video_width &= 0xfffe;
//            video_height = (int)floor(video_width *1.0/video_aspect_ratio); //向下取整
//            video_height &= 0xfffc;
////            video_height &= 0xfffe;
//            x_=0;
//            y_= (win_height-video_height)/2;
//        }else
//        {
//            video_height = win_height;
//            video_height &= 0xfffc;
////            video_height &= 0xfffe;
//            video_width = (int)ceil(video_height*video_aspect_ratio);//向上取整
//            video_width &= 0xfffc;
////            video_width &= 0xfffe;
//            x_= (win_width-video_width)/2;
//            y_=0;
//        }
//        //还要对YUV数据进行缩放
//    //    int ret = libyuv::I420Scale(frame->frame->data, frame->width, frame->height,  // 源YUV数据和大小
//    //                               dst_yuv_data, dst_width, dst_height,  // 目标YUV数据和大小
//    //                               libyuv::kFilterBox);
//        //缩放后
////        UWidth = (int)ceil(video_width*1.0/2);
////        VWidth = (int)ceil(video_width*1.0/2);
//        UWidth = video_width;
//        VWidth = video_height;
////        UWidth &= 0xfffc;
////        VWidth &= 0xfffc;
////        frame->frame->width &= 0xfffc;
////        frame->frame->height &= 0xfffc;

//        memset(&dst_video_frame_, 0, sizeof(VideoFrame));
//        dst_video_frame_.width = video_width;
//        dst_video_frame_.height = video_height;
//        dst_video_frame_.format = AV_PIX_FMT_RGB24;
//        dst_video_frame_.data[0] =  (uint8_t*)av_malloc(video_height*video_width*3);
//        dst_video_frame_.linesize[0] = video_width*3;
//    }
//    int ret = libyuv_scaler_->I420_to_RGB(frame,&dst_video_frame_);
//    if(ret<0)
//        printf("YUVtoRGBfail\n");
////    int ret = libyuv::ConvertToARGB(frame->frame->data, frame->frame->linesize,
////                      dst_video_frame_.data, dst_video_frame_.linesize,
////                      0, 0,
////                      video_width, video_height,
////                      dst_video_frame_.width, dst_video_frame_.height,
////                      0,
////                      libyuv::FOURCC_BPP_I420);//FOURCC_BPP_I420
////    QMutexLocker locker(&m_mutex);

//        QImage imageTmp =  QImage((uint8_t *)dst_video_frame_.data[0],
//                video_width, video_height, QImage::Format_RGB888);

//        img = imageTmp.copy(0, 0, video_width, video_width);

//        update();
//    //    repaint();
//        return 0;
//    }


void DisplayWind::paintEvent(QPaintEvent *)
{
    QMutexLocker locker(&m_mutex);
    if (img.isNull()) {
        return;
    }
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    //    //    p.translate(X, Y);
    //    //    p.drawImage(QRect(0, 0, W, H), img);
    QRect rect = QRect(x_, y_, img.width(), img.height());
//    painter.drawImage(rect, img);
    //        qDebug() << rect << ", win_w:" << this->width() << ", h:" << this->height();
    painter.drawImage(rect, img.scaled(img.width(), img.height()));
}

void DisplayWind::resizeEvent(QResizeEvent *event)
{
    QMutexLocker locker(&m_mutex);
    if(win_width != width() || win_height != height()) {
        //        DeInit();       // 释放尺寸缩放资源，等下一次draw的时候重新初始化
        //        win_width = width();
        //        win_height = height();
        is_display_size_change_ = true;
    }

}

