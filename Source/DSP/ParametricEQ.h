#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_basics/juce_audio_basics.h>

class ParametricEQ
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::AudioBuffer<float>& buffer);

    void updateBands(float lowFreq, float lowGain,
                     float lowMidFreq, float lowMidGain, float lowMidQ,
                     float highMidFreq, float highMidGain, float highMidQ,
                     float highFreq, float highGain,
                     double sampleRate);

private:
    using Filter = juce::dsp::IIR::Filter<float>;
    using Coeffs = juce::dsp::IIR::Coefficients<float>;

    juce::dsp::ProcessorDuplicator<Filter, Coeffs> lowShelf;
    juce::dsp::ProcessorDuplicator<Filter, Coeffs> lowMidPeak;
    juce::dsp::ProcessorDuplicator<Filter, Coeffs> highMidPeak;
    juce::dsp::ProcessorDuplicator<Filter, Coeffs> highShelf;
};
