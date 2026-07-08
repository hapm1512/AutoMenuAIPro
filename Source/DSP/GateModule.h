#pragma once
#include "DspModuleBase.h"

class GateModule final : public DspModuleBase
{
public:
    void prepare (double sampleRate, int blockSize, int channels) override;
    void reset() override;

    void process (juce::AudioBuffer<float>& buffer,
                  juce::AudioProcessorValueTreeState& apvts) override;

private:
    static constexpr int maxChannels = 2;
    static constexpr int maxLookAheadSamples = 4096;

    float rmsEnv[maxChannels] {};
    float peakEnv[maxChannels] {};
    float noiseFloorDb[maxChannels] { -80.0f, -80.0f };
    float gain[maxChannels] { 1.0f, 1.0f };
    float smoothedThresholdDb = -42.0f;
    float makeupGain[maxChannels] { 1.0f, 1.0f };
    float hpState[maxChannels] {};
    float hpPrevIn[maxChannels] {};
    float lpState[maxChannels] {};
    int holdCounter[maxChannels] {};

    juce::AudioBuffer<float> lookAheadBuffer;
    int lookAheadWrite = 0;

    float processSideChain (float x, int ch, float hpHz, float lpHz) noexcept;
    float computeTargetGain (float detectorDb,
                             float thresholdDb,
                             float rangeDb,
                             float hystDb,
                             float kneeDb,
                             float depthPercent,
                             int channel,
                             int holdSamples) noexcept;
};
