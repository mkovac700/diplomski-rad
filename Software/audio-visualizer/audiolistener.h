#ifndef AUDIOLISTENER_H
#define AUDIOLISTENER_H

#include <QAudioSource>
#include <QPointF>
#include <QList>

class AudioListener : public QIODevice
{
    Q_OBJECT
public:
    AudioListener(const QAudioFormat &format);

    void start();
    void stop();

    qreal level_l() const { return m_level_l; }
    qreal level_r() const { return m_level_r; }

    QList<QPointF> buffer() {return m_buffer; }

    // QIODevice interface
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

    //qreal calculateLevel(const char *data, qint64 len) const;
    QPair<qreal, qreal> calculateLevels(const char *data, qint64 len) const;
    void writeBuffer(const char *data, qint64 len);

signals:
    void level_l_Changed(qreal level_l);
    void level_r_Changed(qreal level_r);

    void bufferChanged(QList<QPointF> buffer);

private:
    const QAudioFormat m_format;

    qreal m_level_l = 0.0; // 0.0 <= m_level <= 1.0
    qreal m_level_r = 0.0;

    QList<QPointF> m_buffer;

    qint64 m_sampleCount;

private:
    void init();
};

#endif // AUDIOLISTENER_H
