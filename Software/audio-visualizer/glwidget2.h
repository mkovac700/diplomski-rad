#ifndef GLWIDGET2_H
#define GLWIDGET2_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include <fftw3.h>

#include <signalprocessor.h>

#include <frequencyspectrum.h>

class GLWidget2 : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GLWidget2(QWidget *parent = nullptr);
    ~GLWidget2();

    void setBuffer(QList<QPointF> buffer);

public slots:
    void spectrumChanged(const FrequencySpectrum &spectrum);
    void spectrumChanged(qint64 position, qint64 length, const FrequencySpectrum &spectrum);

    // QOpenGLWidget interface
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QSurfaceFormat m_format;

    QList<QPointF> m_buffer;

    QList<QPointF> dataPoints;

    SignalProcessor signalProcessor;

    FrequencySpectrum m_spectrum;
};

#endif // GLWIDGET2_H
