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
    readingSamples = false;
    DAQready = false;
    ui->setupUi(this);
    ui->analogReadBtn->setEnabled(false);

    // setup DAQ controller and thread
    dc = new DAQController();
    thread_dc = new QThread;
    dc->moveToThread(thread_dc);
    thread_dc->start();

    // connect signals and slots
    connect(this, SIGNAL(readAnalogSamples(bool)), dc, SLOT(startStopReading(bool)));
    connect(dc, SIGNAL(sampleReady(double)), this, SLOT(receiveSample(double)));
    connect(thread_dc, SIGNAL(finished()), thread_dc, SLOT(deleteLater()));
    connect(this, SIGNAL(connectToDAQ(QString)), dc, SLOT(connectToDAQ(QString)));
    connect(dc, SIGNAL(DAQReady(bool)), this, SLOT(enableDataCollection(bool)));


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
    qDebug() << "sample received: " << sample;
}


void ProbeGUI::on_connectBtn_clicked()
{
    emit connectToDAQ(ui->portTextEdit->text());
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
