#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

class MacroPanelComponent final : public juce::Component
{
public:
    MacroPanelComponent();
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    struct MacroItem
    {
        juce::String title;
        juce::String icon;
        juce::Colour colour;
        bool active = false;
    };

    std::vector<MacroItem> items;
    void drawCard (juce::Graphics& g, juce::Rectangle<int> area, const MacroItem& item);
};
