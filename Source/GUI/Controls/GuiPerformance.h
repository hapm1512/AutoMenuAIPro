#pragma once
#include <JuceHeader.h>

namespace GuiPerformance
{
    inline float getScaleFactorFor (const juce::Component& c)
    {
        if (auto* peer = c.getPeer())
            return (float) peer->getPlatformScaleFactor();
        return 1.0f;
    }

    inline int scaled (const juce::Component& c, int px)
    {
        return juce::roundToInt ((float) px * getScaleFactorFor (c));
    }

    inline void enableBufferedPainting (juce::Component& c)
    {
        c.setBufferedToImage (true);
    }

    inline void repaintIfVisible (juce::Component& c)
    {
        if (c.isShowing())
            c.repaint();
    }
}
