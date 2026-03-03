#include "Compressor.h"

void CompressorDSP::prepare(const juce::dsp::ProcessSpec& spec)
{
    compressor.prepare(spec);
    makeupGain.prepare(spec);
    makeupGain.setRampDurationSeconds(0.02);
}

void CompressorDSP::process(juce::AudioBuffer<float>& buffer)
{
    float peakIn = 0.0f;
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto range = juce::FloatVectorOperations::findMinAndMax(
            buffer.getReadPointer(ch), buffer.getNumSamples());
        peakIn = std::max(peakIn, std::max(std::abs(range.getStart()), std::abs(range.getEnd())));
    }
    inputLevelDB.store(juce::Decibels::gainToDecibels(peakIn, -100.0f));

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    compressor.process(context);

    float peakAfterComp = 0.0f;
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto range = juce::FloatVectorOperations::findMinAndMax(
            buffer.getReadPointer(ch), buffer.getNumSamples());
        peakAfterComp = std::max(peakAfterComp, std::max(std::abs(range.getStart()), std::abs(range.getEnd())));
    }

    float grDB = juce::Decibels::gainToDecibels(peakAfterComp, -100.0f) - inputLevelDB.load();
    if (grDB > 0.0f) grDB = 0.0f;
    gainReductionDB.store(grDB);

    makeupGain.process(context);

    float peakOut = 0.0f;
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto range = juce::FloatVectorOperations::findMinAndMax(
            buffer.getReadPointer(ch), buffer.getNumSamples());
        peakOut = std::max(peakOut, std::max(std::abs(range.getStart()), std::abs(range.getEnd())));
    }
    outputLevelDB.store(juce::Decibels::gainToDecibels(peakOut, -100.0f));
}

void CompressorDSP::updateParameters(float threshold, float ratio, float attack, float release, float makeup)
{
    compressor.setThreshold(threshold);
    compressor.setRatio(ratio);
    compressor.setAttack(attack);
    compressor.setRelease(release);
    makeupGain.setGainDecibels(makeup);
}
