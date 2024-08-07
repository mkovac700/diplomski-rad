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
    // QTimer::singleShot(0, this, [this]() {
    //     ui->widget->setScene(new GLBarSpectrumScene(ui->widget));
    // });

    initializeMenuMedia();

    connect(m_engine, &Engine::processedUSecsChanged, this, &MainWindow::processedUSecsChanged);

    initializeScenes();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeMenuMedia()
{
    connect(ui->actionFile, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->actionStream, &QAction::triggered, this, &MainWindow::openStream); //todo
}

void MainWindow::updateMenuMedia()
{
    ui->actionFile->setChecked(m_mode == Mode::LoadFileMode);
    ui->actionStream->setChecked(m_mode == Mode::StreamMode);

    MAINWINDOW_DEBUG << (m_mode == Mode::LoadFileMode) << (m_mode == Mode::StreamMode);
}

void MainWindow::updateLabelDuration(qint64 duration)
{
    qint64 durationMs = duration / 1000;

    QTime time(0, 0);
    time = time.addMSecs(durationMs);
    QString formattedTime = time.toString("m:ss");

    ui->label_Duration->setText(formattedTime);
}

void MainWindow::updateHorizontalSlider(qint64 maxValue)
{
    ui->horizontalSlider_Position->setMaximum(maxValue);
}

void MainWindow::updateHorizontalSliderPosition(qint64 processedUSecs)
{
    ui->horizontalSlider_Position->setSliderPosition(processedUSecs);
}

void MainWindow::updateLabelSeek(qint64 uSecs)
{
    qint64 mSecs = uSecs / 1000;

    QTime time(0, 0);
    time = time.addMSecs(mSecs);
    QString formattedTime = time.toString("m:ss");

    ui->label_Seek->setText(formattedTime);
}

void MainWindow::initializeScenes()
{
    glScenes.append(new GLWaveformScene(ui->widget));
    glScenes.append(new GLBarSpectrumScene(ui->widget));

    QTimer::singleShot(0, this, [this]() { ui->widget->setScene(glScenes[0]); });
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

// void MainWindow::on_pushButton_StartStop_clicked()
// {
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
// }

void MainWindow::on_pushButton_PlayPause_clicked()
{
    if (playing) {
        m_engine->suspend();
        ui->pushButton_PlayPause->setToolTip(tr("Play"));
        ui->pushButton_PlayPause->setIcon(QIcon(":/icons/icons8-play-96-2.png"));
    } else {
        m_engine->startPlayback();
        ui->pushButton_PlayPause->setToolTip(tr("Pause"));
        ui->pushButton_PlayPause->setIcon(QIcon(":/icons/icons8-pause-96-2.png"));
    }

    playing = !playing;
}

void MainWindow::on_pushButton_stop_clicked()
{
    ui->pushButton_PlayPause->setToolTip(tr("Play"));

    ui->pushButton_PlayPause->setIcon(QIcon(":/icons/icons8-play-96-2.png"));

    m_engine->stopPlayback();
    playing = false;
}

void MainWindow::openFile()
{
    setMode(Mode::LoadFileMode);

    updateLabelDuration(0);

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

    qDebug() << "buffer duration (us): " << m_engine->bufferDuration();

    updateLabelDuration(m_engine->bufferDuration());
    updateHorizontalSlider(m_engine->bufferDuration());
}

void MainWindow::openStream()
{
    setMode(Mode::StreamMode);
    updateLabelDuration(0);
}

void MainWindow::processedUSecsChanged(qint64 processedUSecs)
{
    updateHorizontalSliderPosition(processedUSecs);
    updateLabelSeek(processedUSecs);
}

void MainWindow::on_pushButton_PreviousScene_clicked()
{
    if (currentGLScene == 0) {
        currentGLScene = glScenes.size() - 1;
    } else {
        currentGLScene--;
    }

    ui->widget->setScene(glScenes[currentGLScene]);
    ui->label_CurrentScene->setText(glScenes[currentGLScene]->getName());
}

void MainWindow::on_pushButton_NextScene_clicked()
{
    if (currentGLScene == glScenes.size() - 1) {
        currentGLScene = 0;
    } else {
        currentGLScene++;
    }

    ui->widget->setScene(glScenes[currentGLScene]);
    ui->label_CurrentScene->setText(glScenes[currentGLScene]->getName());
}
