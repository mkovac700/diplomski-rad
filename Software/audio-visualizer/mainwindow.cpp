#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAudioDevice>
#include <QVariant>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadDevices();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadDevices()
{
    const QAudioDevice &defaultInputDevice = QMediaDevices::defaultAudioInput();

    ui->comboBox_AudioIn->addItem(defaultInputDevice.description(), QVariant::fromValue(defaultInputDevice));

    int i = 0;

    ui->textEdit_Uredaji->append(QString("Device: ").append(QString::number(i)));
    ui->textEdit_Uredaji->append(QString("  Name: ").append(defaultInputDevice.description()));
    ui->textEdit_Uredaji->append(QString("  Max Input Channels: ").append(QString::number(defaultInputDevice.maximumChannelCount())));
    ui->textEdit_Uredaji->append(QString("  Min Input Channels: ").append(QString::number(defaultInputDevice.minimumChannelCount())));
    ui->textEdit_Uredaji->append(QString("  Max Sample Rate: ").append(QString::number(defaultInputDevice.maximumSampleRate())));
    ui->textEdit_Uredaji->append(QString("  Min Sample Rate: ").append(QString::number(defaultInputDevice.minimumSampleRate())));
    ui->textEdit_Uredaji->append(QString("  Current Sample Rate: ").append(QString::number(defaultInputDevice.preferredFormat().sampleRate())));
    ui->textEdit_Uredaji->append(QString("  Bytes Per Sample: ").append(QString::number(defaultInputDevice.preferredFormat().bytesPerSample())));

    for(auto &inputDevice : m_devices->audioInputs()){
        if(inputDevice != defaultInputDevice){
            ui->comboBox_AudioIn->addItem(inputDevice.description(), QVariant::fromValue(inputDevice));

            ui->textEdit_Uredaji->append(QString("Device: ").append(QString::number(++i)));
            ui->textEdit_Uredaji->append(QString("  Name: ").append(inputDevice.description()));
            ui->textEdit_Uredaji->append(QString("  Max Input Channels: ").append(QString::number(inputDevice.maximumChannelCount())));
            ui->textEdit_Uredaji->append(QString("  Min Input Channels: ").append(QString::number(inputDevice.minimumChannelCount())));
            ui->textEdit_Uredaji->append(QString("  Max Sample Rate: ").append(QString::number(inputDevice.maximumSampleRate())));
            ui->textEdit_Uredaji->append(QString("  Min Sample Rate: ").append(QString::number(inputDevice.minimumSampleRate())));
            ui->textEdit_Uredaji->append(QString("  Current Sample Rate: ").append(QString::number(inputDevice.preferredFormat().sampleRate())));
            ui->textEdit_Uredaji->append(QString("  Bytes Per Sample: ").append(QString::number(inputDevice.preferredFormat().bytesPerSample())));
        }
    }
}
