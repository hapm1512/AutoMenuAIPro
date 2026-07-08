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
    struct FET76
    {
        float detector[2] {};
        float gain[2] { 1.0f, 1.0f };
        float hp[2] {};
    } fet;

    struct Opto2A
    {
        float detector[2] {};
        float optoMemory[2] {};
        float gain[2] { 1.0f, 1.0f };
    } opto;

    float inputSmooth = 1.0f;
    float outputSmooth = 1.0f;
    float mixSmooth = 1.0f;
    float lastGainReduction = 0.0f;

    float process1176Sample (float x, int ch, float thresholdDb, float ratio,
                             float attackMs, float releaseMs, float sidechainHp,
                             float analog, float& grDb) noexcept;

    float processLA2ASample (float x, int ch, float peakReduction,
                             float speed, float analog, float& grDb) noexcept;

    float analogColour (float x, float amount) noexcept;
};
