#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_basics/juce_audio_basics.h>

class CompressorDSP
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::AudioBuffer<float>& buffer);
    void updateParameters(float threshold, float ratio, float attack, float release, float makeup);

private:
    juce::dsp::Compressor<float> compressor;
    juce::dsp::Gain<float> makeupGain;
};
