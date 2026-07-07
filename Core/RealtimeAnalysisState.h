#pragma once

#include <juce_core/juce_core.h>
#include <cmath>

#include "AnalysisTypes.h"
#include "RealtimeToneState.h"

namespace AutoMenu
{
    struct RealtimeAnalysisState
    {
        juce::String keyName = "--";
        juce::String scaleName = "--";
        juce::String camelot = "--";

        float confidence = 0.0f;
        float pitchHz = 0.0f;
        float bpm = 0.0f;
        double timestampSeconds = 0.0;

        bool valid = false;
        bool stable = false;
        bool signalPresent = false;

        juce::String getToneText() const
        {
            if (! valid || ! stable)
                return "--";

            return keyName + " " + scaleName;
        }

        juce::String getBpmText() const
        {
            if (bpm <= 1.0f)
                return "--";

            return juce::String ((int) std::round (bpm));
        }

        static RealtimeAnalysisState fromToneState (const RealtimeToneState& tone, float bpm = 0.0f)
        {
            RealtimeAnalysisState state;
            state.keyName = tone.keyName;
            state.scaleName = tone.scaleName;
            state.camelot = tone.camelot;
            state.confidence = tone.confidence;
            state.pitchHz = tone.pitchHz;
            state.bpm = bpm;
            state.timestampSeconds = tone.detectedTime;
            state.valid = tone.valid;
            state.stable = tone.stable;
            state.signalPresent = tone.signalPresent;
            return state;
        }

        static RealtimeAnalysisState fromAnalysisResult (const AnalysisResult& result)
        {
            RealtimeAnalysisState state;
            state.keyName = result.tone.keyName;
            state.scaleName = result.tone.scaleName;
            state.camelot = result.tone.camelot;
            state.confidence = juce::jlimit (0.0f, 1.0f, result.tone.confidence);
            state.pitchHz = result.tone.pitchHz;
            state.bpm = result.tone.bpm;
            state.timestampSeconds = result.timestampSeconds;
            state.valid = result.tone.valid;
            state.stable = result.tone.valid && result.tone.confidence >= 0.55f;
            state.signalPresent = result.tone.valid && result.tone.confidence > 0.05f;
            return state;
        }
    };
}
