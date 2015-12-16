#include "daqcontroller.h"
#include <QDebug>
#include <QThread>
#include "QLibrary"
#include "DAQmxANSICDev/include/NIDAQmx.h"


// constructor
DAQController::DAQController(QObject *parent) :
    QObject(parent)
{
    errorAI = 0;
    errorAO = 0;
    errorDO = 0;
    taskHandle = 0;
    data = (float64 *) calloc(1000,sizeof(float64));
    errBuff = (char *) calloc(2048,sizeof(char));

    errorAI = DAQmxCreateTask("", &taskHandle);
    qDebug() << "errorAI: " << errorAI;
    errorAI = DAQmxCreateAIVoltageChan(taskHandle,"Dev3/ai0","",
                                       DAQmx_Val_Cfg_Default,-10.0,10.0,
                                       DAQmx_Val_Volts,NULL);

    qDebug() << "errorAI: " << errorAI;
    errorAI = DAQmxCfgSampClkTiming(taskHandle,"",1000.0,DAQmx_Val_Rising,
                                    DAQmx_Val_FiniteSamps,10);

    qDebug() << "errorAI: " << errorAI;
}

// destructor
DAQController::~DAQController(){

    //errorAI = DAQmxClearTask();

}

void DAQController::readSamples()
{
    errorAI = DAQmxReadAnalogF64(taskHandle,10,1.0,DAQmx_Val_GroupByChannel,
                                 data,1000,&read,NULL);

    qDebug() << "errorAI: " << errorAI;
    qDebug() << "samples read: " << read;
    qDebug() << "data: " << *data;
}



