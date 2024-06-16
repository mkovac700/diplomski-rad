#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QAudioDevice>
#include <QAudioOutput>
#include <QMediaDevices>
#include <QMediaPlayer>
#include <QObject>

class AudioPlayer : QObject
{
    Q_OBJECT
public:
    AudioPlayer();

    void prepare();
    void setSource(QString path);
    void play(float volume = 50);
    void pause();
    void stop();

    QMediaPlayer *getPlayer();

private:
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
};

#endif // AUDIOPLAYER_H
