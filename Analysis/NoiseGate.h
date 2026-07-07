#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

namespace AutoMenu
{
    class NoiseGate final
    {
    public:
        void prepare (double sampleRateToUse);
        void reset();

        void setThresholdDb (float db) noexcept { thresholdDb = db; }
        float getLastRmsDb() const noexcept { return lastRmsDb; }
        bool hasSignal() const noexcept { return signalActive; }

        void processMonoBlock (juce::AudioBuffer<float>& buffer);

    private:
        double sampleRate = 48000.0;
        float thresholdDb = -52.0f;
        float envelope = 0.0f;
        float lastRmsDb = -100.0f;
        bool signalActive = false;
    };
}
