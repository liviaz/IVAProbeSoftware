#ifndef PROBEGUI_H
#define PROBEGUI_H

#include <QMainWindow>
#include "daqcontroller.h"
#include "ui_probegui.h"
#include <QThread>
#include <QString>
#include <QVector>

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
    bool digitalOn;
    QThread *thread_dc;
    typedef QVector<QString> PortArray;

private slots:
    void on_analogReadBtn_clicked();
    void on_connectBtn_clicked();
    void on_heatingBtn_clicked();

public slots:
    void receiveSample(double sample);
    void enableDataCollection(bool ready);
    void displayDetectedPorts(QVector<QString> ports);

signals:
    void readAnalogSamples(bool start);
    void connectToDAQ(QString portName);
    void scanForPorts();
    void writeDigitalSample(bool value);
};

#endif // PROBEGUI_H
