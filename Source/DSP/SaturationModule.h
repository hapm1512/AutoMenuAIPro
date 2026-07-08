#pragma once
#include "DspModuleBase.h"

class SaturationModule final : public DspModuleBase
{
public:
    void prepare (double sampleRate, int blockSize, int channels) override;
    void reset() override;

    void process (juce::AudioBuffer<float>& buffer,
                  juce::AudioProcessorValueTreeState& apvts) override;

private:
    struct SmoothedParam
    {
        void reset (float v = 0.0f) noexcept { current = v; target = v; }
        void setTarget (float v) noexcept { target = v; }
        float next (float coeff) noexcept
        {
            current += (target - current) * coeff;
            return current;
        }

        float current = 0.0f;
        float target = 0.0f;
    };

    static float softLimit (float x) noexcept;
    static float shapeTape (float x, float drive, float bias) noexcept;
    static float shapeTube (float x, float drive, float bias) noexcept;
    static float shapeTransformer (float x, float drive) noexcept;
    static float highpassFromLowpass (float x, float& low, float coeff) noexcept;

    SmoothedParam driveSm;
    SmoothedParam toneSm;
    SmoothedParam tapeSm;
    SmoothedParam tubeSm;
    SmoothedParam transformerSm;
    SmoothedParam evenSm;
    SmoothedParam oddSm;
    SmoothedParam biasSm;
    SmoothedParam presenceSm;
    SmoothedParam airSm;
    SmoothedParam lowThickSm;
    SmoothedParam widthSm;
    SmoothedParam crossSm;
    SmoothedParam autoGainSm;
    SmoothedParam mixSm;
    SmoothedParam outputSm;

    float toneLp[2] {};
    float presenceLp[2] {};
    float airLp[2] {};
    float lowLp[2] {};
    float dynamicEnv[2] {};
    float previousOutput[2] {};
    float bypassFade = 1.0f;
    uint32_t noiseState = 0x12345678u;
};
