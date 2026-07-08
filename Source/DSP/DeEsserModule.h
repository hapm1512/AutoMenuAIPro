#pragma once
#include "DspModuleBase.h"
#include <array>
#include <vector>

class DeEsserModule final : public DspModuleBase
{
public:
    void prepare (double sampleRate, int blockSize, int channels) override;
    void reset() override;

    void process (juce::AudioBuffer<float>& buffer,
                  juce::AudioProcessorValueTreeState& apvts) override;

private:
    struct ChannelState
    {
        float sideHp = 0.0f;
        float sideLp = 0.0f;
        float splitLp = 0.0f;
        float peakEnv = 0.0f;
        float rmsEnv = 0.0f;
        float noiseFloor = 0.0001f;
        float gain = 1.0f;
        float holdSamples = 0.0f;
        std::vector<float> delay;
        int delayWrite = 0;
    };

    std::array<ChannelState, 2> state;

    int maxDelaySamples = 1;

    static float softKneeOverDb (float inputDb, float thresholdDb, float kneeDb) noexcept;
    static float safeMix (float value) noexcept;

    void ensureDelaySize();
};
