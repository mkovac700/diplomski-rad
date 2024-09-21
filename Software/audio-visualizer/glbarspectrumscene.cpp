#include "glbarspectrumscene.h"

void GLBarSpectrumScene::initialize()
{
    glWidget->resetState();

    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, glWidget->width(), 0, glWidget->height(), -1, 1); // Adjusted to start from bottom-left
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    m_fftSize = EngineSettings::instance().fftSize();
    m_drawGrid = GraphicsSettings::instance().drawGrid();

    m_lowFreq = GraphicsSettings::instance().minFreq();
    m_highFreq = GraphicsSettings::instance().maxFreq();

    m_bandWidth = GraphicsSettings::instance().bandWidth();
    m_gridStepHz = GraphicsSettings::instance().gridStepHz();

    m_barPowerUnitMeasure = GraphicsSettings::instance().barPowerUnitMeasure();
    m_barYScaleFactor = GraphicsSettings::instance().barYScaleFactor();
}

void GLBarSpectrumScene::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1, 1); // Adjusted to start from bottom-left
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLBarSpectrumScene::paint()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1, 1, 1);

    qreal deltaF = m_inputFrequency / m_fftSize;

    qreal k_lower = m_lowFreq / deltaF;
    qreal k_upper = m_highFreq / deltaF;

    m_numBands = k_upper - k_lower + 1;
    // m_bandWidth = m_highFreq - m_lowFreq / m_numBands;

    /*float xScale = glWidget->width() / (float) 20000;*/ //(float) width() / 10000;
    float xScale = glWidget->width() / (float) m_highFreq;
    float yScale = glWidget->height() / (float) m_barYScaleFactor; //5.0f

    glLineWidth(m_bandWidth);
    glColor3f(1, 1, 1);

    int j = 0;

    FrequencySpectrum::iterator i = m_spectrum.begin();
    const FrequencySpectrum::iterator end = m_spectrum.end();
    for (; i != end; ++i) {
        const FrequencySpectrum::Element e = *i;

        float hue = (j * 360.0f) / m_numBands; //m_spectrum.count() * 2
        float r, g, b;
        HSVtoRGB(hue, 1.0f, 1.0f, r, g, b);
        glColor3f(r, g, b);

        if (e.frequency >= m_lowFreq && e.frequency <= m_highFreq) {
            glBegin(GL_LINE_STRIP);

            // glVertex2f(e.frequency * xScale, 0.0f);              // Start point (x, 0)
            // glVertex2f(e.frequency * xScale, e.amplitude * 100); // End point (x, y) //* 100 //*10

            glVertex2f(e.frequency * xScale, 0.0f); // Start point (x, 0)

            // End point (x, y) //* 100 //*10
            if (m_barPowerUnitMeasure == UnitMeasurement::Magnitude)
                glVertex2f(e.frequency * xScale, e.magnitude * yScale);
            else if (m_barPowerUnitMeasure == UnitMeasurement::DB)
                glVertex2f(e.frequency * xScale, e.db * yScale);
            else if (m_barPowerUnitMeasure == UnitMeasurement::Amplitude)
                glVertex2f(e.frequency * xScale, e.amplitude * yScale);

            glEnd();
        }

        j++;
    }

    if (m_drawGrid) {
        glLineWidth(1.0);
        glColor3f(0, 1, 0);
        for (int i = 0; i < static_cast<int>(m_highFreq); i += m_gridStepHz) {
            glBegin(GL_LINE_STRIP);
            glVertex2f(i * xScale, 0.0f * yScale);
            glVertex2f(i * xScale, glWidget->height());
            glEnd();
        }
    }

    glFlush();
}

void GLBarSpectrumScene::HSVtoRGB(float H, float S, float V, float &r, float &g, float &b)
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
void GLBarSpectrumScene::bufferChanged(QList<qreal> &buffer)
{
    Q_UNUSED(buffer)
}

void GLBarSpectrumScene::spectrumChanged(FrequencySpectrum &spectrum)
{
    m_spectrum = spectrum;
}

void GLBarSpectrumScene::spectrumChanged(qint64 position,
                                         qint64 length,
                                         const FrequencySpectrum &spectrum,
                                         int inputFrequency)
{
    Q_UNUSED(position)
    Q_UNUSED(length)
    m_spectrum = spectrum;
    m_inputFrequency = inputFrequency;
}

void GLBarSpectrumScene::reinitialize()
{
    glWidget->makeCurrent();
    initialize();
    glWidget->doneCurrent();
}

void GLBarSpectrumScene::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void GLBarSpectrumScene::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void GLBarSpectrumScene::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event)
}
