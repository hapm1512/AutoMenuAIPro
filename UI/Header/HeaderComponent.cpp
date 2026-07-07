#include "HeaderComponent.h"
#include "../Theme/AppTheme.h"

HeaderComponent::HeaderComponent() {}

void HeaderComponent::paint (juce::Graphics& g)
{
    g.fillAll (AppTheme::background());
    drawLogo (g);

    auto title = titleArea;
    g.setColour (AppTheme::text());
    g.setFont (juce::Font (14.0f, juce::Font::bold));
    g.drawText ("AUTO AUTOTUNE AI PRO", title.removeFromTop (18), juce::Justification::left);

    g.setColour (AppTheme::subText());
    g.setFont (juce::Font (9.0f));
    g.drawText ("ULTRA MINI", title, juce::Justification::left);

    drawStatusBox (g);
    drawActionButtons (g);
}

void HeaderComponent::resized()
{
    auto r = getLocalBounds();
    logoArea = r.removeFromLeft (46).reduced (7, 5);
    titleArea = r.removeFromLeft (195).reduced (4, 6);
    r.removeFromLeft (16);
    statusArea = r.removeFromLeft (300).reduced (0, 5);
    r.removeFromLeft (14);
    actionArea = r.reduced (0, 5);
}

void HeaderComponent::drawLogo (juce::Graphics& g)
{
    auto b = logoArea.toFloat();
    g.setColour (AppTheme::panel());
    g.fillRoundedRectangle (b, 7.0f);
    g.setColour (AppTheme::purple());
    g.drawRoundedRectangle (b, 7.0f, 1.1f);

    auto bolt = logoArea.reduced (12, 7);
    juce::Path p;
    p.startNewSubPath ((float) bolt.getCentreX() + 2.0f, (float) bolt.getY());
    p.lineTo ((float) bolt.getX(), (float) bolt.getCentreY() + 3.0f);
    p.lineTo ((float) bolt.getCentreX() - 1.0f, (float) bolt.getCentreY() + 3.0f);
    p.lineTo ((float) bolt.getCentreX() - 3.0f, (float) bolt.getBottom());
    p.lineTo ((float) bolt.getRight(), (float) bolt.getCentreY() - 3.0f);
    p.lineTo ((float) bolt.getCentreX() + 3.0f, (float) bolt.getCentreY() - 3.0f);
    p.closeSubPath();
    g.fillPath (p);
}

void HeaderComponent::drawStatusBox (juce::Graphics& g)
{
    g.setColour (AppTheme::panel());
    g.fillRoundedRectangle (statusArea.toFloat(), 6.0f);
    g.setColour (AppTheme::border());
    g.drawRoundedRectangle (statusArea.toFloat(), 6.0f, 1.0f);

    auto r = statusArea.reduced (12, 0);
    g.setColour (AppTheme::green());
    g.fillEllipse (r.removeFromLeft (8).withSizeKeepingCentre (7, 7).toFloat());
    r.removeFromLeft (7);

    g.setFont (juce::Font (11.0f, juce::Font::bold));
    g.drawText ("Connected Cubase", r.removeFromLeft (120), juce::Justification::centredLeft);

    g.setColour (AppTheme::border());
    g.drawVerticalLine (r.getX(), (float) statusArea.getY() + 7.0f, (float) statusArea.getBottom() - 7.0f);
    r.removeFromLeft (12);

    g.setColour (AppTheme::subText());
    g.setFont (juce::Font (10.5f));
    g.drawText ("MIDI:", r.removeFromLeft (42), juce::Justification::centredLeft);
    g.setColour (AppTheme::purple());
    g.drawText ("loopMIDI", r, juce::Justification::centredLeft);
}

void HeaderComponent::drawActionButtons (juce::Graphics& g)
{
    auto r = actionArea;
    auto drawButton = [&] (juce::Rectangle<int> area, const juce::String& text)
    {
        g.setColour (AppTheme::panel());
        g.fillRoundedRectangle (area.toFloat(), 6.0f);
        g.setColour (AppTheme::border());
        g.drawRoundedRectangle (area.toFloat(), 6.0f, 1.0f);
        g.setColour (AppTheme::text());
        g.setFont (juce::Font (10.5f, juce::Font::bold));
        g.drawText (text, area, juce::Justification::centred);
    };

    drawButton (r.removeFromLeft (40), "VIEW"); r.removeFromLeft (7);
    drawButton (r.removeFromLeft (40), "REP");  r.removeFromLeft (7);
    drawButton (r.removeFromLeft (96), "VOL 100%");
}
