#include "WaveformBuffer.h"

void WaveformBuffer::pushSamples(const float* data, int numSamples)
{
    if (!data || numSamples <= 0) return;

    int wp = writePos.load();
    for (int i = 0; i < numSamples; ++i)
    {
        buffer[static_cast<size_t>(wp)] = data[i];
        wp = (wp + 1) % bufferSize;
    }
    writePos.store(wp);
}

void WaveformBuffer::copyTo(std::array<float, bufferSize>& dest) const
{
    int wp = writePos.load();
    for (int i = 0; i < bufferSize; ++i)
        dest[static_cast<size_t>(i)] = buffer[static_cast<size_t>((wp + i) % bufferSize)];
}
