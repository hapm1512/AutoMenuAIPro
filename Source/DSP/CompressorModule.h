#pragma once
#include "DspModuleBase.h"

class CompressorModule final : public DspModuleBase
{
public:
    void prepare (double sampleRate, int blockSize, int channels) override;
    void reset() override;

    void process (juce::AudioBuffer<float>& buffer,
                  juce::AudioProcessorValueTreeState& apvts) override;

    float getGainReduction() const noexcept { return lastGainReduction; }

private:
    float env[2] {};
    float gain[2] { 1.0f, 1.0f };
    float lastGainReduction = 0.0f;
};
