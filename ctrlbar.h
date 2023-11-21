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

    void on_listVisible_clicked();

    void on_cut_clicked();

    void on_quickForward_clicked();

    void on_quickBackward_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);
private:
    Ui::CtrlBar *ui;
//    bool full_screen = false;
    bool _listVisible = true;
signals:
    void SigPlayOrPause();
    void SigStop();
    void setListVisible(bool);
};

#endif // CTRLBAR_H
