#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_core/juce_core.h>

#include <array>
#include <deque>

#include "NoiseGate.h"
#include "PitchDetector.h"
#include "KeyDetector.h"
#include "BPMDetector.h"
#include "FFTAnalyzer.h"
#include "../Core/AnalysisTypes.h"
#include "../Core/RealtimeToneState.h"

namespace AutoMenu
{
    class ToneEngine final
    {
    public:
        void prepare (double sampleRateToUse);
        void reset();

        AnalysisResult processBlock (juce::AudioBuffer<float>& monoBlock, double timestampSeconds);
        RealtimeToneState getToneState() const;

    private:
        struct Candidate
        {
            int keyIndex = -1;
            bool minor = false;
            float confidence = 0.0f;
            float pitchHz = 0.0f;
            double timeSeconds = 0.0;
        };

        static int keyIndexFromName (const juce::String& keyName);
        static juce::String scaleName (bool minor);
        static ToneResult makeToneResult (int keyIndex, bool minor, float confidence, float pitchHz, double timeSeconds);

        void updateChromaFromPitch (float hz, float amount);
        void pushCandidate (const ToneResult& raw, double timestampSeconds);
        ToneResult updateStableTone (double timestampSeconds);

        double sampleRate = 48000.0;

        NoiseGate noiseGate;
        PitchDetector pitchDetector;
        KeyDetector keyDetector;
        BPMDetector bpmDetector;

        std::array<float, 12> chroma{};
        std::deque<Candidate> history;

        RealtimeToneState state;
        ToneResult stableTone;

        static constexpr int maxHistoryFrames = 96;
        static constexpr double maxHistorySeconds = 3.0;
        static constexpr float minRawConfidence = 0.20f;
        static constexpr float minStableShare = 0.58f;
        static constexpr float minStableConfidence = 0.50f;
        static constexpr int minStableVotes = 10;
    };
}
