#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMediaDevices>

#include <audiolistener.h>

#include <QAudioSink>

#include <QStandardPaths>

#include <engine.h>

#include <utils.h>

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
};
#endif // MAINWINDOW_H
