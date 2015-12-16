#ifndef DAQCONTROLLER_H
#define DAQCONTROLLER_H

#include "DAQmxANSICDev/include/NIDAQmx.h"
#include "stdio.h"
#include <QObject>

class DAQController : public QObject
{
    Q_OBJECT

public:
    explicit DAQController(QObject *parent = 0);
    ~DAQController();

private:
    TaskHandle taskHandle;
    int32 errorAI;
    int32 errorAO;
    int32 errorDO;
    int32 read;
    float64 *data;
    char *errBuff;


signals:

public slots:
    void readSamples();

};

#endif // DAQCONTROLLER_H
