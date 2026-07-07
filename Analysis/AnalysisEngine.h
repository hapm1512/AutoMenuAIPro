#pragma once

#include <juce_events/juce_events.h>
#include <mutex>
#include "../Audio/AudioRingBuffer.h"
#include "../Core/AnalysisTypes.h"
#include "PitchDetector.h"
#include "FFTProcessor.h"
#include "KeyDetector.h"
#include "BpmDetector.h"

namespace automenu::analysis
{
    class AnalysisEngine final : private juce::Timer
    {
    public:
        explicit AnalysisEngine (automenu::audio::AudioRingBuffer& sourceBuffer);
        ~AnalysisEngine() override;

        void start();
        void stop();

        automenu::core::ToneAnalysisResult getLatestResult() const;

    private:
        void timerCallback() override;
        void analyseBlock();

        automenu::audio::AudioRingBuffer& ringBuffer;
        PitchDetector pitchDetector;
        FFTProcessor fftProcessor;
        KeyDetector keyDetector;
        BpmDetector bpmDetector;

        mutable std::mutex resultMutex;
        automenu::core::ToneAnalysisResult latestResult;
        std::vector<float> analysisBlock;
        double sampleRate = 48000.0;
    };
}
