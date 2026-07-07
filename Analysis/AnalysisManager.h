#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>

#include "FFTAnalyzer.h"
#include "ToneEngine.h"
#include "../Core/AnalysisTypes.h"
#include "../Core/RealtimeToneState.h"

namespace AutoMenu
{
    class AnalysisManager final : private juce::Timer
    {
    public:
        AnalysisManager();
        ~AnalysisManager() override;

        void prepare (double sampleRateToUse, int blockSizeToUse);
        void pushAudioBlock (const float* const* input, int channels, int samples, double sampleRateToUse);

        AnalysisResult getLatestResult() const;
        RealtimeToneState getLatestToneState() const;
        void reset();

    private:
        void timerCallback() override;
        void analyzePendingBuffer();

        mutable juce::CriticalSection lock;

        juce::AudioBuffer<float> pendingMono;
        int pendingWrite = 0;
        bool hasEnoughSamples = false;

        double sampleRate = 48000.0;
        int blockSize = 512;
        double timestamp = 0.0;

        FFTAnalyzer fftAnalyzer;
        ToneEngine toneEngine;

        AnalysisResult latest;
        RealtimeToneState latestTone;
    };
}
