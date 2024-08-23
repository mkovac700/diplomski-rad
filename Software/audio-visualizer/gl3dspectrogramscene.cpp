#include "gl3dspectrogramscene.h"

GL3DSpectrogramScene::GL3DSpectrogramScene(GLWidget *glWidget)
    : GLScene(glWidget)
    , m_rotationX(10)
    , m_rotationY(0)
    , m_rotationZ(0)
    , m_positionX(0)
    , m_positionY(0)
    , m_distance(-200)
{}

void GL3DSpectrogramScene::initialize()
{
    glWidget->resetState();

    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    // Enable line smoothing
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    // Enable multisampling for better anti-aliasing (if supported)
    // glEnable(GL_MULTISAMPLE);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    int w = glWidget->width(), h = glWidget->height();

    glViewport(0, 0, w, h);

    // Postavljanje perspektivne projekcije
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(45.0f, GLfloat(w) / h, 0.1f, 1000.0f);

    // initShaders();

    // Generiranje i vezivanje VAO i VBO
    //glGenVertexArrays(1, &vao);
    // glGenBuffers(1, &vao);
    // glGenBuffers(1, &vbo);

    // if (!initialized) {

    //     initialized = true;
    // }

    m_numPoints = Settings::instance().fftSize() / 2;
    m_peaks.clear();
    m_freqs.clear();

    //qDebug() << "num points changed to: " << m_numPoints;

    // Inicijalizacija peakova (postavljanje svih na 0)
    for (int i = 0; i < m_numLines; ++i) {
        std::vector<qreal> linePeaks(m_numPoints, 0.0f);
        std::vector<qreal> lineFreqs(m_numPoints, 0.0f);
        m_peaks.push_back(linePeaks);
        m_freqs.push_back(lineFreqs);
    }
}

void GL3DSpectrogramScene::resize(int w, int h)
{
    glViewport(0, 0, w, h);

    // Postavljanje perspektivne projekcije
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(45.0f,
                                   GLfloat(w) / h,
                                   0.1f,
                                   1000.0f); //static_cast<float>(m_numPoints) //1000.0f
}

void GL3DSpectrogramScene::HSVtoRGB(float H, float S, float V, float &r, float &g, float &b)
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

void GL3DSpectrogramScene::paint()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 modelViewMatrix;
    modelViewMatrix.translate(m_positionX, m_positionY, m_distance);
    modelViewMatrix.rotate(m_rotationX, 1.0f, 0.0f, 0.0f);
    modelViewMatrix.rotate(m_rotationY, 0.0f, 1.0f, 0.0f);
    // modelViewMatrix.rotate(rotationZ, 0.0f, 0.0f, 1.0f);

    QMatrix4x4 mvpMatrix = m_projectionMatrix * modelViewMatrix;

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(mvpMatrix.constData());

    // 2D mreža

    //glColor3f(1.0f, 1.0f, 1.0f); // Bijela boja za linije

    //updatePeaks();

    //QMutexLocker locker(&m_mutex);

    // First Pass: Draw the black background under the lines

    glColor3f(0.0f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    for (int i = 0; i < m_numLines; ++i) {
        float z = (i - m_numLines / 2) * m_spacingZ;
        for (int j = 0; j < m_numPoints - 1; ++j) {
            float x1 = (m_freqs[i][j] - 11025.0f) * 0.01f;
            float x2 = (m_freqs[i][j + 1] - 11025.0f) * 0.01f;

            // float x1 = (m_freqs[i][j] - 11025.0f);
            // float x2 = (m_freqs[i][j + 1] - 11025.0f);

            // float x1 = (m_freqs[i][j] - 22050.0f) * 0.01f;
            // float x2 = (m_freqs[i][j + 1] - 22050.0f) * 0.01f;

            // float x1 = (m_freqs[i][j]) * 0.01f;
            // float x2 = (m_freqs[i][j + 1]) * 0.01f;

            // if (j == 0) {
            //     qDebug() << "m_freqs[i][j]" << m_freqs[i][j] << "m_freqs[i][j + 1]"
            //              << m_freqs[i][j + 1];
            //     qDebug() << "x1: " << x1 << "x2: " << x2;
            // }

            float y1 = m_peaks[i][j];
            float y2 = m_peaks[i][j + 1];

            // Draw quad from x1, y1 to x2, y2, extending to the bottom of the screen (y=0)
            glVertex3f(x1, 0.0f, z); // Bottom left
            glVertex3f(x2, 0.0f, z); // Bottom right
            glVertex3f(x2, y2, z);   // Top right
            glVertex3f(x1, y1, z);   // Top left
        }
    }
    glEnd();
    glFlush();

    //Second Pass: Draw the lines themselves

    glBegin(GL_LINES);
    for (int i = 0; i < m_numLines; ++i) {
        float hue = (i * 360.0f) / m_numLines;
        float r, g, b;
        HSVtoRGB(hue, 1.0f, 1.0f, r, g, b);

        glColor3f(r, g, b);

        float z = (i - m_numLines / 2) * m_spacingZ;
        for (int j = 0; j < m_numPoints - 1; ++j) {
            // float x1 = (j - m_numPoints / 2) * m_spacingX;
            // float x2 = ((j + 1) - m_numPoints / 2) * m_spacingX;

            // float x1 = m_freqs[i][j] * (j - m_numPoints / 2) * m_spacingX;
            // float x2 = m_freqs[i][j + 1] * ((j + 1) - m_numPoints / 2) * m_spacingX;

            // float x1 = m_freqs[i][j] * 0.01f;
            // float x2 = m_freqs[i][j + 1] * 0.01f;

            float x1 = (m_freqs[i][j] - 11025.0f) * 0.01f;
            float x2 = (m_freqs[i][j + 1] - 11025.0f) * 0.01f;

            // float x1 = (m_freqs[i][j] - 22050.0f) * 0.01f;
            // float x2 = (m_freqs[i][j + 1] - 22050.0f) * 0.01f;

            // float x1 = (m_freqs[i][j]) * 0.01f;
            // float x2 = (m_freqs[i][j + 1]) * 0.01f;

            float y1 = m_peaks[i][j];
            float y2 = m_peaks[i][j + 1];

            // Crtanje linija s nasumičnim peakovima
            glVertex3f(x1, y1, z);
            glVertex3f(x2, y2, z);
        }
    }
    glEnd();
    glFlush();
}

