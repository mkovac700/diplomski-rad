#include "fileplayer.h"

FilePlayer::FilePlayer(QObject *parent)
    : QIODevice{parent}
{}

void FilePlayer::setSource(const QString &filePath)
{
    m_filePath = filePath;
}

void FilePlayer::setFormat(const QAudioFormat &format)
{
    m_format = format;

    m_sampleCount = m_format.sampleRate() / 100;

    if (m_chunkBuffer.isEmpty()) {
        m_chunkBuffer.reserve(m_sampleCount); //m_sampleCount
        for (int i = 0; i < m_sampleCount; ++i)
            m_chunkBuffer.append(QPointF(i, 0));
    }
}

void FilePlayer::start()
{
    open(QIODevice::ReadOnly);
}

void FilePlayer::stop()
{
    m_pos = 0;
    close();
}

qint64 FilePlayer::readData(char *data, qint64 len)
{
    // qint64 total = 0;
    // if (!m_buffer.isEmpty()) {
    //     while (len - total > 0) {
    //         const qint64 chunk = qMin((m_buffer.size() - m_pos), len - total);
    //         memcpy(data + total, m_buffer.constData() + m_pos, chunk);
    //         m_pos = (m_pos + chunk) % m_buffer.size();
    //         total += chunk;
    //     }
    // }
    // return total;

    qint64 total = 0;
    const qint64 bytesPerFrame = m_format.bytesPerFrame();
    const qint64 chunkSize = m_format.sampleRate() * 0.01; // 10ms worth of samples: 44100/0.01=441
    const qint64 chunkBytes
        = chunkSize
          * m_format.bytesPerFrame(); // Number of bytes in 10ms worth of samples: 441*4=1764

    if (!m_buffer.isEmpty() && m_playbackTime < m_totalDurationUs) {
        while (len - total >= chunkBytes && m_playbackTime < m_totalDurationUs) {
            const qint64 chunk = qMin((m_buffer.size() - m_pos), chunkBytes);
            memcpy(data + total, m_buffer.constData() + m_pos, chunk);
            m_pos = (m_pos + chunk) % m_buffer.size();

            QByteArray chunkData(data + total, chunk);

            //qint64 chunksize = chunkData.size();

            //qDebug() << "chunk: " << chunk << ", chunksize: " << chunksize;

            const char *d
                = chunkData.constData(); //m_buffer.constData() + m_pos; //chunkData.constData();
            const unsigned char *ptr = reinterpret_cast<const unsigned char *>(d);

            for (int i = 0; i < chunkSize; ++i, ptr += bytesPerFrame) { // ili channelBytes * 2
                m_chunkBuffer[i].setY(m_format.normalizedSampleValue(ptr));
            }

            emit bufferChanged(m_chunkBuffer);

            total += chunk;
            m_playbackTime += (chunk * 1000000)
                              / (m_format.sampleRate()
                                 * m_format.bytesPerFrame()); // Time in microseconds
        }
    }
    return total;
}

qint64 FilePlayer::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);

    return 0;
}

qint64 FilePlayer::bytesAvailable() const
{
    return m_buffer.size() + QIODevice::bytesAvailable();
}

bool FilePlayer::loadWavFile()
{
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open WAV file" << m_filePath;
        return false;
    }

    // Read WAV file header
    file.seek(0);
    QByteArray header = file.read(44); // WAV header is 44 bytes

    // Check the "RIFF" chunk descriptor
    if (header.left(4) != "RIFF") {
        qWarning() << "Invalid WAV file: Missing 'RIFF' header";
        return false;
    }

    // Check the "WAVE" format
    if (header.mid(8, 4) != "WAVE") {
        qWarning() << "Invalid WAV file: Missing 'WAVE' format";
        return false;
    }

    // Check the "fmt " subchunk
    if (header.mid(12, 4) != "fmt ") {
        qWarning() << "Invalid WAV file: Missing 'fmt ' subchunk";
        return false;
    }

    // Read the format details
    int audioFormat = *reinterpret_cast<const quint16 *>(header.mid(20, 2).data());
    int numChannels = *reinterpret_cast<const quint16 *>(header.mid(22, 2).data());
    int sampleRate = *reinterpret_cast<const quint32 *>(header.mid(24, 4).data());
    int byteRate = *reinterpret_cast<const quint32 *>(header.mid(28, 4).data());
    int blockAlign = *reinterpret_cast<const quint16 *>(header.mid(32, 2).data());
    int bitsPerSample = *reinterpret_cast<const quint16 *>(header.mid(34, 2).data());

    qInfo() << "Audio format: " << audioFormat;
    qInfo() << "Channel number: " << numChannels;
    qInfo() << "Sample rate: " << sampleRate;
    qInfo() << "byteRate: " << byteRate;
    qInfo() << "block align: " << blockAlign;
    qInfo() << "Bits per sample: " << bitsPerSample;

    if (audioFormat != 1) { // PCM
        qWarning() << "Unsupported WAV file: Only PCM format is supported";
        return false;
    }

    qInfo() << "Chunk header: " << header.mid(36, 4).data();

    //Check the "data" subchunk if (header.mid(36, 4) != "data")
    if (header.mid(36, 4) != "data") {
        qWarning() << "Invalid WAV file: Missing 'data' subchunk";
        return false;
    }

    //Read the data size
    int dataSize = *reinterpret_cast<const quint32 *>(header.mid(40, 4).data());

    qInfo() << "Data size: " << dataSize;

    // Read the audio data
    m_buffer.clear();
    m_buffer.resize(dataSize);
    m_buffer = file.read(dataSize);

    m_totalDurationUs = (dataSize * (qint64) 1000000)
                        / (sampleRate * numChannels * (bitsPerSample / 8));

    m_format.setSampleRate(sampleRate);
    m_format.setChannelCount(numChannels);

    switch (bitsPerSample) {
    case 16:
        m_format.setSampleFormat(QAudioFormat::SampleFormat::Int16);
        break;
    default:
        m_format.setSampleFormat(QAudioFormat::SampleFormat::Unknown);
        break;
    }

    prepareBuffer();

    return true;
}

void FilePlayer::prepareBuffer()
{
    m_sampleCount = m_format.sampleRate() / 100;

    if (m_chunkBuffer.isEmpty()) {
        m_chunkBuffer.reserve(m_sampleCount); //m_sampleCount
        for (int i = 0; i < m_sampleCount; ++i)
            m_chunkBuffer.append(QPointF(i, 0));
    }
}
