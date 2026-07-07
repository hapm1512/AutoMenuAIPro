#pragma once

#include <juce_core/juce_core.h>
#include <array>

namespace automenu::core
{
    enum class MusicalMode
    {
        unknown,
        major,
        minor
    };

    struct ToneAnalysisResult
    {
        juce::String keyName { "--" };
        MusicalMode mode { MusicalMode::unknown };
        juce::String modeName { "Unknown" };
        juce::String camelot { "--" };
        float confidence { 0.0f };
        float bpm { 0.0f };
        float pitchHz { 0.0f };
        bool hasSignal { false };
        std::array<float, 12> chroma {};

        juce::String displayKey() const
        {
            if (keyName == "--" || mode == MusicalMode::unknown)
                return "--";

            return keyName + " " + modeName;
        }
    };

    inline juce::String modeToString (MusicalMode mode)
    {
        switch (mode)
        {
            case MusicalMode::major: return "Major";
            case MusicalMode::minor: return "Minor";
            default: break;
        }

        return "Unknown";
    }
}
