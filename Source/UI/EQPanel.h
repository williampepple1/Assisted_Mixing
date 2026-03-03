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
    void mouseUp(const juce::MouseEvent& e) override;
    void mouseDoubleClick(const juce::MouseEvent& e) override;
    void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel) override;

private:
    void timerCallback() override;
    void drawGrid(juce::Graphics& g, juce::Rectangle<int> area);
    void drawSpectrum(juce::Graphics& g, juce::Rectangle<int> area,
                      const std::array<float, SpectrumAnalyzer::scopeSize>& data,
                      juce::Colour colour);
    void drawCompositeCurve(juce::Graphics& g, juce::Rectangle<int> area);
    void drawBandCurve(juce::Graphics& g, juce::Rectangle<int> area, int bandIdx, juce::Colour colour);
    void drawBandNodes(juce::Graphics& g, juce::Rectangle<int> area);
    void drawBandInfo(juce::Graphics& g, juce::Rectangle<int> area);
    void showBandContextMenu(int bandIdx);

    float freqToX(float freq, float width) const;
    float xToFreq(float x, float width) const;
    float dbToY(float dB, float height) const;
    float yToDB(float y, float height) const;

    int findNearestBand(float x, float y, juce::Rectangle<int> area);
    juce::Rectangle<int> getVisArea() const;
    juce::Colour getBandColour(int idx) const;
    juce::String getFilterTypeName(int type) const;

    juce::AudioProcessorValueTreeState& apvts;
    SpectrumAnalyzer& preAnalyzer;
    SpectrumAnalyzer& postAnalyzer;
    ParametricEQ& eq;

    std::array<float, SpectrumAnalyzer::scopeSize> preData{};
    std::array<float, SpectrumAnalyzer::scopeSize> postData{};

    int dragBand = -1;
    int selectedBand = -1;
    int hoveredBand = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EQPanel)
};
