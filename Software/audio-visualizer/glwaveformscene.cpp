#include "glwaveformscene.h"

void GLWaveformScene::initialize()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, glWidget->width(), -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
        auto x = i * xScale;
        auto y = m_buffer[i];
        glVertex2f(x, y);
    }

    glEnd();
    glFlush();
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
                                      const FrequencySpectrum &spectrum)
{
    Q_UNUSED(position)
    Q_UNUSED(length)
    Q_UNUSED(spectrum)
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
