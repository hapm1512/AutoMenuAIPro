#pragma once
#include <JuceHeader.h>

class DspModuleBase
{
public:
    virtual ~DspModuleBase() = default;

    virtual void prepare (double newSampleRate, int newBlockSize, int newChannels)
    {
        sampleRate = juce::jmax (1.0, newSampleRate);
        blockSize = juce::jmax (1, newBlockSize);
        channels = juce::jlimit (1, 2, newChannels);
    }

    virtual void reset() {}

    virtual void process (juce::AudioBuffer<float>& buffer,
                          juce::AudioProcessorValueTreeState& apvts)
    {
        juce::ignoreUnused (buffer, apvts);
    }

protected:
    double sampleRate = 44100.0;
    int blockSize = 512;
    int channels = 2;

    static float getParam (juce::AudioProcessorValueTreeState& apvts,
                           const char* id,
                           float fallback = 0.0f) noexcept
    {
        if (auto* p = apvts.getRawParameterValue (id))
            return p->load();

        return fallback;
    }

    static bool getOn (juce::AudioProcessorValueTreeState& apvts,
                       const char* id,
                       bool fallback = true) noexcept
    {
        if (auto* p = apvts.getRawParameterValue (id))
            return p->load() >= 0.5f;

        return fallback;
    }

    static float dbToGain (float db) noexcept
    {
        return juce::Decibels::decibelsToGain (db);
    }

    static float gainToDb (float gain) noexcept
    {
        return juce::Decibels::gainToDecibels (gain, -120.0f);
    }

    float msToCoeff (float ms) const noexcept
    {
        const auto safeMs = juce::jmax (0.01f, ms);
        return std::exp (-1.0f / (0.001f * safeMs * (float) sampleRate));
    }

    float freqToOnePoleCoeff (float hz) const noexcept
    {
        const auto f = juce::jlimit (5.0f, (float) sampleRate * 0.45f, hz);
        return std::exp (-2.0f * juce::MathConstants<float>::pi * f / (float) sampleRate);
    }
};
