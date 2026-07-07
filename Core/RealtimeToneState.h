#pragma once

#include <juce_core/juce_core.h>
#include <cmath>
#include "AnalysisTypes.h"

namespace AutoMenu
{
    struct RealtimeToneState
    {
        juce::String keyName = "--";
        juce::String scaleName = "--";
        juce::String camelot = "--";

        float confidence = 0.0f;
        float pitchHz = 0.0f;
        bool stable = false;
        bool valid = false;
        bool signalPresent = false;
        double detectedTime = 0.0;

        juce::String getToneText() const
        {
            if (! valid || ! stable)
                return "--";

            return keyName + " " + scaleName;
        }

        juce::String getScaleText() const
        {
            if (! valid || scaleName.isEmpty())
                return "--";

            return scaleName;
        }

        juce::String getConfidenceText() const
        {
            if (! valid || ! stable)
                return "--%";

            return juce::String ((int) std::round (juce::jlimit (0.0f, 1.0f, confidence) * 100.0f)) + "%";
        }

        static RealtimeToneState fromToneResult (const ToneResult& tone, bool hasSignal, bool isStable, double timeSeconds)
        {
            RealtimeToneState state;
            state.keyName = tone.keyName;
            state.scaleName = tone.scaleName;
            state.camelot = tone.camelot;
            state.confidence = juce::jlimit (0.0f, 1.0f, tone.confidence);
            state.pitchHz = tone.pitchHz;
            state.valid = tone.valid;
            state.stable = isStable && tone.valid;
            state.signalPresent = hasSignal;
            state.detectedTime = timeSeconds;
            return state;
        }
    };
}
