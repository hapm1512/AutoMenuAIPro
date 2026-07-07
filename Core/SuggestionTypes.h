#pragma once

#include <juce_core/juce_core.h>

namespace AutoMenu
{
    struct SuggestedPreset
    {
        juce::String detectedTone = "--";
        juce::String presetName = "--";
        juce::String workspaceName = "Live";
        int macroIndex = -1;
        float confidence = 0.0f;
        bool canApply = false;
        bool stable = false;
        double timestampSeconds = 0.0;

        juce::String getConfidenceText() const
        {
            if (confidence <= 0.0f)
                return "--%";

            return juce::String ((int) juce::roundToInt (juce::jlimit (0.0f, 1.0f, confidence) * 100.0f)) + "%";
        }
    };
}
