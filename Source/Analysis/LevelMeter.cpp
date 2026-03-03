#include "LevelMeter.h"
#include <cmath>

void LevelMeterData::process(const juce::AudioBuffer<float>& buffer)
{
    auto computePeak = [](const float* data, int n) {
        float peak = 0.0f;
        for (int i = 0; i < n; ++i)
            peak = std::max(peak, std::abs(data[i]));
        return peak;
    };

    auto computeRms = [](const float* data, int n) {
        if (n <= 0) return 0.0f;
        float sum = 0.0f;
        for (int i = 0; i < n; ++i)
            sum += data[i] * data[i];
        return std::sqrt(sum / static_cast<float>(n));
    };

    int n = buffer.getNumSamples();
    if (n <= 0) return;

    if (buffer.getNumChannels() >= 1)
    {
        float p = computePeak(buffer.getReadPointer(0), n);
        float r = computeRms(buffer.getReadPointer(0), n);
        float prevP = peakL.load();
        peakL.store(p > prevP ? p : prevP * decayRate);
        rmsL.store(r);
    }

    if (buffer.getNumChannels() >= 2)
    {
        float p = computePeak(buffer.getReadPointer(1), n);
        float r = computeRms(buffer.getReadPointer(1), n);
        float prevP = peakR.load();
        peakR.store(p > prevP ? p : prevP * decayRate);
        rmsR.store(r);
    }
    else
    {
        peakR.store(peakL.load());
        rmsR.store(rmsL.load());
    }
}
