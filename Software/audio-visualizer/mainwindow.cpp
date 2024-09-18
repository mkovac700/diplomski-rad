#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAudioDevice>
#include <QVariant>

#include <QMessageBox>

#include <QFileDialog>

#include <gl3dspectrogramscene.h>
#include <glbarspectrumscene.h>
#include <glwaveformscene.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_engine(new Engine(this))
    , m_mode(Mode::NoMode)
    , m_settingsDialog(new SettingsDialog(this))
    , m_statusLabel(new QLabel(this))
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
            QOverload<qint64, qint64, const FrequencySpectrum &, int>::of(&Engine::spectrumChanged),
            ui->widget,
            QOverload<qint64, qint64, const FrequencySpectrum &, int>::of(
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

    connect(m_engine, &Engine::processingComplete, this, &MainWindow::processingComplete);

    initializeScenes();

    ui->statusbar->addPermanentWidget(m_statusLabel, 100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeMenuMedia()
{
    connect(ui->actionFile, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->actionStream, &QAction::triggered, this, &MainWindow::openStream);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::showSettingsDialog);
}

void MainWindow::updateMenuMedia()
{
    ui->actionFile->setChecked(m_mode == Mode::LoadFileMode);
    ui->actionStream->setChecked(m_mode == Mode::StreamMode);

    MAINWINDOW_DEBUG << (m_mode == Mode::LoadFileMode) << (m_mode == Mode::StreamMode);
}

void MainWindow::updateStatusBar()
{
    QString message;

    if (m_mode == Mode::StreamMode)
        message.append(tr("Strujanje"));
    else if (m_mode == Mode::LoadFileMode)
        message.append(QString(tr("Datoteka: ")).append(m_currentFile.split("/").last()));

    int sampleRate = m_engine->format().sampleRate();
    int fftSize = EngineSettings::instance().fftSize();

    message.append(" | ");

    message.append(QString(tr("Brzina uzorkovanja: "))).append(QString::number(sampleRate));

    message.append(" | ");

    message.append(QString(tr("FFT veličina: "))).append(QString::number(fftSize));

    m_statusLabel->setText(message);

    //ui->statusbar->showMessage(message);
}

void MainWindow::initializeScenes()
{
    glScenes.append(new GLWaveformScene(ui->widget));
    glScenes.append(new GLBarSpectrumScene(ui->widget));
    glScenes.append(new GL3DSpectrogramScene(ui->widget));

    QTimer::singleShot(0, this, [this]() {
        ui->widget->setScene(glScenes[0]);
        ui->label_CurrentScene->setText(glScenes[0]->getName());
    });
}

void MainWindow::loadInputDevices()
{
    const QAudioDevice &defaultInputDevice = QMediaDevices::defaultAudioInput();

    QAction *action = new QAction(tr("Default"), ui->menuAudioIn);
    action->setCheckable(true);
    action->setChecked(true);
    action->setData(QVariant::fromValue(defaultInputDevice));
    ui->menuAudioIn->addAction(action);

    connect(action, &QAction::triggered, this, &MainWindow::changeAudioIn);

    for (auto &inputDevice : m_devices->audioInputs()) {
        QAction *action = new QAction(inputDevice.description(), ui->menuAudioIn);
        action->setCheckable(true);
        action->setData(QVariant::fromValue(inputDevice));
        ui->menuAudioIn->addAction(action);

        connect(action, &QAction::triggered, this, &MainWindow::changeAudioIn);
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

    QAction *action = new QAction(tr("Default"), ui->menuAudioOut);
    action->setCheckable(true);
    action->setChecked(true);
    action->setData(QVariant::fromValue(defaultOutputDevice));
    ui->menuAudioOut->addAction(action);

    connect(action, &QAction::triggered, this, &MainWindow::changeAudioOut);

    for (auto &outputDevice : m_devices->audioOutputs()) {
        QAction *action = new QAction(outputDevice.description(), ui->menuAudioOut);
        action->setCheckable(true);
        action->setData(QVariant::fromValue(outputDevice));
        ui->menuAudioOut->addAction(action);

        connect(action, &QAction::triggered, this, &MainWindow::changeAudioOut);
    }
}

void MainWindow::changeAudioIn()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action && action->isCheckable()) {
        // Poništi sve ostale QAction unutar istog QMenu
        QMenu *menu = qobject_cast<QMenu *>(action->parent());
        for (QAction *act : menu->actions()) {
            if (act != action) {
                act->setChecked(false);
            }
        }
        action->setChecked(true);
        m_engine->setAudioInputDevice(action->data().value<QAudioDevice>());
    }
}

