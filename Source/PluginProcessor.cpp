#include "PluginProcessor.h"
#include "PluginEditor.h"

AssistedMixingProcessor::AssistedMixingProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
{
    inputGainParam    = apvts.getRawParameterValue("inputGain");
    outputGainParam   = apvts.getRawParameterValue("outputGain");
    eqLowFreqParam    = apvts.getRawParameterValue("eqLowFreq");
    eqLowGainParam    = apvts.getRawParameterValue("eqLowGain");
    eqLowMidFreqParam = apvts.getRawParameterValue("eqLowMidFreq");
    eqLowMidGainParam = apvts.getRawParameterValue("eqLowMidGain");
    eqLowMidQParam    = apvts.getRawParameterValue("eqLowMidQ");
    eqHighMidFreqParam = apvts.getRawParameterValue("eqHighMidFreq");
    eqHighMidGainParam = apvts.getRawParameterValue("eqHighMidGain");
    eqHighMidQParam   = apvts.getRawParameterValue("eqHighMidQ");
    eqHighFreqParam   = apvts.getRawParameterValue("eqHighFreq");
    eqHighGainParam   = apvts.getRawParameterValue("eqHighGain");
    compThresholdParam = apvts.getRawParameterValue("compThreshold");
    compRatioParam    = apvts.getRawParameterValue("compRatio");
    compAttackParam   = apvts.getRawParameterValue("compAttack");
    compReleaseParam  = apvts.getRawParameterValue("compRelease");
    compMakeupParam   = apvts.getRawParameterValue("compMakeup");
    satDriveParam     = apvts.getRawParameterValue("satDrive");
    satMixParam       = apvts.getRawParameterValue("satMix");
    stereoWidthParam  = apvts.getRawParameterValue("stereoWidth");
    reverbSendParam   = apvts.getRawParameterValue("reverbSend");
    reverbRoomSizeParam = apvts.getRawParameterValue("reverbRoomSize");
    reverbDampingParam  = apvts.getRawParameterValue("reverbDamping");
    mixAmountParam    = apvts.getRawParameterValue("mixAmount");
    bypassParam       = apvts.getRawParameterValue("bypass");
}

AssistedMixingProcessor::~AssistedMixingProcessor() {}

const juce::String AssistedMixingProcessor::getName() const { return JucePlugin_Name; }
bool AssistedMixingProcessor::acceptsMidi() const { return false; }
bool AssistedMixingProcessor::producesMidi() const { return false; }
bool AssistedMixingProcessor::isMidiEffect() const { return false; }
double AssistedMixingProcessor::getTailLengthSeconds() const { return 0.0; }
int AssistedMixingProcessor::getNumPrograms() { return 1; }
int AssistedMixingProcessor::getCurrentProgram() { return 0; }
void AssistedMixingProcessor::setCurrentProgram(int) {}
const juce::String AssistedMixingProcessor::getProgramName(int) { return {}; }
void AssistedMixingProcessor::changeProgramName(int, const juce::String&) {}

void AssistedMixingProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32>(getTotalNumOutputChannels());

    gainStage.prepare(spec);
    parametricEQ.prepare(spec);
    compressor.prepare(spec);
    saturation.prepare(spec);
    stereoWidth.prepare(spec);
    reverbSend.prepare(spec);

    preEQAnalyzer.setSampleRate(sampleRate);
    postEQAnalyzer.setSampleRate(sampleRate);
}

void AssistedMixingProcessor::releaseResources() {}

