#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glscene.h"

#include <QMediaDevices>

#include <audiolistener.h>

#include <QAudioSink>

#include <QStandardPaths>

#include <engine.h>

#include <utils.h>

#include <settingsdialog.h>

#include <enginesettings.h>

#include <QLabel>

#define LOG_MAINWINDOW

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_stop_clicked();

    void on_pushButton_PlayPause_clicked();

    void openFile();

    void openStream();

    void processedUSecsChanged(qint64 processedUSecs);

    void processingComplete();

    void changeAudioIn();

    void changeAudioOut();

    void showSettingsDialog();

private:
    void loadInputDevices();

    void initializeInputAudio(const QAudioDevice &inputDevice);

    void loadOutputDevices();

    void initializeOutputAudio(const QAudioDevice &outputDevice);

    //MenuBar

    enum Mode { NoMode, StreamMode, GenerateToneMode, LoadFileMode };

    void setMode(Mode mode);

    void initializeMenuMedia();

    void updateMenuMedia();

    /**
     * Updates settings and restarts update timer
     */
    void restartEngine();

    /**
     * Stop all ongoing activity and reset engine to defaults
     */
    void stopEngine();

    /**
     * Set new value of format mm:ss for label_Duration
     * @param duration Duration in microseconds
     */
    void updateLabelDuration(qint64 duration);
    void updateHorizontalSlider(qint64 maxValue);
    void updateHorizontalSliderPosition(qint64 processedUSecs);
    void updateLabelSeek(qint64 uSecs);

    void updateStatusBar();

private:
    Ui::MainWindow *ui;

    QMediaDevices *m_devices = new QMediaDevices(this);

    QScopedPointer<AudioListener> m_audioListener;
    QScopedPointer<QAudioSource> m_audioInput;

    QScopedPointer<QAudioSink> m_audioOutput;

    QString m_currentFile;

    Engine *m_engine;

    bool playing = false;

    Mode m_mode;

    SettingsDialog *m_settingsDialog;

    QLabel *m_statusLabel;

    //------------------------------------------------
    // SCENES
    //------------------------------------------------
private:
    QVector<GLScene *> glScenes;
    int currentGLScene = 0;

private:
    void initializeScenes();

private slots:
    void on_pushButton_PreviousScene_clicked();
    void on_pushButton_NextScene_clicked();
};
#endif // MAINWINDOW_H
