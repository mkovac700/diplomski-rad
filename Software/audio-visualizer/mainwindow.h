#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMediaDevices>

#include <audiolistener.h>

#include <audioplayer.h>

#include <fileplayer.h>

#include <QAudioSink>

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

    void set_level_l(qreal value);
    void set_level_r(qreal value);

private slots:

    void on_pushButton_StartStop_clicked();

    void on_comboBox_SampleRate_currentTextChanged(const QString &arg1);

    void on_pushButton_OsvjeziUredaje_clicked();

    void on_pushButton_play_clicked();

    void on_pushButton_pause_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_openFile_clicked();

private:
    Ui::MainWindow *ui;

    QMediaDevices *m_devices = new QMediaDevices(this);

    QScopedPointer<AudioListener> m_audioListener;
    QScopedPointer<QAudioSource> m_audioInput;

    int m_sample_rate = 48000;

    AudioPlayer *audioPlayer;

    FilePlayer *filePlayer;
    QScopedPointer<QAudioSink> m_audioOutput;

private:
    void loadInputDevices();

    void initializeInputAudio(const QAudioDevice &inputDevice);

    void loadOutputDevices();

    void initializeOutputAudio(const QAudioDevice &outputDevice);
};
#endif // MAINWINDOW_H
