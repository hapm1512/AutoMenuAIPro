#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

namespace AppTheme
{
    static constexpr int windowWidth  = 1120;
    static constexpr int windowHeight = 640;

    juce::Colour background();
    juce::Colour titleBar();
    juce::Colour panel();
    juce::Colour panelDark();
    juce::Colour border();
    juce::Colour text();
    juce::Colour subText();
    juce::Colour purple();
    juce::Colour blue();
    juce::Colour green();
    juce::Colour yellow();
    juce::Colour red();
    juce::Colour orange();
}
