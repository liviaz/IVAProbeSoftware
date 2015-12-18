#ifndef PROBEGUI_H
#define PROBEGUI_H

#include <QMainWindow>
#include "daqcontroller.h"
#include "ui_probegui.h"
#include <QThread>
#include <QString>

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
    bool readingSamples;
    bool DAQready;
    QThread *thread_dc;

private slots:
    void on_analogReadBtn_clicked();
    void receiveSample(double sample);
    void on_connectBtn_clicked();
    void enableDataCollection(bool ready);

signals:
    void readAnalogSamples(bool start);
    void connectToDAQ(QString portName);

};

#endif // PROBEGUI_H
