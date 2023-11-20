#ifndef MAINWIND_H
#define MAINWIND_H
#include <ijkmediaplayer.h>
#include <QMainWindow>

namespace Ui {
class MainWind;
}

class MainWind : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWind(QWidget *parent = 0);
    ~MainWind();
    int msg_loop(void *arg);
    int OutputVideo(const Frame *frame);
    void on_play_or_pause();
//    void OutputVideo(const Frame *frame);
    void on_stop();
private:
    Ui::MainWind *ui;
    IjkMediaPlayer *mp_ = NULL;


private slots:
//    void on_play_or_pause();
};

#endif // MAINWIND_H
