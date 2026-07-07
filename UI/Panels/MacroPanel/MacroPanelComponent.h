#pragma once

#include <array>
#include <functional>
#include <vector>

#include <juce_gui_extra/juce_gui_extra.h>

class MacroPanelComponent final : public juce::Component
{
public:
    MacroPanelComponent();

    void paint (juce::Graphics& g) override;
    void resized() override;
    void mouseUp (const juce::MouseEvent& event) override;

    std::function<void(int)> onMacroPressed;

private:
    enum class IconType
    {
        ai,
        delay,
        fix,
        save,
        lofi,
        off,
        reverb,
        tune,
        music,
        mic,
        remix,
        danca
    };

    struct MacroItem
    {
        juce::String title;
        IconType icon;
        juce::Colour colour;
        bool active = false;
    };

    std::vector<MacroItem> items;
    std::array<juce::Rectangle<int>, 12> macroBounds{};

    void setActiveMacro (int index);

    void drawCard (
        juce::Graphics& g,
        juce::Rectangle<int> area,
        const MacroItem& item,
        int index
    );

    void drawIcon (
        juce::Graphics& g,
        juce::Rectangle<int> area,
        IconType icon,
        juce::Colour colour
    );

    void drawSectionIcon (juce::Graphics& g, juce::Rectangle<int> area);
};
