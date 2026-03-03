#include "ParametricEQ.h"

void ParametricEQ::prepare(const juce::dsp::ProcessSpec& spec)
{
    lowShelf.prepare(spec);
    lowMidPeak.prepare(spec);
    highMidPeak.prepare(spec);
    highShelf.prepare(spec);
}

void ParametricEQ::process(juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);

    lowShelf.process(context);
    lowMidPeak.process(context);
    highMidPeak.process(context);
    highShelf.process(context);
}

void ParametricEQ::updateBands(float lowFreq, float lowGain,
                                float lowMidFreq, float lowMidGain, float lowMidQ,
                                float highMidFreq, float highMidGain, float highMidQ,
                                float highFreq, float highGain,
                                double sampleRate)
{
    if (sampleRate <= 0.0)
        return;

    *lowShelf.state = *Coeffs::makeLowShelf(
        sampleRate, lowFreq,
        0.707f,
        juce::Decibels::decibelsToGain(lowGain));

    *lowMidPeak.state = *Coeffs::makePeakFilter(
        sampleRate, lowMidFreq, lowMidQ,
        juce::Decibels::decibelsToGain(lowMidGain));

    *highMidPeak.state = *Coeffs::makePeakFilter(
        sampleRate, highMidFreq, highMidQ,
        juce::Decibels::decibelsToGain(highMidGain));

    *highShelf.state = *Coeffs::makeHighShelf(
        sampleRate, highFreq,
        0.707f,
        juce::Decibels::decibelsToGain(highGain));
}