void GL3DSpectrogramScene::reinitialize()
{
    glWidget->makeCurrent();
    initialize();
    //paint();
    glWidget->doneCurrent();
}

void GL3DSpectrogramScene::bufferChanged(QList<qreal> &buffer)
{
    Q_UNUSED(buffer)
}

void GL3DSpectrogramScene::spectrumChanged(FrequencySpectrum &spectrum)
{
    Q_UNUSED(spectrum)
}

void GL3DSpectrogramScene::spectrumChanged(qint64 position,
                                           qint64 length,
                                           const FrequencySpectrum &spectrum)
{
    Q_UNUSED(position)
    Q_UNUSED(length)
    Q_UNUSED(spectrum)

    m_spectrum = spectrum;

    updatePeaks();
    //glWidget->update();
}

std::vector<qreal> linearToLogFreqs(const std::vector<qreal> &freqs,
                                    qreal f_min,
                                    qreal f_max,
                                    qreal alpha)
{
    std::vector<double> scaledLogFrequencies;
    scaledLogFrequencies.reserve(freqs.size());

    qreal logF_min = f_min == 0 ? 0 : std::log10(f_min);
    qreal logF_max = std::log10(f_max);

    for (qreal f : freqs) {
        // qreal logF = std::log10(f);
        qreal logF = f == 0 ? 0 : std::log10(f);
        qreal scaledLogF = alpha
                           * ((logF - logF_min) / (logF_max - logF_min) * (f_max - f_min) + f_min);
        scaledLogFrequencies.push_back(scaledLogF);
    }

    return scaledLogFrequencies;
}

void GL3DSpectrogramScene::updatePeaks()
{
    // Generiranje novih peakova za prvu liniju
    std::vector<qreal> linePeaks(m_numPoints, 0.0f);
    std::vector<qreal> lineFreqs(m_numPoints, 0.0f);

    FrequencySpectrum::iterator i = m_spectrum.begin();
    const FrequencySpectrum::iterator end = m_spectrum.end();
    // int j = 0;
    // for (; i != end; ++i) {
    //     const FrequencySpectrum::Element e = *i;

    //     linePeaks[j++] = e.amplitude;
    // }

    for (int j = 0; j < m_numPoints && i != end; ++j) {
        const FrequencySpectrum::Element e = *i;
        lineFreqs[j] = e.frequency;
        linePeaks[j] = e.db;
        //linePeaks[j] = e.magnitude * 0.5f;
        //linePeaks[j] = e.amplitude * 10;
        //linePeaks[j] = e.amplitude; //e.magnitude; //e.amplitude * 10;
        i++;
    }

    qreal f_min = lineFreqs[0];     //0 ili 44..
    qreal f_max = lineFreqs.back(); //22050

    //qDebug() << "fmin " << f_min;

    std::vector<qreal> lineFreqsLogScaled = linearToLogFreqs(lineFreqs, f_min, f_max, 1);

    // Generiranje novih peakova za prvu liniju
    // std::vector<qreal> linePeaks(m_numPoints, 0.0f);
    // for (int j = 0; j < m_numPoints; ++j) {
    //     linePeaks[j] = static_cast<float>(std::rand()) / RAND_MAX * m_maxHeight;
    // }

    //QMutexLocker locker(&m_mutex);

    // Pomicanje starih peakova i dodavanje novih
    // peaks.insert(peaks.begin(), linePeaks); // Dodavanje novih peakova na početak --> obrnuti smjer
    m_peaks.push_back(linePeaks); // Dodavanje novih peakova na kraj
    //m_freqs.push_back(lineFreqs);
    m_freqs.push_back(lineFreqsLogScaled);
    if (m_peaks.size() > static_cast<unsigned long long>(m_numLines)) {
        // peaks.pop_back(); // Uklanjanje viška peakova na kraju --> obrnuti smjer
        m_peaks.erase(m_peaks.begin()); // Uklanjanje viška peakova na početku
        m_freqs.erase(m_freqs.begin());
    }

    // glWidget->update();
}

void GL3DSpectrogramScene::mousePressEvent(QMouseEvent *event)
{
    m_lastMousePosition = event->pos();
}

void GL3DSpectrogramScene::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->position().x() - m_lastMousePosition.x();
    int dy = event->position().y() - m_lastMousePosition.y();

    if (event->buttons() & Qt::LeftButton) {
        m_rotationX += dy;
        m_rotationY += dx;
        // rotationZ += dx;
    }

    if (event->buttons() & Qt::MiddleButton) {
        m_positionX += dx * 0.1f;
        m_positionY += dy * 0.1f;
    }

    m_lastMousePosition = event->pos();
    //update();
}

void GL3DSpectrogramScene::wheelEvent(QWheelEvent *event)
{
    m_distance += event->angleDelta().y() / 120.0f; // 120 je korak kotačića miša
    qDebug() << "distance" << m_distance;
    //update();
}
