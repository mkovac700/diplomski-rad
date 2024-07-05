#include "audioplayer.h"

AudioPlayer::AudioPlayer(const QAudioDevice &device)
    : m_device(device)
    , m_player(new QMediaPlayer)
{
    audioOutput = new QAudioOutput;
    decoder.reset(new QAudioDecoder);

    connect(decoder.data(), &QAudioDecoder::bufferReady, this, &AudioPlayer::readData);
}

void AudioPlayer::prepare()
{
    audioOutput->setDevice(m_device);

    m_player->setAudioOutput(audioOutput);

    QAudioFormat format;
    format.setSampleRate(m_device.preferredFormat().sampleRate()); //8000 //m_sample_rate
    format.setChannelCount(2);
    format.setSampleFormat(QAudioFormat::Int16);

    decoder->setAudioFormat(format);
}

void AudioPlayer::setSource(QString path)
{
    m_player->setSource(QUrl::fromLocalFile(path));
    decoder->setSource(QUrl::fromLocalFile(path));
}

void AudioPlayer::play(float volume)
{
    audioOutput->setVolume(volume);
    m_player->play();

    decoder->start();

    //auto buf = decoder->read();
}

void AudioPlayer::pause()
{
    m_player->pause();
}

void AudioPlayer::stop()
{
    m_player->stop();
    decoder->stop();
}

void AudioPlayer::readData()
{
    auto buff = decoder->read();

    auto size = buff.byteCount();          //4392
    auto sampleCount = buff.sampleCount(); //2196
    auto frameCount = buff.frameCount();   //1098 = num samples (4392/4)

    const char *data = buff.constData<char>();

    const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);
}
