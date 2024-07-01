#ifndef SIGNALPROCESSOR_H
#define SIGNALPROCESSOR_H

#include <QObject>
#include <QPointF>

#include <fftw3.h>

#define REAL 0
#define IMAG 1
#define N 16384 //480
#define Fs 48000.0

class SignalProcessor : QObject
{
    Q_OBJECT
public:
    enum class WindowingMode { None, Hamming, Hanning, Blackman, Bartlett };

    SignalProcessor();
    ~SignalProcessor();

    QList<QPointF> process(QList<QPointF> &buffer_in, SignalProcessor::WindowingMode windowingMode);

private:
    void performZeroPadding(QList<QPointF> &buffer_in);
    void performWindowing(QList<QPointF> &buffer_in, SignalProcessor::WindowingMode windowingMode);
    QList<QPointF> calculateFrequenciesAndMagnitudes(double minFreq, double maxFreq);

private:
    fftw_complex in[N];
    fftw_complex out[N];

    fftw_plan p;

    QList<QPointF> buffer_out;
};

#endif // SIGNALPROCESSOR_H
