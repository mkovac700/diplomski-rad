#include "glwidget2.h"

GLWidget2::GLWidget2(QWidget *parent)
    : QOpenGLWidget(parent)
{
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
}

GLWidget2::~GLWidget2() {}

void GLWidget2::setBuffer(QList<QPointF> buffer)
{
    m_buffer = buffer;

    //calculate fft

    for (int i = 0; i < N; i++) {
        in[i][REAL] = m_buffer[i].y();
        in[i][IMAG] = 0.0;
    }

    fftw_execute(p);

    update();

    //qDebug() << "Buffer received! First value: (" << m_buffer.first().x() << ", " << m_buffer.first().y() << ")";
}

void GLWidget2::calculateFrequenciesAndMagnitudes(
    fftw_complex *out, int n, double fs, double minFreq, double maxFreq)
{
    int k;
    double freq;
    double magnitude;
    double deltaF = fs / n;

    for (k = 0; k <= n / 2; ++k) { // We only need the first N/2 + 1 bins
        freq = k * deltaF;
        if (freq >= minFreq && freq <= maxFreq) {
            magnitude = sqrt(out[k][0] * out[k][0] + out[k][1] * out[k][1]);
            //printf("Frequency: %f Hz, Magnitude: %f\n", freq, magnitude);
        }
    }

    QPointF result = QPointF(freq, magnitude);
}

void GLWidget2::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width(), -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget2::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget2::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1, 1, 1);
    glBegin(GL_LINE_STRIP);

    float xScale = 1;

    if (!m_buffer.empty())
        xScale = width() / static_cast<float>(m_buffer.size());

    for (int i = 0; i < m_buffer.size(); i++) {
        auto x = m_buffer[i].x() * xScale;
        auto y = m_buffer[i].y();
        glVertex2f(x, y);
    }

    glEnd();
    glFlush();
}
