#ifndef DAQCONTROLLER_H
#define DAQCONTROLLER_H

#include "DAQmxANSICDev/include/NIDAQmx.h"
#include "stdio.h"
#include <QObject>
#include <QTimer>
#include <QString>
#include <QVector>

class DAQController : public QObject
{
    Q_OBJECT

public:
    explicit DAQController(QObject *parent = 0);
    ~DAQController();

private:
    TaskHandle taskHandleAI;
    TaskHandle taskHandleAO;
    TaskHandle taskHandleDO;
    QString taskChannel;
    int32 errorAI;
    int32 errorAO;
    int32 errorDO;
    int32 read;
    float64 *data;
    char *errBuff;
    bool readingSamples;
    QTimer *sampleTimer;
    void initTasks(QString taskChannel);

signals:
    void sampleReady(double value);
    void DAQReady(bool ready);
    void portsDetected(QVector<QString> ports);

public slots:
    void startStopReading(bool start);
    void readSingleSample();
    void connectToDAQ(QString taskChannel);
    void readPortArray();
    void writeDigitalSample(bool value);
};

#endif // DAQCONTROLLER_H
