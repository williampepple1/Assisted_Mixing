#include "Compressor.h"

void CompressorDSP::prepare(const juce::dsp::ProcessSpec& spec)
{
    compressor.prepare(spec);
    makeupGain.prepare(spec);
    makeupGain.setRampDurationSeconds(0.02);
}

void CompressorDSP::process(juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    compressor.process(context);
    makeupGain.process(context);
}

void CompressorDSP::updateParameters(float threshold, float ratio, float attack, float release, float makeup)
{
    compressor.setThreshold(threshold);
    compressor.setRatio(ratio);
    compressor.setAttack(attack);
    compressor.setRelease(release);
    makeupGain.setGainDecibels(makeup);
}
