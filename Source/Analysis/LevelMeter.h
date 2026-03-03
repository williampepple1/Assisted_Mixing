#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <atomic>

class LevelMeterData
{
public:
    void process(const juce::AudioBuffer<float>& buffer);

    float getPeakL() const { return peakL.load(); }
    float getPeakR() const { return peakR.load(); }
    float getRmsL()  const { return rmsL.load(); }
    float getRmsR()  const { return rmsR.load(); }

    void setDecayRate(float rate) { decayRate = rate; }

private:
    std::atomic<float> peakL{ 0.0f };
    std::atomic<float> peakR{ 0.0f };
    std::atomic<float> rmsL{ 0.0f };
    std::atomic<float> rmsR{ 0.0f };
    float decayRate = 0.95f;
};
