#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <array>

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

    using Coeffs = juce::dsp::IIR::Coefficients<float>;

    Coeffs::Ptr getLowShelfCoeffs()   const { return lowShelfCoeffs; }
    Coeffs::Ptr getLowMidPeakCoeffs() const { return lowMidPeakCoeffs; }
    Coeffs::Ptr getHighMidPeakCoeffs() const { return highMidPeakCoeffs; }
    Coeffs::Ptr getHighShelfCoeffs()  const { return highShelfCoeffs; }

    void getMagnitudeResponse(const double* frequencies, double* magnitudes, int numPoints, double sampleRate) const;
    double getCurrentSampleRate() const { return currentSampleRate; }

private:
    using Filter = juce::dsp::IIR::Filter<float>;

    juce::dsp::ProcessorDuplicator<Filter, Coeffs> lowShelf;
    juce::dsp::ProcessorDuplicator<Filter, Coeffs> lowMidPeak;
    juce::dsp::ProcessorDuplicator<Filter, Coeffs> highMidPeak;
    juce::dsp::ProcessorDuplicator<Filter, Coeffs> highShelf;

    Coeffs::Ptr lowShelfCoeffs;
    Coeffs::Ptr lowMidPeakCoeffs;
    Coeffs::Ptr highMidPeakCoeffs;
    Coeffs::Ptr highShelfCoeffs;
    double currentSampleRate = 44100.0;
};
