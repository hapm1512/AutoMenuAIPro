#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../../../Core/AnalysisTypes.h"

class ToneDetectorComponent final : public juce::Component
{
public:
    ToneDetectorComponent();

    void setAnalysisResult (const AutoMenu::AnalysisResult& result);

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    AutoMenu::AnalysisResult currentResult;
    bool hasLiveResult = false;

    juce::String getToneText() const;
    juce::String getScaleText() const;
    juce::String getBpmText() const;
    juce::String getCamelotText() const;
    float getConfidence() const;

    void drawLedMeter (juce::Graphics& g, juce::Rectangle<int> area);
    void drawButton (juce::Graphics& g, juce::Rectangle<int> area, const juce::String& text, juce::Colour colour);
};
