#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

class MixerPanelComponent final : public juce::Component
{
public:
    MixerPanelComponent();
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    struct KnobItem
    {
        juce::String name;
        int value;
        juce::Colour colour;
    };

    std::vector<KnobItem> knobs;
    void drawKnob (juce::Graphics& g, juce::Rectangle<int> area, const KnobItem& item);
};
