#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAudioDevice>
#include <QVariant>

#include <QMessageBox>

#include <QFileDialog>

#include <glbarspectrumscene.h>
#include <glwaveformscene.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_engine(new Engine(this))
    , m_mode(Mode::NoMode)
{
    ui->setupUi(this);

    if (m_devices->audioInputs().isEmpty())
        QMessageBox::critical(this, "Greška", "Nema dostupnih ulaznih uređaja!");
    else
        loadInputDevices();

    if (m_devices->audioOutputs().isEmpty())
        QMessageBox::critical(this, "Greška", "Nema dostupnih izlaznih uređaja!");
    else
        loadOutputDevices();

    // ui->widget->setScene(new GLWaveformScene(ui->widget));

    //connect(m_engine, &Engine::buffer2Changed, ui->widget, &GLWidget::bufferChanged);
    connect(m_engine, &Engine::buffer2Changed, ui->widget, &GLWidget::handleBufferChanged);
    connect(m_engine,
            QOverload<qint64, qint64, const FrequencySpectrum &>::of(&Engine::spectrumChanged),
            ui->widget,
            QOverload<qint64, qint64, const FrequencySpectrum &>::of(
                &GLWidget::handleSpectrumChanged));
    // connect(m_engine,
    //         QOverload<qint64, qint64, const FrequencySpectrum &>::of(&Engine::spectrumChanged),
    //         ui->widget_2,
    //         QOverload<qint64, qint64, const FrequencySpectrum &>::of(&GLWidget2::spectrumChanged));

    //mora se osigurati da je widget do kraja inicijaliziran prije postavljanja defaultne scene
    // QTimer::singleShot(0, this, [this]() { ui->widget->setScene(new GLWaveformScene(ui->widget)); });
    QTimer::singleShot(0, this, [this]() {
        ui->widget->setScene(new GLBarSpectrumScene(ui->widget));
    });

    initializeMenuMedia();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeMenuMedia()
{
    connect(ui->actionFile, &QAction::triggered, this, &MainWindow::openFile);
    //connect(ui->actionStream, &QAction::triggered, this, &QMainWindow::openStream); //todo
}

void MainWindow::updateMenuMedia()
{
    ui->actionFile->setChecked(m_mode == Mode::LoadFileMode);
    ui->actionStream->setChecked(m_mode == Mode::StreamMode);
}

void MainWindow::loadInputDevices()
{
    const QAudioDevice &defaultInputDevice = QMediaDevices::defaultAudioInput();

    QAction *action = new QAction(tr("Default"), this);
    action->setCheckable(true);
    action->setChecked(true);
    action->setData(QVariant::fromValue(defaultInputDevice));
    ui->menuAudioIn->addAction(action);

    for (auto &inputDevice : m_devices->audioInputs()) {
        QAction *action = new QAction(inputDevice.description(), this);
        action->setCheckable(true);
        action->setData(QVariant::fromValue(inputDevice));
        ui->menuAudioIn->addAction(action);
    }
}

void MainWindow::initializeInputAudio(const QAudioDevice &inputDeviceInfo)
{
    QAudioFormat format;
    format.setSampleRate(inputDeviceInfo.preferredFormat().sampleRate()); //8000 //m_sample_rate
    format.setChannelCount(2);
    format.setSampleFormat(QAudioFormat::Int16);

    m_audioListener.reset(new AudioListener(format));

    //connect(m_audioListener.data(), &AudioListener::bufferChanged, ui->widget, &GLWidget::setBuffer);
    // connect(m_audioListener.data(),
    //         &AudioListener::bufferChanged,
    //         ui->widget_2,
    //         &GLWidget2::setBuffer);

    m_audioInput.reset(new QAudioSource(inputDeviceInfo, format));

    m_audioListener->start();
    m_audioInput->start(m_audioListener.data()); //-> default, pull mode (moguce je implementirati i push mode -> bolja varijanta za real-time)
}

void MainWindow::loadOutputDevices()
{
    const QAudioDevice &defaultOutputDevice = QMediaDevices::defaultAudioOutput();

    QAction *action = new QAction(tr("Default"), this);
    action->setCheckable(true);
    action->setChecked(true);
    action->setData(QVariant::fromValue(defaultOutputDevice));
    ui->menuAudioOut->addAction(action);

    for (auto &outputDevice : m_devices->audioOutputs()) {
        QAction *action = new QAction(outputDevice.description(), this);
        action->setCheckable(true);
        action->setData(QVariant::fromValue(outputDevice));
        ui->menuAudioOut->addAction(action);
    }
}

void MainWindow::initializeOutputAudio(const QAudioDevice &outputDevice)
{
    m_engine->setAudioOutputDevice(outputDevice);
}

void MainWindow::setMode(Mode mode)
{
    m_mode = mode;
    updateMenuMedia();
}

void MainWindow::on_pushButton_StartStop_clicked()
{
    // if(m_audioInput.isNull()){ //prvi put -> jos nije inicijaliziran pointer m_audioInput
    //     ui->pushButton_StartStop->setText("Zaustavi stream");
    //     initializeInputAudio(ui->comboBox_AudioIn->currentData().value<QAudioDevice>());
    //     return;
    // }

    // switch (m_audioInput->state()) {
    // case QtAudio::StoppedState:
    //     ui->pushButton_StartStop->setText("Zaustavi stream");
    //     initializeInputAudio(ui->comboBox_AudioIn->currentData().value<QAudioDevice>());
    //     break;
    // case QtAudio::ActiveState:
    //     ui->pushButton_StartStop->setText("Započni stream");
    //     m_audioListener->stop();
    //     m_audioInput->stop();
    //     m_audioInput->disconnect(this);
    // default:
    //     break;
    // }
}

void MainWindow::on_pushButton_PlayPause_clicked()
{
    if (playing) {
        m_engine->suspend();
        ui->pushButton_PlayPause->setText(tr("Play"));
    } else {
        m_engine->startPlayback();
        ui->pushButton_PlayPause->setText(tr("Pause"));
    }

    playing = !playing;
}

void MainWindow::on_pushButton_stop_clicked()
{
    ui->pushButton_PlayPause->setText(tr("Play"));

    m_engine->stopPlayback();
    playing = false;
}

void MainWindow::on_pushButton_openFile_clicked()
{
    m_currentFile = QFileDialog::getOpenFileName(this,
                                                 tr("Open File"),
                                                 QStandardPaths::writableLocation(
                                                     QStandardPaths::MusicLocation),
                                                 tr("Audio Files (*.wav)"));

    qDebug() << m_currentFile;

    ui->statusbar->showMessage(QString("Datoteka: ").append(m_currentFile.split("/").last()));

    if (m_currentFile.isEmpty())
        return;

    m_engine->reset();

    m_engine->loadFile(m_currentFile);
}

void MainWindow::openFile()
{
    setMode(Mode::LoadFileMode);

    m_currentFile = QFileDialog::getOpenFileName(this,
                                                 tr("Open File"),
                                                 QStandardPaths::writableLocation(
                                                     QStandardPaths::MusicLocation),
                                                 tr("Audio Files (*.wav)"));

    qDebug() << m_currentFile;

    ui->statusbar->showMessage(QString("Datoteka: ").append(m_currentFile.split("/").last()));

    if (m_currentFile.isEmpty())
        return;

    m_engine->reset();

    m_engine->loadFile(m_currentFile);
}
