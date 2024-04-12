#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <portaudio.h>

#include <QMessageBox>

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 512

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
    void on_comboBox_AudioIn_currentIndexChanged(int index);

    void on_comboBox_AudioOut_currentIndexChanged(int index);

    void on_pushButton_StartStop_clicked();

private:
    Ui::MainWindow *ui;

    PaError err;

    PaStreamParameters inputParameters;
    PaStreamParameters outputParameters;

    PaStream* stream;

    int device_in;
    int device_out;

    bool isStreaming = false;

    bool checkErr(PaError err);

    static inline float max(float a, float b);

    void scanDevices();

    void setDevices();

    static int patestCallback(
        const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags,
        void* userData
        );

    static MainWindow *s_mainWindow;

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
