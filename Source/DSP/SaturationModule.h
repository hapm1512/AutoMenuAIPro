#pragma once
#include "DspModuleBase.h"

class SaturationModule final : public DspModuleBase
{
public:
    void process (juce::AudioBuffer<float>& buffer,
                  juce::AudioProcessorValueTreeState& apvts) override;
};
