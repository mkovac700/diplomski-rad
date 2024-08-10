#include "gl3dspectrogramscene.h"

GL3DSpectrogramScene::GL3DSpectrogramScene(GLWidget *glWidget)
    : GLScene(glWidget)
    , m_rotationX(10)
    , m_rotationY(0)
    , m_rotationZ(0)
    , m_positionX(0)
    , m_positionY(0)
    , m_distance(-20)
{}

void GL3DSpectrogramScene::initialize()
{
    glWidget->resetState();

    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
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

    if (!initialized) {
        // Inicijalizacija peakova (postavljanje svih na 0)
        for (int i = 0; i < m_numLines; ++i) {
            std::vector<qreal> linePeaks(m_numPoints, 0.0f);
            m_peaks.push_back(linePeaks);
        }
        initialized = true;
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

    glColor3f(1.0f, 1.0f, 1.0f); // Bijela boja za linije

    //updatePeaks();

    QMutexLocker locker(&m_mutex);

    glBegin(GL_LINES);
    for (int i = 0; i < m_numLines; ++i) {
        float z = (i - m_numLines / 2) * m_spacingZ;
        for (int j = 0; j < m_numPoints - 1; ++j) {
            float x1 = (j - m_numPoints / 2) * m_spacingX;
            float x2 = ((j + 1) - m_numPoints / 2) * m_spacingX;

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

void GL3DSpectrogramScene::updatePeaks()
{
    // Generiranje novih peakova za prvu liniju
    std::vector<qreal> linePeaks(m_numPoints, 0.0f);

    FrequencySpectrum::iterator i = m_spectrum.begin();
    const FrequencySpectrum::iterator end = m_spectrum.end();
    // int j = 0;
    // for (; i != end; ++i) {
    //     const FrequencySpectrum::Element e = *i;

    //     linePeaks[j++] = e.amplitude;
    // }

    for (int j = 0; j < m_numPoints && i != end; ++j) {
        const FrequencySpectrum::Element e = *i;
        linePeaks[j] = e.amplitude * 10;
        i++;
    }

    // Generiranje novih peakova za prvu liniju
    // std::vector<qreal> linePeaks(m_numPoints, 0.0f);
    // for (int j = 0; j < m_numPoints; ++j) {
    //     linePeaks[j] = static_cast<float>(std::rand()) / RAND_MAX * m_maxHeight;
    // }

    QMutexLocker locker(&m_mutex);

    // Pomicanje starih peakova i dodavanje novih
    // peaks.insert(peaks.begin(), linePeaks); // Dodavanje novih peakova na početak --> obrnuti smjer
    m_peaks.push_back(linePeaks); // Dodavanje novih peakova na kraj
    if (m_peaks.size() > static_cast<unsigned long long>(m_numLines)) {
        // peaks.pop_back(); // Uklanjanje viška peakova na kraju --> obrnuti smjer
        m_peaks.erase(m_peaks.begin()); // Uklanjanje viška peakova na početku
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
