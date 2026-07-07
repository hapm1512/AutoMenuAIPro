#include "SuggestionEngine.h"

namespace AutoMenu
{
    SuggestionEngine::SuggestionEngine() = default;

    void SuggestionEngine::setMinimumConfidence (float value)
    {
        minimumConfidence = juce::jlimit (0.0f, 1.0f, value);
    }

    void SuggestionEngine::setMinimumStableHoldSeconds (double seconds)
    {
        stableHoldSeconds = juce::jlimit (0.0, 5.0, seconds);
    }

    void SuggestionEngine::reset()
    {
        const juce::ScopedLock sl (lock);
        latest = {};
        lastTone.clear();
        lastToneChangeSeconds = 0.0;
    }

    SuggestedPreset SuggestionEngine::update (const RealtimeToneState& toneState,
                                              const TonePresetMapping& mapping,
                                              double nowSeconds)
    {
        SuggestedPreset next;
        next.detectedTone = toneState.getToneText();
        next.workspaceName = mapping.getWorkspaceName();
        next.confidence = toneState.confidence;
        next.stable = toneState.stable;
        next.timestampSeconds = nowSeconds;

        const bool toneValid = toneState.valid && toneState.stable && toneState.signalPresent;
        const bool confidenceOk = toneState.confidence >= minimumConfidence;
        const auto toneText = toneState.getToneText();

        if (toneText != lastTone)
        {
            lastTone = toneText;
            lastToneChangeSeconds = nowSeconds;
        }

        const bool heldLongEnough = (nowSeconds - lastToneChangeSeconds) >= stableHoldSeconds;

        if (toneValid && confidenceOk && heldLongEnough)
        {
            const auto entry = mapping.findPresetForTone (toneText);
            next.presetName = entry.presetName;
            next.macroIndex = entry.macroIndex;
            next.canApply = entry.macroIndex >= 0 && entry.presetName != "--";
        }
        else
        {
            next.presetName = "--";
            next.macroIndex = -1;
            next.canApply = false;
        }

        const juce::ScopedLock sl (lock);
        latest = next;
        return latest;
    }

    SuggestedPreset SuggestionEngine::getLatestSuggestion() const
    {
        const juce::ScopedLock sl (lock);
        return latest;
    }
}
