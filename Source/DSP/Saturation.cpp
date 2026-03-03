#include "Saturation.h"
#include <cmath>

void Saturation::prepare(const juce::dsp::ProcessSpec& spec)
{
    numChannels = static_cast<int>(spec.numChannels);
}

void Saturation::process(juce::AudioBuffer<float>& buffer)
{
    if (drive <= 0.001f)
        return;

    const float driveGain = 1.0f + drive * 9.0f;

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            float dry = data[i];
            float wet = std::tanh(dry * driveGain) / std::tanh(driveGain);
            data[i] = dry * (1.0f - mix) + wet * mix;
        }
    }
}

void Saturation::setDrive(float drivePercent)
{
    drive = drivePercent / 100.0f;
}

void Saturation::setMix(float mixPercent)
{
    mix = mixPercent / 100.0f;
}
