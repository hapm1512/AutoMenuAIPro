#pragma once
#include "DspModuleBase.h"

class OutputModule final : public DspModuleBase
{
public:
    void prepare (double sampleRate, int blockSize, int channels) override;
    void reset() override;

    void process (juce::AudioBuffer<float>& buffer,
                  juce::AudioProcessorValueTreeState& apvts) override;

    float getLimiterGainReductionDb() const noexcept { return lastGainReductionDb; }
    float getRmsLevel() const noexcept { return lastRms; }
    float getCorrelation() const noexcept { return lastCorrelation; }

private:
    struct ChannelState
    {
        float dcX1 = 0.0f;
        float dcY1 = 0.0f;
        float env = 0.0f;
        float gain = 1.0f;
        float dither = 0.0f;
    };

    ChannelState state[2];

    float outputTrimSmoothed = 1.0f;
    float ceilingSmoothed = 0.89125f;
    float thresholdSmoothed = 0.79433f;
    float lufsGainSmoothed = 1.0f;
    float mixSmoothed = 1.0f;

    float lastGainReductionDb = 0.0f;
    float lastRms = 0.0f;
    float lastCorrelation = 0.0f;
    uint32_t noiseSeed = 0x12345678u;

    float processDcBlock (float x, ChannelState& s) const noexcept;
    float softClip (float x, float amount) const noexcept;
    float nextNoise() noexcept;
};
