// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "spectrumanalyser.h"
//#include "fftreal_wrapper.h"
#include "utils.h"

#include <QAudioFormat>
#include <QMetaType>
#include <QThread>
#include <QtMath>

SpectrumAnalyserThread::SpectrumAnalyserThread(QObject *parent)
    : QObject(parent)
#ifndef DISABLE_FFT
// ,
// m_fft(new FFTRealWrapper)
#endif
      ,
      m_numSamples(SpectrumLengthSamples),
      m_windowFunction(DefaultWindowFunction),
      m_window(SpectrumLengthSamples, 0.0),
      m_input(SpectrumLengthSamples, 0.0),
      m_output(SpectrumLengthSamples, 0.0),
      m_spectrum(SpectrumLengthSamples)
#ifdef SPECTRUM_ANALYSER_SEPARATE_THREAD
      ,
      m_thread(new QThread(this))
#endif
{
#ifdef SPECTRUM_ANALYSER_SEPARATE_THREAD
    // moveToThread() cannot be called on a QObject with a parent
    setParent(nullptr);
    moveToThread(m_thread);
    m_thread->start();
#endif
    p = fftw_plan_dft_1d(SpectrumLengthSamples, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    calculateWindow();
}

SpectrumAnalyserThread::~SpectrumAnalyserThread()
{
#ifndef DISABLE_FFT
    // delete m_fft;
    fftw_destroy_plan(p);
#endif
}

void SpectrumAnalyserThread::setWindowFunction(WindowFunction type)
{
    m_windowFunction = type;
    calculateWindow();
}

void SpectrumAnalyserThread::calculateWindow()
{
    for (int i = 0; i < m_numSamples; ++i) {
        // DataType x = 0.0;
        qreal x = 0.0;

        switch (m_windowFunction) {
        case NoWindow:
            x = 1.0;
            break;
        case HannWindow:
            x = 0.5 * (1 - qCos((2 * M_PI * i) / (m_numSamples - 1)));
            break;
        case BlackmanWindow:
            x = 0.42 - 0.5 * qCos(2 * M_PI * i / (m_numSamples - 1))
                + 0.08 * qCos(4 * M_PI * i / (m_numSamples - 1));
            break;
        default:
            Q_ASSERT(false);
        }

        m_window[i] = x;
    }
}

void SpectrumAnalyserThread::calculateSpectrum(const QByteArray &buffer, int inputFrequency,
                                               int bytesPerFrame)
{
#ifndef DISABLE_FFT
    Q_ASSERT(buffer.size() == m_numSamples * bytesPerFrame);

    // Initialize data array
    const char *ptr = buffer.constData();
    for (int i = 0; i < m_numSamples; ++i) {
        const qint16 pcmSample = *reinterpret_cast<const qint16 *>(ptr);
        // Scale down to range [-1.0, 1.0]
        const qreal realSample = pcmToReal(pcmSample);
        // const DataType windowedSample = realSample * m_window[i];
        // m_input[i] = windowedSample;
        const qreal windowedSample = realSample * m_window[i];
        m_input[i] = windowedSample;
        in[i][REAL] = windowedSample;
        in[i][IMAG] = 0.0f;
        ptr += bytesPerFrame;
    }

    emit bufferReceived(m_input); //connectano na spectrumanalyzer::bufferreceived

    // Calculate the FFT
    // m_fft->calculateFFT(m_output.data(), m_input.data());
    fftw_execute(p);

    // Analyze output to obtain amplitude and phase for each frequency
    for (int i = 0; i <= m_numSamples / 2; ++i) { // i = 2 ?!
        // Calculate frequency of this complex sample
        m_spectrum[i].frequency = qreal(i * inputFrequency) / (m_numSamples);

        // const qreal real = m_output[i];
        // qreal imag = 0.0;
        // if (i > 0 && i < m_numSamples / 2)
        //     imag = m_output[m_numSamples / 2 + i];
        qreal real = out[i][REAL];
        qreal imag = out[i][IMAG];

        const qreal magnitude = qSqrt(real * real + imag * imag);
        qreal amplitude = SpectrumAnalyserMultiplier * qLn(magnitude);

        // Bound amplitude to [0.0, 1.0]
        m_spectrum[i].clipped = (amplitude > 1.0);
        amplitude = qMax(qreal(0.0), amplitude);
        amplitude = qMin(qreal(1.0), amplitude);
        m_spectrum[i].amplitude = amplitude;
    }
#endif
    SPECTRUMANALYSER_DEBUG << "SpectrumAnalyserThread::calculateSpectrum"
                           << QThread::currentThread() << "spectrum size " << m_spectrum.count();

    emit calculationComplete(m_spectrum); //TODO: dodati inputFrequency
}

//=============================================================================
// SpectrumAnalyser
//=============================================================================

SpectrumAnalyser::SpectrumAnalyser(QObject *parent)
    : QObject(parent),
      m_thread(new SpectrumAnalyserThread(this)),
      m_state(Idle)
#ifdef DUMP_SPECTRUMANALYSER
      ,
      m_count(0)
#endif
{
    connect(m_thread,
            &SpectrumAnalyserThread::bufferReceived,
            this,
            &SpectrumAnalyser::bufferReceived);

    connect(m_thread,
            &SpectrumAnalyserThread::calculationComplete,
            this,
            &SpectrumAnalyser::calculationComplete);
}

SpectrumAnalyser::~SpectrumAnalyser() = default;

#ifdef DUMP_SPECTRUMANALYSER
void SpectrumAnalyser::setOutputPath(const QString &outputDir)
{
    m_outputDir.setPath(outputDir);
    m_textFile.setFileName(m_outputDir.filePath("spectrum.txt"));
    m_textFile.open(QIODevice::WriteOnly | QIODevice::Text);
    m_textStream.setDevice(&m_textFile);
}
#endif

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void SpectrumAnalyser::setWindowFunction(WindowFunction type)
{
    const bool b = QMetaObject::invokeMethod(m_thread, "setWindowFunction", Qt::AutoConnection,
                                             Q_ARG(WindowFunction, type));
    Q_ASSERT(b);
    Q_UNUSED(b); // suppress warnings in release builds
}

void SpectrumAnalyser::calculate(const QByteArray &buffer, const QAudioFormat &format)
{
    // QThread::currentThread is marked 'for internal use only', but
    // we're only using it for debug output here, so it's probably OK :)
    SPECTRUMANALYSER_DEBUG << "SpectrumAnalyser::calculate" << QThread::currentThread() << "state"
                           << m_state;

    // SPECTRUMANALYSER_DEBUG << "SpectrumAnalyser::calculate" << QThread::currentThread()
    //                        << "buffer size " << buffer.size();

    if (isReady()) {
        Q_ASSERT(format.sampleFormat() == QAudioFormat::Int16);

        const int bytesPerFrame = format.bytesPerFrame();

#ifdef DUMP_SPECTRUMANALYSER
        m_count++;
        const QString pcmFileName =
                m_outputDir.filePath(QString("spectrum_%1.pcm").arg(m_count, 4, 10, QChar('0')));
        QFile pcmFile(pcmFileName);
        pcmFile.open(QIODevice::WriteOnly);
        const int bufferLength = m_numSamples * bytesPerFrame;
        pcmFile.write(buffer, bufferLength);

        m_textStream << "TimeDomain " << m_count << "\n";
        const qint16 *input = reinterpret_cast<const qint16 *>(buffer);
        for (int i = 0; i < m_numSamples; ++i) {
            m_textStream << i << "\t" << *input << "\n";
            input += format.channels();
        }
#endif

        m_state = Busy;

        // Invoke SpectrumAnalyserThread::calculateSpectrum using QMetaObject.  If
        // m_thread is in a different thread from the current thread, the
        // calculation will be done in the child thread.
        // Once the calculation is finished, a calculationChanged signal will be
        // emitted by m_thread.
        const bool b = QMetaObject::invokeMethod(
                m_thread, "calculateSpectrum", Qt::AutoConnection, Q_ARG(QByteArray, buffer),
                Q_ARG(int, format.sampleRate()), Q_ARG(int, bytesPerFrame));
        Q_ASSERT(b);
        Q_UNUSED(b); // suppress warnings in release builds

#ifdef DUMP_SPECTRUMANALYSER
        m_textStream << "FrequencySpectrum " << m_count << "\n";
        FrequencySpectrum::const_iterator x = m_spectrum.begin();
        for (int i = 0; i < m_numSamples; ++i, ++x)
            m_textStream << i << "\t" << x->frequency << "\t" << x->amplitude << "\t" << x->phase
                         << "\n";
#endif
    }
}

bool SpectrumAnalyser::isReady() const
{
    return (Idle == m_state);
}

void SpectrumAnalyser::cancelCalculation()
{
    if (Busy == m_state)
        m_state = Cancelled;
}

//-----------------------------------------------------------------------------
// Private slots
//-----------------------------------------------------------------------------

void SpectrumAnalyser::bufferReceived(QList<qreal> &buffer)
{
    emit bufferChanged(buffer); //CONNECTAT NA OGL1->BUFFERCHANGED
}

void SpectrumAnalyser::calculationComplete(const FrequencySpectrum &spectrum)
{
    Q_ASSERT(Idle != m_state);
    if (Busy == m_state) {
        emit spectrumChanged(spectrum); //CONNECTANO NATRAG NA ENGINE --> OD TAMO CONNECT NA OGL2
    }                                   //TODO: dodati inputFrequency
    m_state = Idle;
}

#include "moc_spectrumanalyser.cpp"
