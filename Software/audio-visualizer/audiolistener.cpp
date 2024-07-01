#include "audiolistener.h"

AudioListener::AudioListener(const QAudioFormat &format) : m_format(format)
{
    init();
}

void AudioListener::init()
{
    //podaci pristižu svakih 0.01 s, odnosno 10 ms --> 48000 / 100 = 480 sample-ova svakih 10 ms
    m_sampleCount = m_format.sampleRate() / 100;

    if (m_buffer.isEmpty()) {
        m_buffer.reserve(m_sampleCount);
        for (int i = 0; i < m_sampleCount; ++i)
            m_buffer.append(QPointF(i, 0));
    }
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

    writeBuffer(data, len);

    emit bufferChanged(m_buffer);

    return len;
}

QPair<qreal, qreal> AudioListener::calculateLevels(const char *data, qint64 len) const
{
    const int channelBytes = m_format.bytesPerSample();
    const int sampleBytes = m_format.bytesPerFrame();
    const int numSamples = sampleBytes == 0 ? 0 : len / sampleBytes; //1920 / 4 = 480 --> poklapa se sa m_sampleCount

    const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);

    float max_l_Value = 0;
    float max_r_Value = 0;

    for (int i = 0; i < numSamples; ++i){
        for(int i = 0; i < m_format.channelCount(); i++){
            if(i == 0){ //left channel
                float value_l = m_format.normalizedSampleValue(ptr);
                max_l_Value = qMax(value_l, max_l_Value);
            }
            else if (i == 1){ //right channel
                float value_r = m_format.normalizedSampleValue(ptr);
                max_r_Value = qMax(value_r, max_r_Value);
            }

            ptr += channelBytes;
        }
    }

    return qMakePair(max_l_Value, max_r_Value);
}

void AudioListener::writeBuffer(const char *data, qint64 len)
{
    //const int channelBytes = m_format.bytesPerSample(); // 2
    const int sampleBytes = m_format.bytesPerFrame(); // 4
    const int numSamples = sampleBytes == 0 ? 0 : len / sampleBytes; //1920 / 4 = 480 --> poklapa se sa m_sampleCount

    const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);

    for (int i = 0; i < numSamples; ++i, ptr += sampleBytes){ // ili channelBytes * 2
        m_buffer[i].setY(m_format.normalizedSampleValue(ptr));
    }
}
