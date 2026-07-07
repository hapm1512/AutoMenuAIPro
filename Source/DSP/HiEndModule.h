#pragma once
#include "DspModuleBase.h"

class HiEndModule final : public DspModuleBase
{
public:
    void prepare (double sampleRate, int blockSize, int channels) override;
    void reset() override;

    void process (juce::AudioBuffer<float>& buffer,
                  juce::AudioProcessorValueTreeState& apvts) override;

private:
    float airLp[2] {};
    float warmLp[2] {};
};
