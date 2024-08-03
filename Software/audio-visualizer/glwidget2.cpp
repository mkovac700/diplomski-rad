#include "glwidget2.h"

GLWidget2::GLWidget2(QWidget *parent)
    : QOpenGLWidget(parent)
{}

GLWidget2::~GLWidget2() {}

void GLWidget2::setBuffer(QList<QPointF> buffer)
{
    dataPoints.clear();
    dataPoints.append(signalProcessor.process(buffer, SignalProcessor::WindowingMode::Bartlett));

    update();
}

void GLWidget2::spectrumChanged(const FrequencySpectrum &spectrum)
{ //TODO: dodati inputFrequency (spectrumanalyzer.cpp)
    m_spectrum = spectrum;

    update();
}

void GLWidget2::spectrumChanged(qint64 position, qint64 length, const FrequencySpectrum &spectrum)
{
    Q_UNUSED(position)
    Q_UNUSED(length)

    m_spectrum = spectrum;

    update();
}

void GLWidget2::initializeGL()
{
    initializeOpenGLFunctions();

    // glClearColor(0, 0, 0, 1);
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // glOrtho(0, width(), -1, 1, -1, 1);
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();

    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width(), 0, height(), -1, 1); // Adjusted to start from bottom-left
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget2::resizeGL(int w, int h)
{
    // glViewport(0, 0, w, h);
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // glOrtho(0, w, -1, 1, -1, 1);
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1, 1); // Adjusted to start from bottom-left
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget2::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1, 1, 1);
    //glBegin(GL_LINE_STRIP);

    //glLineWidth(10.0f); // Set line width to 2.0 pixels

    //float yScale = 1;

    //yScale = height() / 100;

    // for (int i = 0; i < m_buffer.size(); i++) {
    //     auto x = m_buffer[i].x() * xScale;
    //     auto y = m_buffer[i].y();
    //     glVertex2f(x, y);
    // }

    // glEnd();
    // glFlush();

    //zapravo: 8192 * (48000/16384) = 8192*2.92=~24000
    float xScale = width() / (float) 20000; //(float) width() / 10000;
    float yScale = height() / (float) 1.5f;

    // for (const QPointF point : dataPoints) {
    //     glBegin(GL_LINE_STRIP);

    //     glVertex2f(point.x() * xScale, 0.0f);           // Start point (x, 0)
    //     glVertex2f(point.x() * xScale, point.y() * 10); // End point (x, y) //* 100

    //     glEnd();
    // }

    // //glEnd();
    // glFlush();

    FrequencySpectrum::iterator i = m_spectrum.begin();
    const FrequencySpectrum::iterator end = m_spectrum.end();
    for (; i != end; ++i) {
        const FrequencySpectrum::Element e = *i;

        glBegin(GL_LINE_STRIP);

        // glVertex2f(e.frequency * xScale, 0.0f);              // Start point (x, 0)
        // glVertex2f(e.frequency * xScale, e.amplitude * 100); // End point (x, y) //* 100 //*10

        glVertex2f(e.frequency * xScale, 0.0f * yScale);        // Start point (x, 0)
        glVertex2f(e.frequency * xScale, e.amplitude * yScale); // End point (x, y) //* 100 //*10

        glEnd();
    }

    glFlush();
}
