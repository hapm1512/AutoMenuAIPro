#pragma once
#include <JuceHeader.h>

namespace GuiScale
{
    inline int scaleInt (int v, float scale)
    {
        return juce::roundToInt ((float) v * scale);
    }

    inline juce::Rectangle<int> reducedScaled (juce::Rectangle<int> r, int x, int y, float scale)
    {
        return r.reduced (scaleInt (x, scale), scaleInt (y, scale));
    }
}
