#include "PluginEditor.h"

AssistedMixingEditor::AssistedMixingEditor(AssistedMixingProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    setSize(600, 500);
}

AssistedMixingEditor::~AssistedMixingEditor() {}

void AssistedMixingEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1e1e2e));
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawFittedText("Assisted Mixing", getLocalBounds().removeFromTop(40),
                     juce::Justification::centred, 1);
}

void AssistedMixingEditor::resized() {}
