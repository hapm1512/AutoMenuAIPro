#pragma once
#include <juce_gui_extra/juce_gui_extra.h>

class MacroPanelComponent final : public juce::Component
{
public:
    MacroPanelComponent();
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    enum class IconType { ai, delay, fix, save, lofi, off, reverb, tune, music, mic, remix, danca };

    struct MacroItem
    {
        juce::String title;
        IconType icon;
        juce::Colour colour;
        bool active = false;
    };

    std::vector<MacroItem> items;
    void drawCard (juce::Graphics& g, juce::Rectangle<int> area, const MacroItem& item);
    void drawIcon (juce::Graphics& g, juce::Rectangle<int> area, IconType icon, juce::Colour colour);
    void drawSectionIcon (juce::Graphics& g, juce::Rectangle<int> area);
};
