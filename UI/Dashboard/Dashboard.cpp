#include "Dashboard.h"
#include "../Theme/AppTheme.h"

Dashboard::Dashboard()
{
    setSize (AppTheme::windowWidth, AppTheme::windowHeight);

    addAndMakeVisible (header);
    addAndMakeVisible (macroPanel);
    addAndMakeVisible (mixerPanel);
    addAndMakeVisible (toneDetector);
    addAndMakeVisible (footer);
    addAndMakeVisible (outputMeter);
}

void Dashboard::paint (juce::Graphics& g)
{
    g.fillAll (AppTheme::background());
}

void Dashboard::resized()
{
    auto r = getLocalBounds().reduced (8);

    header.setBounds (r.removeFromTop (50));
    r.removeFromTop (8);

    auto bottom = r.removeFromBottom (106);
    r.removeFromBottom (8);

    auto right = r.removeFromRight (310);
    r.removeFromRight (8);

    macroPanel.setBounds (r.removeFromTop (230));
    r.removeFromTop (8);
    mixerPanel.setBounds (r);

    outputMeter.setBounds (bottom.removeFromRight (310));
    bottom.removeFromRight (8);
    footer.setBounds (bottom);

    toneDetector.setBounds (right);
}
