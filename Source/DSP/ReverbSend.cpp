#include "ReverbSend.h"

void ReverbSend::prepare(const juce::dsp::ProcessSpec& spec)
{
    reverb.prepare(spec);
    reverbParams.roomSize = 0.5f;
    reverbParams.damping  = 0.5f;
    reverbParams.wetLevel = 0.0f;
    reverbParams.dryLevel = 1.0f;
    reverbParams.width    = 1.0f;
    reverb.setParameters(reverbParams);
}

void ReverbSend::process(juce::AudioBuffer<float>& buffer)
{
    reverbParams.wetLevel = sendLevel;
    reverbParams.dryLevel = 1.0f;
    reverb.setParameters(reverbParams);

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    reverb.process(context);
}

void ReverbSend::setSendLevel(float levelDB)
{
    if (levelDB <= -59.0f)
        sendLevel = 0.0f;
    else
        sendLevel = juce::Decibels::decibelsToGain(levelDB);
}
