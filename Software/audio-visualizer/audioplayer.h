#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QAudioDecoder>
#include <QAudioDevice>
#include <QAudioOutput>
#include <QMediaDevices>
#include <QMediaPlayer>
#include <QObject>

class AudioPlayer : public QObject
{
    Q_OBJECT
public:
    AudioPlayer(const QAudioDevice &device);

    void prepare();
    void setSource(QString path);
    void play(float volume = 50);
    void pause();
    void stop();

    QMediaPlayer *player() const { return m_player.get(); }

public slots:
    void readData();

private:
    const QAudioDevice m_device;

    QScopedPointer<QMediaPlayer> m_player;
    QAudioOutput *audioOutput;

    QScopedPointer<QAudioDecoder> decoder;
};

#endif // AUDIOPLAYER_H
