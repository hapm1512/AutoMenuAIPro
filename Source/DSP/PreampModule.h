#pragma once
#include "DspModuleBase.h"

class PreampModule final : public DspModuleBase
{
public:
    void prepare (double sampleRate, int blockSize, int channels) override;
    void reset() override;

    void process (juce::AudioBuffer<float>& buffer,
                  juce::AudioProcessorValueTreeState& apvts) override;

private:
    struct ChannelState
    {
        float dcX1 = 0.0f;
        float dcY1 = 0.0f;
        float memory = 0.0f;
        float lowpass = 0.0f;
        float autoGainEnv = 1.0f;
        float peakEnv = 0.0f;
        float lastOut = 0.0f;
    };

    std::vector<ChannelState> states;
    juce::AudioBuffer<float> dryBuffer;
    juce::Random noiseRandom;

    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> inputTrimGain;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> driveGain;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Multiplicative> outputGain;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> headroomDb;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> transformerAmount;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> tubeAmount;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> harmonicAmount;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> biasAmount;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> memoryAmount;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> stereoVarianceAmount;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> crosstalkAmount;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> noiseAmount;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> autoGainAmount;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> peakProtectAmount;
    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> mixAmount;

    float processOneSample (float input, int channel, float previousChannelSample,
                            float inTrim, float drive, float outGain, float headroom,
                            float transformer, float tube, float harmonic, float bias,
                            float memory, float variance, float crosstalk, float noise,
                            float autoGain, float peakProtect) noexcept;

    static float saturateTransformer (float x, float amount) noexcept;
    static float saturateTube (float x, float amount, float harmonic, float bias) noexcept;
    static float softCompressor (float x, float threshold, float ratio) noexcept;
    static float softClip (float x) noexcept;
};
