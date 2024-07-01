#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QAudioDecoder>
#include <QAudioDevice>
#include <QAudioOutput>
#include <QMediaDevices>
#include <QMediaPlayer>
#include <QObject>

class AudioPlayer : QObject
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

private:
    const QAudioDevice m_device;

    QScopedPointer<QMediaPlayer> m_player;
    QAudioOutput *audioOutput;

    QAudioDecoder *decoder;
};

#endif // AUDIOPLAYER_H
