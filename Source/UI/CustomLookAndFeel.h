#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace KingMixerColours
{
    const juce::Colour background     (0xff0d0d1a);
    const juce::Colour panelBg        (0xff141428);
    const juce::Colour headerBg       (0xff0a0a1e);
    const juce::Colour tabActive      (0xff1a1a3e);
    const juce::Colour tabInactive    (0xff0e0e22);
    const juce::Colour accent         (0xff4fc3f7);
    const juce::Colour accentDim      (0xff1a6fa0);
    const juce::Colour accentWarm     (0xffff7043);
    const juce::Colour gridLine       (0xff1e1e3a);
    const juce::Colour textBright     (0xffe0e0e0);
    const juce::Colour textDim        (0xff808090);
    const juce::Colour spectrumPre    (0x554fc3f7);
    const juce::Colour spectrumPost   (0xaa4fc3f7);
    const juce::Colour eqCurve        (0xffffffff);
    const juce::Colour grMeter        (0xffff5252);
    const juce::Colour grTimeline     (0xccff5252);
    const juce::Colour meterGreen     (0xff66bb6a);
    const juce::Colour meterYellow    (0xffffca28);
    const juce::Colour meterRed       (0xffef5350);
    const juce::Colour knobFill       (0xff252545);
    const juce::Colour knobArc        (0xff4fc3f7);
    const juce::Colour knobPointer    (0xffffffff);
}

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel();

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                          juce::Slider& slider) override;

    void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                      int buttonX, int buttonY, int buttonW, int buttonH,
                      juce::ComboBox& box) override;

    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                              const juce::Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override;

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                          bool shouldDrawButtonAsHighlighted,
                          bool shouldDrawButtonAsDown) override;

    juce::Font getComboBoxFont(juce::ComboBox&) override;
    juce::Font getTextButtonFont(juce::TextButton&, int buttonHeight) override;
};
