#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_basics/juce_audio_basics.h>

class ReverbSend
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec);
    void process(juce::AudioBuffer<float>& buffer);
    void setSendLevel(float levelDB);
    void setRoomSize(float size);
    void setDamping(float damp);

    float getRoomSize() const { return reverbParams.roomSize; }
    float getDamping()  const { return reverbParams.damping; }
    float getSendLevel() const { return sendLevel; }

private:
    juce::dsp::Reverb reverb;
    float sendLevel = 0.0f;
    juce::dsp::Reverb::Parameters reverbParams;
};
