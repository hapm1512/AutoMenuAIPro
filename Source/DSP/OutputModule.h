#pragma once
#include "DspModuleBase.h"

class OutputModule final : public DspModuleBase
{
public:
    void prepare (double sampleRate, int blockSize, int channels) override;
    void reset() override;

    void process (juce::AudioBuffer<float>& buffer,
                  juce::AudioProcessorValueTreeState& apvts) override;

private:
    float limiterGain[2] { 1.0f, 1.0f };
};
