#pragma once

#include <juce_core/juce_core.h>
#include "AnalysisTypes.h"

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
        bool signalPresent = false;

        juce::String getToneText() const
        {
            if (! valid)
                return "--";

            return keyName + " " + scaleName;
        }

        juce::String getBpmText() const
        {
            if (! valid || bpm <= 1.0f)
                return "--";

            return juce::String ((int) std::round (bpm));
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
            state.signalPresent = result.tone.valid && result.tone.confidence > 0.05f;

            return state;
        }
    };
}
