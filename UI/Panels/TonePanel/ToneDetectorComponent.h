#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

class ToneDetectorComponent final : public juce::Component
{
public:
    ToneDetectorComponent();

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    void drawLedMeter (juce::Graphics& g, juce::Rectangle<int> area);
    void drawButton (juce::Graphics& g, juce::Rectangle<int> area, const juce::String& text, juce::Colour colour);
};
