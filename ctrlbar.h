#ifndef CTRLBAR_H
#define CTRLBAR_H

#include <QWidget>

namespace Ui {
class CtrlBar;
}

class CtrlBar : public QWidget
{
    Q_OBJECT

public:
    explicit CtrlBar(QWidget *parent = 0);
    ~CtrlBar();

private slots:
    void on_PlayOrPauseBtn_clicked();

    void on_stopBtn_clicked();
//    void on_fullscrean();

private:
    Ui::CtrlBar *ui;
//    bool full_screen = false;
signals:
    void SigPlayOrPause();
    void SigStop();
};

#endif // CTRLBAR_H
