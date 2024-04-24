#include "audiolistener.h"

AudioListener::AudioListener(const QAudioFormat &format) : m_format(format)
{

}

void AudioListener::start()
{
    open(QIODevice::OpenModeFlag::WriteOnly);
}

void AudioListener::stop()
{
    close();
}

qint64 AudioListener::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data)
    Q_UNUSED(maxlen)

    return 0;
}

qint64 AudioListener::writeData(const char *data, qint64 len)
{
    QPair <qreal, qreal> pair = calculateLevels(data,len);

    m_level_l = pair.first;
    m_level_r = pair.second;

    emit level_l_Changed(m_level_l);
    emit level_r_Changed(m_level_r);

    return len;
}

QPair<qreal, qreal> AudioListener::calculateLevels(const char *data, qint64 len) const
{
    const int channelBytes = m_format.bytesPerSample();
    const int sampleBytes = m_format.bytesPerFrame();
    const int numSamples = sampleBytes == 0 ? 0 : len / sampleBytes;

    const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);

    float max_l_Value = 0;
    float max_r_Value = 0;

    for (int i = 0; i < numSamples; ++i){
        float value_l = m_format.normalizedSampleValue(ptr);
        max_l_Value = qMax(value_l, max_l_Value);

        ptr += channelBytes;

        float value_r = m_format.normalizedSampleValue(ptr);
        max_r_Value = qMax(value_r, max_r_Value);

        ptr += channelBytes;
    }

    return qMakePair(max_l_Value, max_r_Value);
}
