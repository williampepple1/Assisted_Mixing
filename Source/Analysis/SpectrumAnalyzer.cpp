#include "SpectrumAnalyzer.h"

SpectrumAnalyzer::SpectrumAnalyzer()
    : forwardFFT(fftOrder),
      window(fftSize, juce::dsp::WindowingFunction<float>::hann)
{
}

void SpectrumAnalyzer::pushSamples(const float* data, int numSamples)
{
    for (int i = 0; i < numSamples; ++i)
    {
        fifo[static_cast<size_t>(fifoIndex)] = data[i];
        ++fifoIndex;

        if (fifoIndex >= fftSize)
        {
            std::copy(fifo.begin(), fifo.end(), fftData.begin());
            std::fill(fftData.begin() + fftSize, fftData.end(), 0.0f);
            nextBlockReady.store(true);
            fifoIndex = 0;
        }
    }
}

bool SpectrumAnalyzer::getNextBlock(std::array<float, scopeSize>& output)
{
    if (!nextBlockReady.exchange(false))
        return false;

    window.multiplyWithWindowingTable(fftData.data(), fftSize);
    forwardFFT.performFrequencyOnlyForwardTransform(fftData.data());

    const float minDB = -80.0f;
    const float maxDB = 0.0f;
    const float rangeDB = maxDB - minDB;

    for (int i = 0; i < scopeSize; ++i)
    {
        float proportion = static_cast<float>(i) / static_cast<float>(scopeSize);
        int fftIndex = static_cast<int>(std::pow(2.0f, proportion * (std::log2(static_cast<float>(fftSize) / 2.0f))));
        fftIndex = juce::jmin(fftIndex, fftSize / 2 - 1);
        fftIndex = juce::jmax(fftIndex, 0);

        float magnitude = fftData[static_cast<size_t>(fftIndex)];
        float level = juce::jlimit(minDB, maxDB,
            juce::Decibels::gainToDecibels(magnitude / static_cast<float>(fftSize)));
        output[static_cast<size_t>(i)] = (level - minDB) / rangeDB;
    }
    return true;
}
