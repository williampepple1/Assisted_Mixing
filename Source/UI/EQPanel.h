#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "../Analysis/SpectrumAnalyzer.h"
#include "../DSP/ParametricEQ.h"
#include "CustomLookAndFeel.h"

class EQPanel : public juce::Component, private juce::Timer
{
public:
    EQPanel(juce::AudioProcessorValueTreeState& apvts,
            SpectrumAnalyzer& preAnalyzer, SpectrumAnalyzer& postAnalyzer,
            ParametricEQ& eq);
    ~EQPanel() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;

private:
    void timerCallback() override;
    void drawGrid(juce::Graphics& g, juce::Rectangle<int> area);
    void drawSpectrum(juce::Graphics& g, juce::Rectangle<int> area,
                      const std::array<float, SpectrumAnalyzer::scopeSize>& data,
                      juce::Colour colour);
    void drawEQCurve(juce::Graphics& g, juce::Rectangle<int> area);
    void drawBandNodes(juce::Graphics& g, juce::Rectangle<int> area);

    float freqToX(float freq, float width) const;
    float xToFreq(float x, float width) const;
    float dbToY(float dB, float height) const;

    int findNearestBand(float x, float y, juce::Rectangle<int> area);

    juce::AudioProcessorValueTreeState& apvts;
    SpectrumAnalyzer& preAnalyzer;
    SpectrumAnalyzer& postAnalyzer;
    ParametricEQ& eq;

    std::array<float, SpectrumAnalyzer::scopeSize> preData{};
    std::array<float, SpectrumAnalyzer::scopeSize> postData{};

    juce::Slider eqLowFreq, eqLowGain;
    juce::Slider eqLowMidFreq, eqLowMidGain, eqLowMidQ;
    juce::Slider eqHighMidFreq, eqHighMidGain, eqHighMidQ;
    juce::Slider eqHighFreq, eqHighGain;
    juce::Label lblLowFreq{"","Lo F"}, lblLowGain{"","Lo G"};
    juce::Label lblLMFreq{"","LM F"}, lblLMGain{"","LM G"}, lblLMQ{"","LM Q"};
    juce::Label lblHMFreq{"","HM F"}, lblHMGain{"","HM G"}, lblHMQ{"","HM Q"};
    juce::Label lblHiFreq{"","Hi F"}, lblHiGain{"","Hi G"};

    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>> attachments;

    int dragBand = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQPanel)
};
