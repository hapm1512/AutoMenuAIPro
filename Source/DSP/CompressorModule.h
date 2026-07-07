#pragma once
#include "DspModuleBase.h"

class CompressorModule final : public DspModuleBase
{
public:
    void process (juce::AudioBuffer<float>& buffer,
                  juce::AudioProcessorValueTreeState& apvts) override;
};
