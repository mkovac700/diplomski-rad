#include "audioplayer.h"

AudioPlayer::AudioPlayer(const QAudioDevice &device)
    : m_device(device)
    , m_player(new QMediaPlayer)
{
    audioOutput = new QAudioOutput;
    decoder = new QAudioDecoder;
}

void AudioPlayer::prepare()
{
    audioOutput->setDevice(m_device);

    m_player->setAudioOutput(audioOutput);
}

void AudioPlayer::setSource(QString path)
{
    m_player->setSource(QUrl::fromLocalFile(path));
}

void AudioPlayer::play(float volume)
{
    audioOutput->setVolume(volume);
    m_player->play();

    //auto buf = decoder->read();
}

void AudioPlayer::pause()
{
    m_player->pause();
}

void AudioPlayer::stop()
{
    m_player->stop();
}