void MainWindow::changeAudioOut()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action && action->isCheckable()) {
        // Poništi sve ostale QAction unutar istog QMenu
        QMenu *menu = qobject_cast<QMenu *>(action->parent());
        for (QAction *act : menu->actions()) {
            if (act != action) {
                act->setChecked(false);
            }
        }
        action->setChecked(true);
        m_engine->setAudioOutputDevice(action->data().value<QAudioDevice>());
    }
}

void MainWindow::showSettingsDialog()
{
    m_settingsDialog->exec();
    if (m_settingsDialog->result() == QDialog::Accepted) {
        qDebug() << "settings dialog: accepted";

        WindowFunction wf = EngineSettings::instance().windowFunction();
        int fftSize = EngineSettings::instance().fftSize();
        int intervalMs = EngineSettings::instance().updateIntervalMs();

        if (wf != m_settingsDialog->windowFunction() || fftSize != m_settingsDialog->fftSize()) {
            EngineSettings::instance().setWindowFunction(m_settingsDialog->windowFunction());
            EngineSettings::instance().setFFTSize(m_settingsDialog->fftSize());
            QMessageBox::warning(this,
                                 tr("Upozorenje"),
                                 tr("Unijeli ste postavke koje zahtijevaju restart pogona. To može "
                                    "uzrokovati kratkotrajni prekid rada."));
            //reset engine
            m_engine->resetSoft();
        }

        // if (fftSize != m_settingsDialog->fftSize()) {
        //     EngineSettings::instance().setFFTSize(m_settingsDialog->fftSize());
        //     QMessageBox::warning(this,
        //                          tr("Upozorenje"),
        //                          tr("Unijeli ste postavke koje zahtijevaju restart pogona. To može "
        //                             "uzrokovati kratkotrajni prekid rada."));
        //     //reset engine
        //     m_engine->resetSoft();
        // }

        if (intervalMs != m_settingsDialog->updateIntervalMs()) {
            EngineSettings::instance().setUpdateIntervalMs(m_settingsDialog->updateIntervalMs());
            restartEngineTimer(); //to update interval ms
        }

        // EngineSettings::instance().setWindowFunction(m_settingsDialog->windowFunction());
        // EngineSettings::instance().setFFTSize(m_settingsDialog->fftSize());
        // EngineSettings::instance().setUpdateIntervalMs(m_settingsDialog->updateIntervalMs());

        GraphicsSettings::instance().setIsLogScale(m_settingsDialog->isLogScale());
        GraphicsSettings::instance().setLogFactor(m_settingsDialog->logFactor());
        GraphicsSettings::instance().setNumLines(m_settingsDialog->numLines());
        GraphicsSettings::instance().setSpacingX(m_settingsDialog->spacingX());
        GraphicsSettings::instance().setSpacingZ(m_settingsDialog->spacingZ());

        GraphicsSettings::instance().setApplyWindow(m_settingsDialog->applyWindow());
        GraphicsSettings::instance().setDrawGrid(m_settingsDialog->drawGrid());

        GraphicsSettings::instance().setMinFreq(m_settingsDialog->minFreq());
        GraphicsSettings::instance().setMaxFreq(m_settingsDialog->maxFreq());
        GraphicsSettings::instance().setBandWidth(m_settingsDialog->bandWidth());
        GraphicsSettings::instance().setGridStepHz(m_settingsDialog->gridStepHz());

        ui->widget->reinitialize();

        updateStatusBar();
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

void MainWindow::restartEngineTimer()
{
    m_engine->setUpdateInterval(EngineSettings::instance().updateIntervalMs());

    if (playing) {
        m_engine->suspend();
        if (m_mode == Mode::LoadFileMode)
            m_engine->startPlayback();
        else if (m_mode == Mode::StreamMode)
            m_engine->startStream();
    }
}

void MainWindow::stopEngine()
{
    if (playing) {
        ui->pushButton_PlayPause->setToolTip(tr("Play"));

        ui->pushButton_PlayPause->setIcon(QIcon(":/icons/icons8-play-96-2.png"));

        if (m_mode == Mode::LoadFileMode)
            m_engine->stopPlayback();
        else if (m_mode == Mode::StreamMode)
            m_engine->stopStream();

        playing = false;
    }
}

void MainWindow::on_pushButton_PlayPause_clicked()
{
    if (playing) {
        m_engine->suspend();
        ui->pushButton_PlayPause->setToolTip(tr("Play"));
        ui->pushButton_PlayPause->setIcon(QIcon(":/icons/icons8-play-96-2.png"));
    } else {
        if (m_mode == Mode::LoadFileMode)
            m_engine->startPlayback();
        else if (m_mode == Mode::StreamMode)
            m_engine->startStream();
        else
            return;
        ui->pushButton_PlayPause->setToolTip(tr("Pause"));
        ui->pushButton_PlayPause->setIcon(QIcon(":/icons/icons8-pause-96-2.png"));
    }

    playing = !playing;
}

void MainWindow::on_pushButton_stop_clicked()
{
    stopEngine();
}

void MainWindow::openFile()
{
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Open File"),
                                                QStandardPaths::writableLocation(
                                                    QStandardPaths::MusicLocation),
                                                tr("Audio Files (*.wav)"));

    qDebug() << file;

    if (file.isEmpty()) {
        setMode(m_mode); //if canceled, set back to current mode
        qDebug() << "test";
        return;
    }

    //updateLabelDuration(0);
    setMode(Mode::LoadFileMode);

    stopEngine();

    m_currentFile = file;

    m_engine->reset();

    m_engine->loadFile(m_currentFile);

    qDebug() << "buffer duration (us): " << m_engine->bufferDuration();

    updateLabelDuration(m_engine->bufferDuration());
    updateLabelSeek(0);
    ui->horizontalSlider_Position->setEnabled(true);
    updateHorizontalSlider(m_engine->bufferDuration());

    updateStatusBar();
}

void MainWindow::openStream()
{
    stopEngine();

    setMode(Mode::StreamMode);
    updateLabelDuration(0);
    updateLabelSeek(0);
    ui->horizontalSlider_Position->setDisabled(true);

    m_engine->reset();
    m_engine->openStream();

    updateStatusBar();
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

void MainWindow::processedUSecsChanged(qint64 processedUSecs)
{
    updateHorizontalSliderPosition(processedUSecs);
    updateLabelSeek(processedUSecs);
}

void MainWindow::on_horizontalSlider_Position_sliderPressed()
{
    //connect(m_engine, &Engine::processedUSecsChanged, this, &MainWindow::processedUSecsChanged);
    //privremeno iskljuci azuriranja slidera
    disconnect(m_engine, &Engine::processedUSecsChanged, 0, 0);
}

void MainWindow::on_horizontalSlider_Position_sliderMoved(int position)
{
    m_startPosition = position;
    updateLabelSeek(m_startPosition);
}

void MainWindow::on_horizontalSlider_Position_sliderReleased()
{
    //qint64 newPosition = ui->horizontalSlider_Position->sliderPosition(); //isto sto i value()

    //reset engine
    m_engine->resetSoft(m_startPosition);

    //natrag ukljuci azuriranje slidera
    connect(m_engine, &Engine::processedUSecsChanged, this, &MainWindow::processedUSecsChanged);
}

void MainWindow::processingComplete()
{
    //todo reset slider, reset seek, reset play/pause button

    updateHorizontalSliderPosition(0);
    updateLabelSeek(0);
    ui->pushButton_PlayPause->setToolTip(tr("Play"));
    ui->pushButton_PlayPause->setIcon(QIcon(":/icons/icons8-play-96-2.png"));

    playing = false;
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
