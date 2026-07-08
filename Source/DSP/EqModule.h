#pragma once
#include "DspModuleBase.h"

class EqModule final : public DspModuleBase
{
public:
    void prepare (double sampleRate, int blockSize, int channels) override;
    void reset() override;

    void process (juce::AudioBuffer<float>& buffer,
                  juce::AudioProcessorValueTreeState& apvts) override;

private:
    struct Biquad
    {
        float b0 { 1.0f }, b1 { 0.0f }, b2 { 0.0f };
        float a1 { 0.0f }, a2 { 0.0f };
        float z1 { 0.0f }, z2 { 0.0f };

        void reset() noexcept;
        float process (float x) noexcept;
        void setBypass() noexcept;
        void setHighPass (float sampleRate, float freq, float q) noexcept;
        void setBell (float sampleRate, float freq, float gainDb, float q) noexcept;
        void setLowShelf (float sampleRate, float freq, float gainDb, float slope) noexcept;
        void setHighShelf (float sampleRate, float freq, float gainDb, float slope) noexcept;
    };

    struct ChannelEq
    {
        Biquad hpf;
        Biquad lowShelf;
        Biquad lowMid;
        Biquad mid;
        Biquad highMid;
        Biquad highShelf;
        Biquad air;
        float dynEnv { 0.0f };
        float makeupEnv { 1.0f };

        void reset() noexcept;
    };

    std::array<ChannelEq, 2> lr;
    std::array<ChannelEq, 2> ms;

    float smoothHpf { 80.0f };
    float smoothLowGain { 0.0f };
    float smoothLowMidGain { 0.0f };
    float smoothMidGain { 0.0f };
    float smoothHighMidGain { 0.0f };
    float smoothHighGain { 0.0f };
    float smoothAirGain { 0.0f };
    float smoothOutput { 1.0f };

    void updateFilters (ChannelEq& c,
                        float hpfFreq,
                        float lowFreq, float lowGain,
                        float lowMidFreq, float lowMidGain, float lowMidQ,
                        float midFreq, float midGain, float midQ,
                        float highMidFreq, float highMidGain, float highMidQ,
                        float highFreq, float highGain,
                        float airFreq, float airGain) noexcept;

    float processEqSample (ChannelEq& c, float x, float dynAmount, float dynThresholdDb) noexcept;
};
