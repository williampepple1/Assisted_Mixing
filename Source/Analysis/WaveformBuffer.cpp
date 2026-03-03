#include "WaveformBuffer.h"

void WaveformBuffer::pushSamples(const float* data, int numSamples)
{
    if (!data || numSamples <= 0) return;

    int active = activeBuffer.load(std::memory_order_relaxed);
    auto& buf = (active == 0) ? bufferA : bufferB;
    int wp = writePos.load(std::memory_order_relaxed);

    for (int i = 0; i < numSamples; ++i)
    {
        buf[static_cast<size_t>(wp)] = data[i];
        wp = (wp + 1) % bufferSize;
    }

    writePos.store(wp, std::memory_order_relaxed);

    auto& snap = (active == 0) ? bufferB : bufferA;
    int snapWp = wp;
    for (int i = 0; i < bufferSize; ++i)
        snap[static_cast<size_t>(i)] = buf[static_cast<size_t>((snapWp + i) % bufferSize)];

    activeBuffer.store(active == 0 ? 1 : 0, std::memory_order_release);
    snapshotReady.store(true, std::memory_order_release);
}

void WaveformBuffer::copyTo(std::array<float, bufferSize>& dest) const
{
    int active = activeBuffer.load(std::memory_order_acquire);
    const auto& snap = (active == 0) ? bufferA : bufferB;
    std::copy(snap.begin(), snap.end(), dest.begin());
}
