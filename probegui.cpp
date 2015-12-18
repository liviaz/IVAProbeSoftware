#include "probegui.h"
#include <QMainWindow>
#include <QString>
#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#include <QtCore/qmath.h>
#include <QThread>
#include <QtGlobal>
#include <QTimer>
#include "daqcontroller.h"


ProbeGUI::ProbeGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProbeGUI)
{

    qRegisterMetaType<PortArray>("PortArray");

    readingSamples = false;
    DAQready = false;
    digitalOn = false;
    ui->setupUi(this);
    ui->analogReadBtn->setEnabled(false);

    // setup DAQ controller and thread
    dc = new DAQController();
    thread_dc = new QThread;
    dc->moveToThread(thread_dc);
    thread_dc->start();

    // connect signals and slots
    connect(thread_dc, SIGNAL(finished()), thread_dc, SLOT(deleteLater()));
    connect(this, SIGNAL(scanForPorts()), dc, SLOT(readPortArray()));
    connect(dc, SIGNAL(portsDetected(PortArray)), this, SLOT(displayDetectedPorts(PortArray)));
    connect(this, SIGNAL(connectToDAQ(QString)), dc, SLOT(connectToDAQ(QString)));
    connect(dc, SIGNAL(DAQReady(bool)), this, SLOT(enableDataCollection(bool)));
    connect(this, SIGNAL(readAnalogSamples(bool)), dc, SLOT(startStopReading(bool)));
    connect(dc, SIGNAL(sampleReady(double)), this, SLOT(receiveSample(double)));
    connect(this, SIGNAL(writeDigitalSample(bool)), dc, SLOT(writeDigitalSample(bool)));

    emit scanForPorts();
}

ProbeGUI::~ProbeGUI()
{
    dc->deleteLater();
    delete ui;
}


void ProbeGUI::on_analogReadBtn_clicked()
{
    if (!DAQready){
        qDebug() << "error: DAQ not ready ...";
        return;
    }

    if (!readingSamples) {
        readingSamples = true;
        emit readAnalogSamples(readingSamples);
        ui->analogReadBtn->setText("stop reading");

    } else {
        readingSamples = false;
        emit readAnalogSamples(readingSamples);
        ui->analogReadBtn->setText("start reading");
    }


}

void ProbeGUI::receiveSample(double sample)
{
    //qDebug() << "sample received: " << sample;
}


void ProbeGUI::on_connectBtn_clicked()
{
    QString currPort = ui->portList->currentText();
    emit connectToDAQ(currPort);
}

void ProbeGUI::enableDataCollection(bool ready)
{
    DAQready = ready;

    if (ready) {
        ui->analogReadBtn->setEnabled(true);
    } else {
        qDebug() << "error : DAQ not starting correctly";
    }

}

void ProbeGUI::displayDetectedPorts(QVector<QString> ports)
{
    ui->portList->clear();
    for (QString currPort : ports){
        ui->portList->addItem(currPort);
    }
}

void ProbeGUI::on_heatingBtn_clicked()
{
   if (digitalOn){
       emit writeDigitalSample(false);
       digitalOn = false;
       ui->heatingBtn->setText("Start heating");
       ui->heatingLabel->setText("Heating: OFF");
   } else {
       emit writeDigitalSample(true);
       digitalOn = true;
       ui->heatingBtn->setText("Stop heating");
       ui->heatingLabel->setText("Heating: ON");
   }
}






