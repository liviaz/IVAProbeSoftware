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
    taskDO.append("/port0/line0");

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


    errorDO = DAQmxCreateTask("", &taskHandleDO);
    errorDO = DAQmxCreateDOChan(taskHandleDO,taskDO.toLatin1().data(),
                                "",DAQmx_Val_ChanForAllLines);
    qDebug() << "errorDO: " << errorDO;
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

    qDebug() << "data: " << *data;

    if ((read == 1) && (errorAI == 0)){
        emit sampleReady(*data);
    } else {
        qDebug() << "error! " << errorAI;
    }
}

void DAQController::writeDigitalSample(bool value){

    qDebug() << "value to write: " << value;

    uInt8 dataOut[1] = {0};
    if (value){
        dataOut[0] = 255;
    }

    qDebug() << "dataOut: " << *dataOut;
    int32 *numSamps = new int32(1);
    errorDO = DAQmxWriteDigitalU8(taskHandleDO,1,1,1.0,DAQmx_Val_GroupByChannel,
                                  dataOut,numSamps,NULL);
    qDebug() << "errorDO: " << errorDO;
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

void DAQController::readPortArray(){

    QVector<QString> ports(QVector<QString>(0));
    char devNamesBuffer[1000];
    DAQmxGetSystemInfoAttribute(DAQmx_Sys_DevNames,devNamesBuffer,1000);
    char *currChar = devNamesBuffer;
    QString currPort = "";

    while (*currChar){
        if (*currChar == ','){
            ports.append(currPort);
            currPort.clear();
            currChar++;
        } else {
            currPort.append(*currChar);
        }
        currChar++;
    }

    ports.append(currPort);
    emit portsDetected(ports);

    char doNamesBuffer[1000];
    DAQmxGetDeviceAttribute("Dev3",DAQmx_Dev_DI_Lines, doNamesBuffer);

    qDebug() << "digital lines: " << doNamesBuffer;
}

