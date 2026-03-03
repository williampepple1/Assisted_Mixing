#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_basics/juce_audio_basics.h>

class StereoWidthDSP
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::AudioBuffer<float>& buffer);
    void setWidth(float widthPercent);

private:
    float width = 1.0f;  // 1.0 = 100% = normal
};
