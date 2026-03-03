#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <array>
#include <atomic>

class WaveformBuffer
{
public:
    static constexpr int bufferSize = 1024;

    void pushSamples(const float* data, int numSamples);
    void copyTo(std::array<float, bufferSize>& dest) const;

private:
    std::array<float, bufferSize> bufferA{};
    std::array<float, bufferSize> bufferB{};
    std::atomic<int> writePos{ 0 };
    std::atomic<int> activeBuffer{ 0 };
    std::atomic<bool> snapshotReady{ false };
};
