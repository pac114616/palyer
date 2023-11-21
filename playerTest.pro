#-------------------------------------------------
#
# Project created by QtCreator 2023-10-07T14:03:18
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = playerTest
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    ff_ffplayer.cpp \
    ffmsg_queue.cpp \
    globalhelper.cpp \
    ijkmediaplayer.cpp \
        main.cpp \
        mainwind.cpp \
    ctrlbar.cpp \
    titlebar.cpp \
    playlistwind.cpp \
    displaywind.cpp \
    ff_ffplay_def.cpp \
    libyuvscale.cpp

HEADERS += \
    ff_ffplayer.h \
    ffmsg.h \
    ffmsg_queue.h \
    globalhelper.h \
    ijkmediaplayer.h \
        mainwind.h \
    ctrlbar.h \
    titlebar.h \
    playlistwind.h \
    displaywind.h \
    ff_ffplay_def.h \
    ff_fferror.h \
    imagescaler.h \
    libyuvscale.h

FORMS += \
        mainwind.ui \
    ctrlbar.ui \
    titlebar.ui \
    playlistwind.ui \
    displaywind.ui

RESOURCES += \
    res.qrc


HEADERS += \
        mainwind.h \
    ctrlbar.h \
    titlebar.h \
    playlistwind.h \
    displaywind.h

FORMS += \
        mainwind.ui \
    ctrlbar.ui \
    titlebar.ui \
    playlistwind.ui \
    displaywind.ui

    INCLUDEPATH += $$PWD/libyuv/include
    LIBS += $$PWD/libyuv/lib/libyuv.lib
#    LIBS += "F:/QTProject/armeabi-v7a/libyuv.a"
#    LIBS += -L$$PWD/libyuv/lib/ \
#                -llibyuv
#            "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/Ole32.Lib"
    win32 {
    INCLUDEPATH += $$PWD/ffmpeg-4.2.1-win32-dev/include
    INCLUDEPATH += $$PWD/SDL2/include

    LIBS += $$PWD/ffmpeg-4.2.1-win32-dev/lib/avformat.lib   \
            $$PWD/ffmpeg-4.2.1-win32-dev/lib/avcodec.lib    \
            $$PWD/ffmpeg-4.2.1-win32-dev/lib/avdevice.lib   \
            $$PWD/ffmpeg-4.2.1-win32-dev/lib/avfilter.lib   \
            $$PWD/ffmpeg-4.2.1-win32-dev/lib/avutil.lib     \
            $$PWD/ffmpeg-4.2.1-win32-dev/lib/postproc.lib   \
            $$PWD/ffmpeg-4.2.1-win32-dev/lib/swresample.lib \
            $$PWD/ffmpeg-4.2.1-win32-dev/lib/swscale.lib    \
            $$PWD/SDL2/lib/x86/SDL2.lib \        
            "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x86/Ole32.Lib"
    }
RESOURCES += \
    res.qrc
