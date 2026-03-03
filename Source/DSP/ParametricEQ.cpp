#include "ParametricEQ.h"

void ParametricEQ::prepare(const juce::dsp::ProcessSpec& spec)
{
    lowShelf.prepare(spec);
    lowMidPeak.prepare(spec);
    highMidPeak.prepare(spec);
    highShelf.prepare(spec);
    currentSampleRate = spec.sampleRate;
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

    currentSampleRate = sampleRate;

    lowShelfCoeffs = Coeffs::makeLowShelf(
        sampleRate, lowFreq, 0.707f,
        juce::Decibels::decibelsToGain(lowGain));
    *lowShelf.state = *lowShelfCoeffs;

    lowMidPeakCoeffs = Coeffs::makePeakFilter(
        sampleRate, lowMidFreq, lowMidQ,
        juce::Decibels::decibelsToGain(lowMidGain));
    *lowMidPeak.state = *lowMidPeakCoeffs;

    highMidPeakCoeffs = Coeffs::makePeakFilter(
        sampleRate, highMidFreq, highMidQ,
        juce::Decibels::decibelsToGain(highMidGain));
    *highMidPeak.state = *highMidPeakCoeffs;

    highShelfCoeffs = Coeffs::makeHighShelf(
        sampleRate, highFreq, 0.707f,
        juce::Decibels::decibelsToGain(highGain));
    *highShelf.state = *highShelfCoeffs;
}

void ParametricEQ::getMagnitudeResponse(const double* frequencies, double* magnitudes, int numPoints, double sampleRate) const
{
    for (int i = 0; i < numPoints; ++i)
        magnitudes[i] = 1.0;

    if (sampleRate <= 0.0)
        return;

    auto multiplyMagnitudes = [&](Coeffs::Ptr c) {
        if (c == nullptr) return;
        for (int i = 0; i < numPoints; ++i)
            magnitudes[i] *= c->getMagnitudeForFrequency(frequencies[i], sampleRate);
    };

    multiplyMagnitudes(lowShelfCoeffs);
    multiplyMagnitudes(lowMidPeakCoeffs);
    multiplyMagnitudes(highMidPeakCoeffs);
    multiplyMagnitudes(highShelfCoeffs);
}
