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
    ui->setupUi(this);

    // setup DAQ controller and thread
    dc = new DAQController();
    QThread* thread_dc = new QThread;
    dc->moveToThread(thread_dc);

    // connect signals and slots
    connect(this, SIGNAL(readAnalogSamples()), dc, SLOT(readSamples()));

    // start thread
    thread_dc->start();

}

ProbeGUI::~ProbeGUI()
{
    delete dc;
    delete ui;
}

void ProbeGUI::on_pushButton_clicked()
{

    ui->textLabel->setText("SUPPPPP");

}

void ProbeGUI::on_analogReadBtn_clicked()
{
    emit readAnalogSamples();
}
