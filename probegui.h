#ifndef PROBEGUI_H
#define PROBEGUI_H

#include <QMainWindow>
#include "daqcontroller.h"
#include "ui_probegui.h"

namespace Ui {
class ProbeGUI;
}

class ProbeGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProbeGUI(QWidget *parent = 0);
    ~ProbeGUI();

private:
    Ui::ProbeGUI *ui;
    DAQController *dc;

private slots:
    void on_pushButton_clicked();
    void on_analogReadBtn_clicked();

signals:
    void readAnalogSamples();

};

#endif // PROBEGUI_H
