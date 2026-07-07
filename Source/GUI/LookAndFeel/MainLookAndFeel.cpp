#include "MainLookAndFeel.h"
#include "GUI/Theme.h"

MainLookAndFeel::MainLookAndFeel()
{
    setColour (juce::TextButton::buttonColourId, juce::Colour (0xff121313));
    setColour (juce::TextButton::buttonOnColourId, Theme::gold);
    setColour (juce::TextButton::textColourOffId, Theme::text);
    setColour (juce::TextButton::textColourOnId, juce::Colours::black);
}

void MainLookAndFeel::drawButtonBackground (juce::Graphics& g,
                                            juce::Button& button,
                                            const juce::Colour&,
                                            bool highlighted,
                                            bool down)
{
    auto r = button.getLocalBounds().toFloat().reduced (1.0f);
    const bool on = button.getToggleState();

    auto top = on ? Theme::gold.brighter (0.25f)
                  : highlighted ? juce::Colour (0xff2a2b2b)
                                : juce::Colour (0xff171818);

    auto bottom = on ? Theme::gold.darker (0.20f)
                     : down ? juce::Colour (0xff090909)
                            : juce::Colour (0xff0d0e0e);

    juce::ColourGradient grad (top, r.getX(), r.getY(),
                               bottom, r.getX(), r.getBottom(), false);

    g.setGradientFill (grad);
    g.fillRoundedRectangle (r, 4.0f);

    g.setColour (on ? juce::Colours::black.withAlpha (0.45f)
                    : juce::Colour (0xff3c3d3d));
    g.drawRoundedRectangle (r, 4.0f, 1.0f);
}
