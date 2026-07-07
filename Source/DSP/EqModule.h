#pragma once
#include "DspModuleBase.h"

class EqModule final : public DspModuleBase
{
public:
    void prepare (double sampleRate, int blockSize, int channels) override;
    void reset() override;

    void process (juce::AudioBuffer<float>& buffer,
                  juce::AudioProcessorValueTreeState& apvts) override;

private:
    float lowState[2] {};
    float highState[2] {};
    float airState[2] {};
};
