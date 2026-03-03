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
    float denom = std::tanh(driveGain);
    if (std::abs(denom) < 1e-6f) return;

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* data = buffer.getWritePointer(ch);
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            float dry = data[i];
            float wet = std::tanh(dry * driveGain) / denom;
            data[i] = dry * (1.0f - mix) + wet * mix;
        }
    }
}

void Saturation::setDrive(float drivePercent)
{
    drive = juce::jlimit(0.0f, 100.0f, drivePercent) / 100.0f;
}

void Saturation::setMix(float mixPercent)
{
    mix = juce::jlimit(0.0f, 100.0f, mixPercent) / 100.0f;
}
