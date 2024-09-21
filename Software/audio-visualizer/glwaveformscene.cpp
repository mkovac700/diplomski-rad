#include "glwaveformscene.h"

void GLWaveformScene::initialize()
{
    glWidget->resetState();

    initializeOpenGLFunctions();

    // Enable line smoothing
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_POINT_SMOOTH);

    // Enable multisampling for better anti-aliasing (if supported)
    glEnable(GL_MULTISAMPLE);

    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, glWidget->width(), -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLineWidth(1.1f);
}

void GLWaveformScene::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWaveformScene::paint()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1, 1, 1);
    glBegin(GL_LINE_STRIP);

    float xScale = 1;

    if (!m_buffer.empty())
        xScale = glWidget->width() / static_cast<float>(m_buffer.size());

    for (int i = 0; i < m_buffer.size(); i++) {
        float hue = (i * 360.0f) / m_buffer.size();
        float r, g, b;
        HSVtoRGB(hue, 1.0f, 1.0f, r, g, b);
        glColor3f(r, g, b);

        auto x = i * xScale;
        auto y = m_buffer[i];
        glVertex2f(x, y);
    }

    glEnd();
    glFlush();
}

void GLWaveformScene::HSVtoRGB(float H, float S, float V, float &r, float &g, float &b)
{
    float C = V * S;
    float X = C * (1 - fabs(fmod(H / 60.0, 2) - 1));
    float m = V - C;

    if (0 <= H && H < 60) {
        r = C;
        g = X;
        b = 0;
    } else if (60 <= H && H < 120) {
        r = X;
        g = C;
        b = 0;
    } else if (120 <= H && H < 180) {
        r = 0;
        g = C;
        b = X;
    } else if (180 <= H && H < 240) {
        r = 0;
        g = X;
        b = C;
    } else if (240 <= H && H < 300) {
        r = X;
        g = 0;
        b = C;
    } else if (300 <= H && H < 360) {
        r = C;
        g = 0;
        b = X;
    } else {
        r = 0;
        g = 0;
        b = 0;
    }

    r += m;
    g += m;
    b += m;
}

//---------------------
//SLOTS
//---------------------
void GLWaveformScene::bufferChanged(QList<qreal> &buffer)
{
    m_buffer = buffer;
}

void GLWaveformScene::spectrumChanged(FrequencySpectrum &spectrum)
{
    Q_UNUSED(spectrum)
}

void GLWaveformScene::spectrumChanged(qint64 position,
                                      qint64 length,
                                      const FrequencySpectrum &spectrum,
                                      int inputFrequency)
{
    Q_UNUSED(position)
    Q_UNUSED(length)
    Q_UNUSED(spectrum)
    Q_UNUSED(inputFrequency)
}

void GLWaveformScene::reinitialize()
{
    glWidget->makeCurrent();
    initialize();
    glWidget->doneCurrent();
}

void GLWaveformScene::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void GLWaveformScene::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void GLWaveformScene::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event)
}
