#include "FooterComponent.h"
#include "../Theme/AppTheme.h"

FooterComponent::FooterComponent()
{
    setInterceptsMouseClicks (true, true);
}

void FooterComponent::paint (juce::Graphics& g)
{
    auto area = getLocalBounds();

    g.setColour (AppTheme::panel());
    g.fillRoundedRectangle (area.toFloat(), 8.0f);

    g.setColour (AppTheme::border());
    g.drawRoundedRectangle (area.toFloat(), 8.0f, 1.0f);

    auto r = area.reduced (10, 0);

    settingsButtonArea = r.removeFromLeft (92).reduced (0, 4);

    g.setColour (AppTheme::panelDark().interpolatedWith (AppTheme::purple(), 0.20f));
    g.fillRoundedRectangle (settingsButtonArea.toFloat(), 6.0f);

    g.setColour (AppTheme::purple());
    g.drawRoundedRectangle (settingsButtonArea.toFloat(), 6.0f, 1.1f);

    g.setColour (AppTheme::text());
    g.setFont (juce::Font (11.5f, juce::Font::bold));
    g.drawText ("SETTINGS", settingsButtonArea, juce::Justification::centred);

    r.removeFromLeft (12);

    g.setColour (AppTheme::subText());
    g.setFont (juce::Font (10.8f));
    g.drawText ("Template: Live Vocal", r.removeFromLeft (138), juce::Justification::centredLeft);

    g.setColour (AppTheme::border());
    g.drawVerticalLine (r.getX(), 6.0f, (float) area.getBottom() - 6.0f);
    r.removeFromLeft (12);

    g.setColour (AppTheme::subText());
    g.drawText ("Shortcut: Analyze / Send Cubase / Export", r.removeFromLeft (245), juce::Justification::centredLeft);

    g.setColour (AppTheme::border());
    g.drawVerticalLine (r.getX(), 6.0f, (float) area.getBottom() - 6.0f);
    r.removeFromLeft (12);

    g.setColour (AppTheme::green());
    g.fillEllipse (r.removeFromLeft (9).withSizeKeepingCentre (7, 7).toFloat());
    r.removeFromLeft (7);

    g.setColour (AppTheme::green());
    g.setFont (juce::Font (10.8f, juce::Font::bold));
    g.drawText ("Cubase Connected", r.removeFromLeft (126), juce::Justification::centredLeft);

    g.setColour (AppTheme::subText());
    g.drawText ("loopMIDI   48kHz   CPU 4%", r, juce::Justification::centredRight);
}

void FooterComponent::resized()
{
}

void FooterComponent::mouseUp (const juce::MouseEvent& event)
{
    if (settingsButtonArea.contains (event.getPosition()))
    {
        if (onSettings)
            onSettings();
    }
}