bool AssistedMixingProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void AssistedMixingProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    if (bypassParam->load() > 0.5f)
        return;

    const float mix = mixAmountParam->load();

    juce::AudioBuffer<float> dryBuffer;
    if (mix < 1.0f)
        dryBuffer.makeCopyOf(buffer);

    inputMeter.process(buffer);

    gainStage.setGainDB(inputGainParam->load());
    gainStage.process(buffer);

    // Pre-EQ spectrum
    if (buffer.getNumChannels() > 0)
        preEQAnalyzer.pushSamples(buffer.getReadPointer(0), buffer.getNumSamples());

    parametricEQ.updateBands(
        eqLowFreqParam->load(), eqLowGainParam->load(),
        eqLowMidFreqParam->load(), eqLowMidGainParam->load(), eqLowMidQParam->load(),
        eqHighMidFreqParam->load(), eqHighMidGainParam->load(), eqHighMidQParam->load(),
        eqHighFreqParam->load(), eqHighGainParam->load(),
        getSampleRate());
    parametricEQ.process(buffer);

    // Post-EQ spectrum
    if (buffer.getNumChannels() > 0)
        postEQAnalyzer.pushSamples(buffer.getReadPointer(0), buffer.getNumSamples());

    compressor.updateParameters(
        compThresholdParam->load(), compRatioParam->load(),
        compAttackParam->load(), compReleaseParam->load(),
        compMakeupParam->load());
    compressor.process(buffer);

    // Pre-saturation waveform
    if (buffer.getNumChannels() > 0)
        preSatBuffer.pushSamples(buffer.getReadPointer(0), buffer.getNumSamples());

    saturation.setDrive(satDriveParam->load());
    saturation.setMix(satMixParam->load());
    saturation.process(buffer);

    // Post-saturation waveform
    if (buffer.getNumChannels() > 0)
        postSatBuffer.pushSamples(buffer.getReadPointer(0), buffer.getNumSamples());

    stereoWidth.setWidth(stereoWidthParam->load());
    stereoWidth.process(buffer);

    // Pre-reverb waveform
    if (buffer.getNumChannels() > 0)
        dryRevBuffer.pushSamples(buffer.getReadPointer(0), buffer.getNumSamples());

    reverbSend.setSendLevel(reverbSendParam->load());
    reverbSend.setRoomSize(reverbRoomSizeParam->load());
    reverbSend.setDamping(reverbDampingParam->load());
    reverbSend.process(buffer);

    // Post-reverb waveform
    if (buffer.getNumChannels() > 0)
        wetRevBuffer.pushSamples(buffer.getReadPointer(0), buffer.getNumSamples());

    float outGainLinear = juce::Decibels::decibelsToGain(outputGainParam->load());
    buffer.applyGain(outGainLinear);

    if (mix < 1.0f)
    {
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        {
            auto* wet = buffer.getWritePointer(ch);
            const auto* dry = dryBuffer.getReadPointer(ch);
            for (int i = 0; i < buffer.getNumSamples(); ++i)
                wet[i] = dry[i] * (1.0f - mix) + wet[i] * mix;
        }
    }

    outputMeter.process(buffer);
}

void AssistedMixingProcessor::applyRule(Genre genre, Instrument instrument)
{
    MixRule rule = MixRuleDatabase::getRule(genre, instrument);

    auto setParam = [&](const juce::String& id, float value) {
        auto* p = apvts.getParameter(id);
        p->setValueNotifyingHost(p->convertTo0to1(value));
    };

    setParam("inputGain", rule.inputGain);
    setParam("outputGain", rule.outputGain);
    setParam("eqLowFreq", rule.eqLowFreq);
    setParam("eqLowGain", rule.eqLowGain);
    setParam("eqLowMidFreq", rule.eqLowMidFreq);
    setParam("eqLowMidGain", rule.eqLowMidGain);
    setParam("eqLowMidQ", rule.eqLowMidQ);
    setParam("eqHighMidFreq", rule.eqHighMidFreq);
    setParam("eqHighMidGain", rule.eqHighMidGain);
    setParam("eqHighMidQ", rule.eqHighMidQ);
    setParam("eqHighFreq", rule.eqHighFreq);
    setParam("eqHighGain", rule.eqHighGain);
    setParam("compThreshold", rule.compThreshold);
    setParam("compRatio", rule.compRatio);
    setParam("compAttack", rule.compAttack);
    setParam("compRelease", rule.compRelease);
    setParam("compMakeup", rule.compMakeup);
    setParam("satDrive", rule.satDrive);
    setParam("satMix", rule.satMix);
    setParam("stereoWidth", rule.stereoWidth);
    setParam("reverbSend", rule.reverbSend);
}

