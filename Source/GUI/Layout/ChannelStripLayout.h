#pragma once
#include <JuceHeader.h>

struct ChannelStripLayout
{
    juce::Rectangle<int> topBar;
    juce::Rectangle<int> modeTabs;
    juce::Rectangle<int> moduleArea;
    juce::Rectangle<int> meterBar;
    juce::Rectangle<int> footer;

    static ChannelStripLayout calculate (juce::Rectangle<int> bounds)
    {
        ChannelStripLayout l;

        auto r = bounds.reduced (10);
        l.topBar = r.removeFromTop (74);
        l.modeTabs = r.removeFromTop (66);

        auto bottom = r.removeFromBottom (166);
        l.footer = bottom.removeFromBottom (38);
        l.meterBar = bottom.reduced (0, 5);

        l.moduleArea = r.reduced (0, 5);
        return l;
    }

    static std::array<int, 10> moduleWidths()
    {
        return { 145, 145, 165, 145, 160, 160, 155, 145, 125, 125 };
    }
};
