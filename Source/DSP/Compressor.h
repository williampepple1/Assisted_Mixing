#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <atomic>

class CompressorDSP
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::AudioBuffer<float>& buffer);
    void updateParameters(float threshold, float ratio, float attack, float release, float makeup);

    float getGainReduction() const { return gainReductionDB.load(); }
    float getInputLevel()  const { return inputLevelDB.load(); }
    float getOutputLevel() const { return outputLevelDB.load(); }

private:
    juce::dsp::Compressor<float> compressor;
    juce::dsp::Gain<float> makeupGain;

    juce::AudioBuffer<float> preCompBuffer;
    float smoothedGR = 0.0f;
    float currentThreshold = -20.0f;

    std::atomic<float> gainReductionDB{ 0.0f };
    std::atomic<float> inputLevelDB{ -100.0f };
    std::atomic<float> outputLevelDB{ -100.0f };
};
