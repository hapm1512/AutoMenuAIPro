#pragma once

#include <juce_core/juce_core.h>
#include "RealtimeToneState.h"
#include "SuggestionTypes.h"
#include "../Workspace/TonePresetMapping.h"

namespace AutoMenu
{
    class SuggestionEngine final
    {
    public:
        SuggestionEngine();

        void setMinimumConfidence (float value);
        void setMinimumStableHoldSeconds (double seconds);
        void reset();

        SuggestedPreset update (const RealtimeToneState& toneState,
                                const TonePresetMapping& mapping,
                                double nowSeconds);

        SuggestedPreset getLatestSuggestion() const;

    private:
        mutable juce::CriticalSection lock;
        SuggestedPreset latest;

        juce::String lastTone;
        double lastToneChangeSeconds = 0.0;

        float minimumConfidence = 0.78f;
        double stableHoldSeconds = 0.65;
    };
}
