#include "signalprocessor.h"
#include "qdebug.h"

SignalProcessor::SignalProcessor()
{
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
}

SignalProcessor::~SignalProcessor()
{
    fftw_destroy_plan(p);
}

QList<QPointF> SignalProcessor::process(QList<QPointF> &buffer_in, WindowingMode windowingMode)
{
    for (int i = 0; i < buffer_in.size(); i++) {
        in[i][REAL] = buffer_in[i].y();
        in[i][IMAG] = 0.0;
    }

    performWindowing(buffer_in, windowingMode);

    performZeroPadding(buffer_in);

    fftw_execute(p);

    return calculateFrequenciesAndMagnitudes(0, 20000);
}

void SignalProcessor::performZeroPadding(QList<QPointF> &buffer_in)
{
    for (int i = buffer_in.size(); i < N; i++) {
        in[i][REAL] = 0.0;
        in[i][IMAG] = 0.0;
    }
}

void SignalProcessor::performWindowing(QList<QPointF> &buffer_in, WindowingMode windowingMode)
{
    switch (windowingMode) {
    case WindowingMode::Hamming: {
        for (int i = 0; i < buffer_in.size(); i++) {
            in[i][REAL] *= (0.54 - 0.46 * std::cos(2 * M_PI * i / (buffer_in.size() - 1)));
            in[i][IMAG] = 0.0;
        }

        break;
    }
    case WindowingMode::Bartlett: {
        for (int i = 0; i < buffer_in.size(); i++) {
            in[i][REAL] *= (2.0 / (buffer_in.size() - 1)
                            * ((buffer_in.size() - 1) / 2.0
                               - std::abs(i - (buffer_in.size() - 1) / 2.0)));
            in[i][IMAG] = 0.0;
        }

        break;
    }
    default:
        break;
    }
}

QList<QPointF> SignalProcessor::calculateFrequenciesAndMagnitudes(double minFreq, double maxFreq)
{
    int k;
    double freq = 0;
    double magnitude = 0;
    double deltaF = Fs / N;

    buffer_out.clear();

    for (k = 0; k <= N / 2; ++k) { // We only need the first N/2 + 1 bins
        freq = k * deltaF;
        //if (freq >= minFreq && freq <= maxFreq) {
        magnitude = sqrt(out[k][0] * out[k][0] + out[k][1] * out[k][1]);
        //printf("Frequency: %f Hz, Magnitude: %f\n", freq, magnitude);
        buffer_out.append({freq, magnitude});
        //}
    }

    return buffer_out;
}
