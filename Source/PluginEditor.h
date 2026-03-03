#pragma once

#include "PluginProcessor.h"

class AssistedMixingEditor : public juce::AudioProcessorEditor
{
public:
    explicit AssistedMixingEditor(AssistedMixingProcessor&);
    ~AssistedMixingEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    AssistedMixingProcessor& processorRef;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AssistedMixingEditor)
};
