#include "GainStage.h"

void GainStage::prepare(const juce::dsp::ProcessSpec& spec)
{
    gain.prepare(spec);
    gain.setRampDurationSeconds(0.02);
}

void GainStage::process(juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    gain.process(context);
}

void GainStage::setGainDB(float dB)
{
    gain.setGainDecibels(dB);
}
