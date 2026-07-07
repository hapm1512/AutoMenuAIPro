#include "FooterComponent.h"
#include "../Theme/AppTheme.h"

FooterComponent::FooterComponent()
{
    setInterceptsMouseClicks (true, true);
}

void FooterComponent::setStatus (const juce::String& workspace,
                                 bool audioConnected,
                                 bool cubaseConnected,
                                 bool performanceMode,
                                 const juce::String& midiName)
{
    workspaceName = workspace.isNotEmpty() ? workspace : "Live Vocal";
    audioOk = audioConnected;
    cubaseOk = cubaseConnected;
    performance = performanceMode;
    midiOutputName = midiName.isNotEmpty() ? midiName : "No MIDI";
    repaint();
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

    g.setColour (performance ? AppTheme::green().withAlpha (0.24f)
                             : AppTheme::panelDark().interpolatedWith (AppTheme::purple(), 0.20f));
    g.fillRoundedRectangle (settingsButtonArea.toFloat(), 6.0f);

    g.setColour (performance ? AppTheme::green() : AppTheme::purple());
    g.drawRoundedRectangle (settingsButtonArea.toFloat(), 6.0f, 1.1f);

    g.setColour (AppTheme::text());
    g.setFont (juce::Font (11.5f, juce::Font::bold));
    g.drawText (performance ? "LIVE" : "SETTINGS", settingsButtonArea, juce::Justification::centred);

    r.removeFromLeft (12);

    g.setColour (AppTheme::subText());
    g.setFont (juce::Font (10.8f));
    g.drawText ("Workspace: " + workspaceName, r.removeFromLeft (160), juce::Justification::centredLeft);

    g.setColour (AppTheme::border());
    g.drawVerticalLine (r.getX(), 6.0f, (float) area.getBottom() - 6.0f);
    r.removeFromLeft (12);

    auto drawLedText = [&] (const juce::String& text, bool ok, int width)
    {
        g.setColour (ok ? AppTheme::green() : AppTheme::red());
        g.fillEllipse (r.removeFromLeft (9).withSizeKeepingCentre (7, 7).toFloat());
        r.removeFromLeft (7);

        g.setColour (ok ? AppTheme::green() : AppTheme::subText());
        g.setFont (juce::Font (10.8f, juce::Font::bold));
        g.drawText (text, r.removeFromLeft (width), juce::Justification::centredLeft);
        r.removeFromLeft (10);
    };

    drawLedText ("Audio", audioOk, 54);
    drawLedText ("Cubase", cubaseOk, 70);

    g.setColour (AppTheme::border());
    g.drawVerticalLine (r.getX(), 6.0f, (float) area.getBottom() - 6.0f);
    r.removeFromLeft (12);

    g.setColour (AppTheme::subText());
    g.setFont (juce::Font (10.8f));
    g.drawText (midiOutputName, r.removeFromLeft (150), juce::Justification::centredLeft);

    g.setColour (performance ? AppTheme::green() : AppTheme::purple());
    g.setFont (juce::Font (10.8f, juce::Font::bold));
    g.drawText (performance ? "Performance Mode" : "Setup Mode", r, juce::Justification::centredRight);
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
