#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "../Analysis/WaveformBuffer.h"
#include "../DSP/ReverbSend.h"
#include "CustomLookAndFeel.h"

class ReverbPanel : public juce::Component, private juce::Timer
{
public:
    ReverbPanel(juce::AudioProcessorValueTreeState& apvts,
                WaveformBuffer& dryBuf, WaveformBuffer& wetBuf,
                ReverbSend& reverbDSP);
    ~ReverbPanel() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    void timerCallback() override;
    void drawDecayRing(juce::Graphics& g, juce::Rectangle<int> area);
    void drawGroupBox(juce::Graphics& g, juce::Rectangle<int> area, const juce::String& title);
    void drawWaveformOverlay(juce::Graphics& g, juce::Rectangle<int> area);
    void drawModeColorBar(juce::Graphics& g, juce::Rectangle<int> area);

    juce::AudioProcessorValueTreeState& apvts;
    WaveformBuffer& dryBuf;
    WaveformBuffer& wetBuf;
    ReverbSend& reverbDSP;

    std::array<float, WaveformBuffer::bufferSize> dryData{};
    std::array<float, WaveformBuffer::bufferSize> wetData{};

    // Mix / Predelay / Decay
    juce::Slider mixSlider, predelaySlider, decaySlider;
    juce::Label lblMix{"","MIX"}, lblPredelay{"","PREDELAY"}, lblDecay{"","DECAY"};

    // Damping group
    juce::Slider dampHiFreqSlider, dampHiShelfSlider, dampBassFreqSlider, dampBassMultSlider;
    juce::Label lblDampHiFreq{"","HighFreq"}, lblDampHiShelf{"","HighShelf"};
    juce::Label lblDampBassFreq{"","BassFreq"}, lblDampBassMult{"","BassMult"};

    // Shape group
    juce::Slider sizeSlider, attackSlider;
    juce::Label lblSize{"","Size"}, lblAttack{"","Attack"};

    // Diffusion group
    juce::Slider earlyDiffSlider, lateDiffSlider;
    juce::Label lblEarlyDiff{"","Early"}, lblLateDiff{"","Late"};

    // Modulation group
    juce::Slider modRateSlider, modDepthSlider;
    juce::Label lblModRate{"","Rate"}, lblModDepth{"","Depth"};

    // EQ group
    juce::Slider eqHighCutSlider, eqLowCutSlider;
    juce::Label lblEqHighCut{"","HighCut"}, lblEqLowCut{"","LowCut"};

    // Mode / Color
    juce::ComboBox modeBox, colorBox;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modeAttach, colorAttach;

    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> attachments;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbPanel)
};
