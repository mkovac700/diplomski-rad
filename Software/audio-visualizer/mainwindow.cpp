#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAudioDevice>
#include <QVariant>

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(m_devices->audioInputs().isEmpty()) QMessageBox::critical(this,"Greška","Nema dostupnih uređaja!");
    loadDevices();
    // initializeAudio(QMediaDevices::defaultAudioInput());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_level_l(qreal value)
{
    int level = qRound(qreal(ui->progressBar_Vol_L->maximum() * value));
    ui->progressBar_Vol_L->setValue(level);
}

void MainWindow::set_level_r(qreal value)
{
    int level = qRound(qreal(ui->progressBar_Vol_R->maximum() * value));
    ui->progressBar_Vol_R->setValue(level);
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

void MainWindow::initializeAudio(const QAudioDevice &inputDeviceInfo)
{
    QAudioFormat format;
    format.setSampleRate(m_sample_rate); //8000
    format.setChannelCount(2);
    format.setSampleFormat(QAudioFormat::Int16);

    m_audioListener.reset(new AudioListener(format));

    connect(m_audioListener.data(), &AudioListener::level_l_Changed, this, &MainWindow::set_level_l);
    connect(m_audioListener.data(), &AudioListener::level_r_Changed, this, &MainWindow::set_level_r);

    connect(m_audioListener.data(), &AudioListener::bufferChanged, ui->widget, &GLWidget::setBuffer);

    m_audioInput.reset(new QAudioSource(inputDeviceInfo, format));

    m_audioListener->start();
    m_audioInput->start(m_audioListener.data()); //-> default, pull mode (moguce je implementirati i push mode -> bolja varijanta za real-time)
}

void MainWindow::on_pushButton_StartStop_clicked()
{
    if(m_audioInput.isNull()){ //prvi put -> jos nije inicijaliziran pointer m_audioInput
        ui->pushButton_StartStop->setText("Zaustavi stream");
        initializeAudio(ui->comboBox_AudioIn->currentData().value<QAudioDevice>());
        return;
    }

    switch (m_audioInput->state()) {
    case QtAudio::StoppedState:
        ui->pushButton_StartStop->setText("Zaustavi stream");
        initializeAudio(ui->comboBox_AudioIn->currentData().value<QAudioDevice>());
        break;
    case QtAudio::ActiveState:
        ui->pushButton_StartStop->setText("Započni stream");
        m_audioListener->stop();
        m_audioInput->stop();
        m_audioInput->disconnect(this);

        ui->progressBar_Vol_L->setValue(0);
        ui->progressBar_Vol_R->setValue(0);
    default:
        break;
    }
}


void MainWindow::on_comboBox_SampleRate_currentTextChanged(const QString &text)
{
    int value = text.toInt();

    if(value) m_sample_rate = value;
}


void MainWindow::on_pushButton_OsvjeziUredaje_clicked()
{
    ui->comboBox_AudioIn->clear();
    ui->textEdit_Uredaji->clear();

    loadDevices();
}

