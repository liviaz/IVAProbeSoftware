#include "daqcontroller.h"
#include <QDebug>
#include <QThread>
#include "QLibrary"
#include <QTimer>
#include "DAQmxANSICDev/include/NIDAQmx.h"


// constructor
DAQController::DAQController(QObject *parent) :
    QObject(parent)
{
    errorAI = 0;
    errorAO = 0;
    errorDO = 0;
    taskHandleAI = 0;
    data = (float64 *) calloc(1000,sizeof(float64));
    errBuff = (char *) calloc(2048,sizeof(char));

    // init timer
    readingSamples = false;
    sampleTimer = new QTimer(this);
    connect(sampleTimer, SIGNAL(timeout()), this, SLOT(readSingleSample()));

    // check for ports
    // TODO: finish parsing devNamesBuffer into QString array
    // send to user, put in dropdown list to give connection options
    char devNamesBuffer[1000];
    DAQmxGetSystemInfoAttribute(DAQmx_Sys_DevNames,devNamesBuffer,1000);
    QString devices = QString(devNamesBuffer);
    qDebug() << devices;
}

// destructor
DAQController::~DAQController(){

    sampleTimer->stop();
    delete sampleTimer;
    delete data;
    delete errBuff;

}

void DAQController::initTasks(QString taskChannel)
{
    QString taskAI = QString(taskChannel);
    taskAI.append("/ai0");
    QString taskAO = QString(taskChannel);
    taskAO.append("/ao0");
    QString taskDO = QString(taskChannel);
    taskDO.append("/p0.0");

    // analog in first
    errorAI = DAQmxCreateTask("", &taskHandleAI);
    errorAI = DAQmxCreateAIVoltageChan(taskHandleAI,taskAI.toLatin1().data(),
                                       "", DAQmx_Val_Cfg_Default,-10.0,10.0,
                                       DAQmx_Val_Volts,NULL);
    qDebug() << "errorAI: " << errorAI;
    errorAI = DAQmxCfgSampClkTiming(taskHandleAI,"",1000.0,DAQmx_Val_Rising,
                                    DAQmx_Val_FiniteSamps,2);
    qDebug() << "errorAI: " << errorAI;


    // analog out next
    float64 dataOut[1] = {-5.0};
    qDebug() << "dataOut: " << *dataOut;
    int32 *numSamps = new int32(1);
    qDebug() << "numSamps: " << *numSamps;
    errorAO = DAQmxCreateTask("", &taskHandleAO);
    errorAO = DAQmxCreateAOVoltageChan(taskHandleAO,taskAO.toLatin1().data(),
                                       "", -10.0,10.0,DAQmx_Val_Volts,NULL);
    errorAO = DAQmxWriteAnalogF64(taskHandleAO,1,true,1.0,DAQmx_Val_Default,
                                  dataOut,numSamps,NULL);
    qDebug() << "errorAO: " << errorAO;


}

void DAQController::startStopReading(bool start)
{
    readingSamples = start;
    if (start) {
        sampleTimer->start(5);
    } else {
        sampleTimer->stop();
    }
}

void DAQController::readSingleSample(){

    errorAI = DAQmxReadAnalogF64(taskHandleAI,1,1.0,DAQmx_Val_GroupByChannel,
                                 data,1000,&read,NULL);

    qDebug() << "errorAI: " << errorAI;
    qDebug() << "samples read: " << read;
    qDebug() << "data: " << *data;

    if ((read == 1) && (errorAI == 0)){
        emit sampleReady(*data);
    } else {
        qDebug() << "error! " << errorAI;
    }
}

void DAQController::connectToDAQ(QString taskChannelName)
{
    taskChannel = taskChannelName;

    // init all tasks
    initTasks(taskChannel);

    if (errorAI || errorAO || errorDO){
        emit DAQReady(false);
    } else {
        emit DAQReady(true);
    }

}



