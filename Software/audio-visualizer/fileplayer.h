#ifndef FILEPLAYER_H
#define FILEPLAYER_H

#include <QAudioFormat>
#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QPointF>
#include <QTime>
#include <QTimer>

class FilePlayer : public QIODevice
{
    Q_OBJECT
public:
    explicit FilePlayer(QObject *parent = nullptr);
    ~FilePlayer();

    void setSource(const QString &filePath);
    void start();
    void stop();

    qint64 readData(char *data, qint64 len) override;
    qint64 writeData(const char *data, qint64 len) override;
    qint64 bytesAvailable() const override;
    qint64 size() const override { return m_buffer.size(); }

    bool loadWavFile();

    QAudioFormat getFormat() { return m_format; }

    void prepareBuffer();
    void bigBufferChanged();

private:
    bool loadWavFile(const QString &filePath, const QAudioFormat &format);

private:
    QString m_filePath;

    qint64 m_pos = 0;
    QByteArray m_buffer;

    qint64 m_playbackTime = 0;
    qint64 m_totalDurationUs;

    QAudioFormat m_format;

    qint64 m_sampleCount;
    QList<QPointF> m_chunkBuffer;
    QList<QPointF> m_bigBuffer;

    QTimer *timer;

signals:
    void bufferChanged(QList<QPointF> buffer);
};

#endif // FILEPLAYER_H
