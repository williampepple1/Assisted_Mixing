#include "Compressor.h"

void CompressorDSP::prepare(const juce::dsp::ProcessSpec& spec)
{
    compressor.prepare(spec);
    makeupGain.prepare(spec);
    makeupGain.setRampDurationSeconds(0.02);

    preCompBuffer.setSize(static_cast<int>(spec.numChannels),
                          static_cast<int>(spec.maximumBlockSize));
}

void CompressorDSP::process(juce::AudioBuffer<float>& buffer)
{
    if (buffer.getNumSamples() == 0) return;

    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    // Measure input peak
    float peakIn = 0.0f;
    for (int ch = 0; ch < numChannels; ++ch)
    {
        auto range = juce::FloatVectorOperations::findMinAndMax(
            buffer.getReadPointer(ch), numSamples);
        peakIn = std::max(peakIn, std::max(std::abs(range.getStart()), std::abs(range.getEnd())));
    }
    inputLevelDB.store(juce::Decibels::gainToDecibels(peakIn, -100.0f));

    // Ensure pre-comp buffer is large enough (no-alloc path if already sized)
    if (preCompBuffer.getNumChannels() < numChannels || preCompBuffer.getNumSamples() < numSamples)
        preCompBuffer.setSize(numChannels, numSamples, false, false, true);

    // Save pre-compression samples for GR calculation
    for (int ch = 0; ch < numChannels; ++ch)
        preCompBuffer.copyFrom(ch, 0, buffer, ch, 0, numSamples);

    // Run compressor
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    compressor.process(context);

    // Sample-accurate gain reduction measurement
    // Compare pre/post samples to find actual applied GR
    float blockMaxGR = 0.0f;
    const float silenceThresh = 1e-6f;

    for (int ch = 0; ch < numChannels; ++ch)
    {
        const float* pre  = preCompBuffer.getReadPointer(ch);
        const float* post = buffer.getReadPointer(ch);

        for (int i = 0; i < numSamples; ++i)
        {
            float absPre = std::abs(pre[i]);
            if (absPre < silenceThresh)
                continue;

            float gainApplied = post[i] / pre[i];
            float grDB = juce::Decibels::gainToDecibels(std::abs(gainApplied), -100.0f);
            if (grDB < blockMaxGR)
                blockMaxGR = grDB;
        }
    }

    blockMaxGR = juce::jlimit(-60.0f, 0.0f, blockMaxGR);
    smoothedGR = smoothedGR * 0.8f + blockMaxGR * 0.2f;
    gainReductionDB.store(smoothedGR);

    // Apply makeup gain
    makeupGain.process(context);

    // Measure output peak
    float peakOut = 0.0f;
    for (int ch = 0; ch < numChannels; ++ch)
    {
        auto range = juce::FloatVectorOperations::findMinAndMax(
            buffer.getReadPointer(ch), numSamples);
        peakOut = std::max(peakOut, std::max(std::abs(range.getStart()), std::abs(range.getEnd())));
    }
    outputLevelDB.store(juce::Decibels::gainToDecibels(peakOut, -100.0f));
}

void CompressorDSP::updateParameters(float threshold, float ratio, float attack, float release, float makeup)
{
    currentThreshold = threshold;
    compressor.setThreshold(threshold);
    compressor.setRatio(ratio);
    compressor.setAttack(attack);
    compressor.setRelease(release);
    makeupGain.setGainDecibels(makeup);
}
