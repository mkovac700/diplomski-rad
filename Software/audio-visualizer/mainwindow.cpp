#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow* MainWindow::s_mainWindow = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    err = Pa_Initialize();

    if(checkErr(err)){
        QMessageBox::critical(this,"Greška",QString("PortAudio: ").append(Pa_GetErrorText(err)));
    }

    scanDevices();

    s_mainWindow = this;

}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::checkErr(PaError err)
{
    if(err != paNoError) return true;
    else return false;
}

float MainWindow::max(float a, float b)
{
    return a > b ? a : b;
}

void MainWindow::scanDevices()
{
    int numDevices = Pa_GetDeviceCount();

    if(numDevices < 0) {
        QMessageBox::critical(this,"Greška","Greška u pretraživanju uređaja!");
        return;
    }
    else if (numDevices == 0){
        QMessageBox::critical(this,"Greška","Nema dostupnih uređaja!");
        return;
    }

    const PaDeviceInfo* deviceInfo;
    for (int i = 0; i < numDevices; i++) {
        deviceInfo = Pa_GetDeviceInfo(i);

        qDebug() << "Device: " << i;
        qDebug() << "Name: " << deviceInfo->name;
        qDebug() << "Max Input Channels: " << deviceInfo->maxInputChannels;
        qDebug() << "Max Output Channels: " << deviceInfo->maxOutputChannels;
        qDebug() << "Default Sample Rate: " << deviceInfo->defaultSampleRate;

        if(deviceInfo->maxInputChannels > 0)
            ui->comboBox_AudioIn->addItem(deviceInfo->name,i);

        if(deviceInfo->maxOutputChannels > 0)
            ui->comboBox_AudioOut->addItem(deviceInfo->name,i);

        ui->textEdit_Uredaji->append(QString("Device: ").append(QString::number(i)));
        ui->textEdit_Uredaji->append(QString("  Name: ").append(deviceInfo->name));
        ui->textEdit_Uredaji->append(QString("  Max Input Channels: ").append(QString::number(deviceInfo->maxInputChannels)));
        ui->textEdit_Uredaji->append(QString("  Max Output Channels: ").append(QString::number(deviceInfo->maxOutputChannels)));
        ui->textEdit_Uredaji->append(QString("  Default Sample Rate: ").append(QString::number(deviceInfo->defaultSampleRate)));
    }

    setDevices();
}

void MainWindow::setDevices()
{
    device_in = ui->comboBox_AudioIn->itemData(ui->comboBox_AudioIn->currentIndex()).toInt();
    device_out = ui->comboBox_AudioOut->itemData(ui->comboBox_AudioOut->currentIndex()).toInt();

    memset(&inputParameters, 0, sizeof(inputParameters));
    inputParameters.channelCount = 2;
    inputParameters.device = device_in;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(device_in)->defaultLowInputLatency;

    memset(&outputParameters, 0, sizeof(outputParameters));
    outputParameters.channelCount = 2;
    outputParameters.device = device_out;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(device_out)->defaultLowInputLatency;
}

int MainWindow::patestCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
    float* in = (float*)inputBuffer;
    (void)outputBuffer;

    float vol_l = 0;
    float vol_r = 0;

    for (unsigned long i = 0; i < framesPerBuffer * 2; i += 2) {
        vol_l = max(vol_l, std::abs(in[i]));
        vol_r = max(vol_r, std::abs(in[i+1]));
    }

    s_mainWindow->ui->progressBar_Vol_L->setValue(static_cast<int>(vol_l*100));
    s_mainWindow->ui->progressBar_Vol_R->setValue(static_cast<int>(vol_r*100));

    return 0;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    err = Pa_Terminate();

    if(checkErr(err)){
        QMessageBox::critical(this,"Greška",QString("PortAudio: ").append(Pa_GetErrorText(err)));
        return;
    }

    QMainWindow::closeEvent(event);
}

void MainWindow::on_comboBox_AudioIn_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    setDevices();
}


void MainWindow::on_comboBox_AudioOut_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    setDevices();
}

void MainWindow::on_pushButton_StartStop_clicked()
{
    if(!isStreaming){ //ako ne streama
        err = Pa_OpenStream(
            &stream,
            &inputParameters,
            &outputParameters,
            sample_rate,
            FRAMES_PER_BUFFER,
            paNoFlag,
            patestCallback,
            NULL
            );

        if(checkErr(err)){
            QMessageBox::critical(this,"Greška",QString("PortAudio: ").append(Pa_GetErrorText(err)));
            return;
        }

        err = Pa_StartStream(stream);

        if(checkErr(err)){
            QMessageBox::critical(this,"Greška",QString("PortAudio: ").append(Pa_GetErrorText(err)));
            return;
        }
    }
    else{ //ako streama
        err = Pa_StopStream(stream);

        if(checkErr(err)){
            QMessageBox::critical(this,"Greška",QString("PortAudio: ").append(Pa_GetErrorText(err)));
            return;
        }

        err = Pa_CloseStream(stream);

        if(checkErr(err)){
            QMessageBox::critical(this,"Greška",QString("PortAudio: ").append(Pa_GetErrorText(err)));
            return;
        }
    }

    if(!isStreaming) ui->pushButton_StartStop->setText("Zaustavi stream");
    else {
        ui->pushButton_StartStop->setText("Započni stream");
        ui->progressBar_Vol_L->setValue(0);
        ui->progressBar_Vol_R->setValue(0);
    }

    isStreaming = !isStreaming;


}


void MainWindow::on_comboBox_SampleRate_currentIndexChanged(int index)
{
    Q_UNUSED(index)

    sample_rate = ui->comboBox_SampleRate->currentText().toInt();
}

