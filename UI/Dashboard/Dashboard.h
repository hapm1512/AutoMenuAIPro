#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

class Dashboard final : public juce::Component
{
public:
    Dashboard();

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::Rectangle<int> headerArea;
    juce::Rectangle<int> macroArea;
    juce::Rectangle<int> mixerArea;
    juce::Rectangle<int> toneArea;
    juce::Rectangle<int> footerArea;
    juce::Rectangle<int> meterArea;

    void drawPanel(
        juce::Graphics& g,
        juce::Rectangle<int> area,
        const juce::String& title
    );
};