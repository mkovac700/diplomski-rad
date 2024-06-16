#include "audioplayer.h"

AudioPlayer::AudioPlayer()
{
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
}

void AudioPlayer::prepare()
{
    const QAudioDevice &defaultOutputDevice = QMediaDevices::defaultAudioOutput();
    audioOutput->setDevice(defaultOutputDevice);

    player->setAudioOutput(audioOutput);
}

void AudioPlayer::setSource(QString path)
{
    player->setSource(QUrl::fromLocalFile(path));
}

void AudioPlayer::play(float volume)
{
    audioOutput->setVolume(volume);
    player->play();
}

void AudioPlayer::pause()
{
    player->pause();
}

void AudioPlayer::stop()
{
    player->stop();
}

QMediaPlayer *AudioPlayer::getPlayer()
{
    return player;
}