juce::AudioProcessorValueTreeState::ParameterLayout AssistedMixingProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        "genre", "Genre", juce::StringArray{
            "Rock", "Pop", "Hip-Hop", "EDM", "Jazz", "R&B", "Metal", "Classical"}, 0));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        "instrument", "Instrument", juce::StringArray{
            "Vocals", "Drums", "Bass", "Electric Guitar", "Acoustic Guitar",
            "Keys/Synths", "Strings", "Brass"}, 0));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "mixAmount", "Mix Amount", 0.0f, 1.0f, 1.0f));

    params.push_back(std::make_unique<juce::AudioParameterBool>(
        "bypass", "Bypass", false));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "inputGain", "Input Gain",
        juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f), 0.0f, "dB"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "outputGain", "Output Gain",
        juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f), 0.0f, "dB"));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "eqLowFreq", "EQ Low Freq",
        juce::NormalisableRange<float>(20.0f, 500.0f, 1.0f, 0.5f), 80.0f, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "eqLowGain", "EQ Low Gain",
        juce::NormalisableRange<float>(-18.0f, 18.0f, 0.1f), 0.0f, "dB"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "eqLowMidFreq", "EQ Low-Mid Freq",
        juce::NormalisableRange<float>(100.0f, 2000.0f, 1.0f, 0.5f), 400.0f, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "eqLowMidGain", "EQ Low-Mid Gain",
        juce::NormalisableRange<float>(-18.0f, 18.0f, 0.1f), 0.0f, "dB"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "eqLowMidQ", "EQ Low-Mid Q",
        juce::NormalisableRange<float>(0.1f, 10.0f, 0.01f, 0.5f), 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "eqHighMidFreq", "EQ High-Mid Freq",
        juce::NormalisableRange<float>(500.0f, 8000.0f, 1.0f, 0.5f), 2500.0f, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "eqHighMidGain", "EQ High-Mid Gain",
        juce::NormalisableRange<float>(-18.0f, 18.0f, 0.1f), 0.0f, "dB"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "eqHighMidQ", "EQ High-Mid Q",
        juce::NormalisableRange<float>(0.1f, 10.0f, 0.01f, 0.5f), 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "eqHighFreq", "EQ High Freq",
        juce::NormalisableRange<float>(2000.0f, 20000.0f, 1.0f, 0.5f), 8000.0f, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "eqHighGain", "EQ High Gain",
        juce::NormalisableRange<float>(-18.0f, 18.0f, 0.1f), 0.0f, "dB"));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "compThreshold", "Comp Threshold",
        juce::NormalisableRange<float>(-60.0f, 0.0f, 0.1f), -20.0f, "dB"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "compRatio", "Comp Ratio",
        juce::NormalisableRange<float>(1.0f, 20.0f, 0.1f, 0.5f), 2.0f, ":1"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "compAttack", "Comp Attack",
        juce::NormalisableRange<float>(0.1f, 200.0f, 0.1f, 0.5f), 10.0f, "ms"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "compRelease", "Comp Release",
        juce::NormalisableRange<float>(5.0f, 1000.0f, 1.0f, 0.5f), 100.0f, "ms"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "compMakeup", "Comp Makeup",
        juce::NormalisableRange<float>(-6.0f, 24.0f, 0.1f), 0.0f, "dB"));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "satDrive", "Sat Drive",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f, "%"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "satMix", "Sat Mix",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 50.0f, "%"));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "stereoWidth", "Stereo Width",
        juce::NormalisableRange<float>(0.0f, 200.0f, 1.0f), 100.0f, "%"));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "reverbSend", "Reverb Send",
        juce::NormalisableRange<float>(-60.0f, 0.0f, 0.1f), -60.0f, "dB"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "reverbRoomSize", "Room Size",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "reverbDamping", "Damping",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));

    return { params.begin(), params.end() };
}

void AssistedMixingProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AssistedMixingProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml != nullptr && xml->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessorEditor* AssistedMixingProcessor::createEditor()
{
    return new AssistedMixingEditor(*this);
}

bool AssistedMixingProcessor::hasEditor() const { return true; }

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AssistedMixingProcessor();
}
