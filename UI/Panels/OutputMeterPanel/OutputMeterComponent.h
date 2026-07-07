#pragma once
#include <juce_gui_extra/juce_gui_extra.h>

class OutputMeterComponent final : public juce::Component
{
public:
    OutputMeterComponent();
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    void drawLedRow (juce::Graphics& g, juce::Rectangle<int> area, const juce::String& label, float level);
};
