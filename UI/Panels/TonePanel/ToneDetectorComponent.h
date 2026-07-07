#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../../Core/AnalysisTypes.h"
#include "../../../Core/RealtimeAnalysisState.h"
#include "../../../Core/RealtimeToneState.h"
#include "../../../Core/SuggestionTypes.h"
#include <functional>

class ToneDetectorComponent final : public juce::Component
{
public:
    ToneDetectorComponent();

    void setAnalysisResult (const AutoMenu::AnalysisResult& result);
    void setRealtimeState (const AutoMenu::RealtimeAnalysisState& state);
    void setRealtimeToneState (const AutoMenu::RealtimeToneState& state);
    void setSuggestionState (const AutoMenu::SuggestedPreset& suggestion);

    std::function<void()> onApplySuggestion;

    void paint (juce::Graphics& g) override;
    void resized() override;
    void mouseUp (const juce::MouseEvent& event) override;

private:
    AutoMenu::RealtimeToneState currentState;
    AutoMenu::SuggestedPreset suggestionState;
    bool hasLiveResult = false;
    juce::Rectangle<int> applyButtonBounds;

    juce::String getToneText() const;
    juce::String getScaleText() const;
    juce::String getBpmText() const;
    juce::String getCamelotText() const;
    float getConfidence() const;

    void drawLedMeter (juce::Graphics& g, juce::Rectangle<int> area);
    void drawButton (juce::Graphics& g, juce::Rectangle<int> area, const juce::String& text, juce::Colour colour);
};
