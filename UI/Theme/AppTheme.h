#pragma once
#include <juce_gui_extra/juce_gui_extra.h>

namespace AppTheme
{
    constexpr float scale = 0.85f;

    inline int sx (int v) { return (int) std::round ((float) v * scale); }

    constexpr int windowWidth  = 1100;
    constexpr int windowHeight = 560;
    constexpr int minWidth     = 980;
    constexpr int minHeight    = 500;
    constexpr int maxWidth     = 1280;
    constexpr int maxHeight    = 720;

    const juce::Colour background();
    const juce::Colour panel();
    const juce::Colour panelDark();
    const juce::Colour border();
    const juce::Colour text();
    const juce::Colour subText();
    const juce::Colour purple();
    const juce::Colour blue();
    const juce::Colour green();
    const juce::Colour red();
    const juce::Colour yellow();
}
