#ifndef AUDIOLISTENER_H
#define AUDIOLISTENER_H

#include <QAudioSource>

class AudioListener : public QIODevice
{
    Q_OBJECT
public:
    AudioListener(const QAudioFormat &format);

    void start();
    void stop();

    qreal level_l() const { return m_level_l; }
    qreal level_r() const { return m_level_r; }

    // QIODevice interface
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

    //qreal calculateLevel(const char *data, qint64 len) const;
    QPair<qreal, qreal> calculateLevels(const char *data, qint64 len) const;

signals:
    void level_l_Changed(qreal level_l);
    void level_r_Changed(qreal level_r);

private:
    const QAudioFormat m_format;
    qreal m_level_l = 0.0; // 0.0 <= m_level <= 1.0
    qreal m_level_r = 0.0;
};

#endif // AUDIOLISTENER_H
