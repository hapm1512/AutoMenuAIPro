#include "Theme.h"

namespace Theme
{
    juce::FontOptions bold (float size)
    {
        return juce::FontOptions (size, juce::Font::bold);
    }

    juce::FontOptions regular (float size)
    {
        return juce::FontOptions (size, juce::Font::plain);
    }

    void drawInsetPanel (juce::Graphics& g, juce::Rectangle<float> r, float radius)
    {
        g.setColour (juce::Colour (0xaa000000));
        g.fillRoundedRectangle (r.translated (0.0f, 2.0f), radius);

        juce::ColourGradient grad (panelTop, r.getX(), r.getY(), panel, r.getX(), r.getBottom(), false);
        g.setGradientFill (grad);
        g.fillRoundedRectangle (r, radius);

        g.setColour (panelLine);
        g.drawRoundedRectangle (r, radius, 1.0f);

        g.setColour (juce::Colour (0x24ffffff));
        g.drawLine (r.getX() + 8.0f, r.getY() + 1.0f, r.getRight() - 8.0f, r.getY() + 1.0f, 1.0f);
    }

    void drawSectionHeader (juce::Graphics& g, juce::Rectangle<int> r, const juce::String& title)
    {
        auto rf = r.toFloat();
        g.setColour (juce::Colour (0xff0a0b0b));
        g.fillRoundedRectangle (rf.reduced (3.0f), 4.0f);

        g.setColour (gold);
        g.fillRoundedRectangle (rf.removeFromLeft (4.0f).reduced (0.0f, 8.0f), 2.0f);

        g.setColour (textBright);
        g.setFont (bold (15.5f));
        g.drawText (title, r.reduced (14, 0), juce::Justification::centredLeft);
    }
void drawSmallLed (juce::Graphics& g, juce::Rectangle<float> r, bool active, juce::Colour colour)
{
    g.setColour (juce::Colour (0xff030303));
    g.fillEllipse (r.expanded (4.0f));

    g.setColour (active ? colour.withAlpha (0.22f) : juce::Colour (0x22000000));
    g.fillEllipse (r.expanded (9.0f));

    juce::ColourGradient led (active ? colour.brighter (0.35f) : juce::Colour (0xff333333),
                              r.getCentreX(), r.getY(),
                              active ? colour.darker (0.45f) : juce::Colour (0xff101010),
                              r.getCentreX(), r.getBottom(), false);
    g.setGradientFill (led);
    g.fillEllipse (r);

    g.setColour (juce::Colour (0x55ffffff));
    g.fillEllipse (r.reduced (2.0f).withTrimmedBottom (r.getHeight() * 0.45f));
}

}
