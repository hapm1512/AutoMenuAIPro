#pragma once
#include <JuceHeader.h>

class DspModuleBase
{
public:
    virtual ~DspModuleBase() = default;

    virtual void prepare (double sampleRate, int blockSize, int channels)
    {
        juce::ignoreUnused (sampleRate, blockSize, channels);
    }

    virtual void reset() {}

    virtual void process (juce::AudioBuffer<float>& buffer,
                          juce::AudioProcessorValueTreeState& apvts)
    {
        juce::ignoreUnused (buffer, apvts);
    }
};
