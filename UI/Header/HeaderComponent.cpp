#include "HeaderComponent.h"
#include "../Theme/AppTheme.h"

HeaderComponent::HeaderComponent() {}

void HeaderComponent::paint (juce::Graphics& g)
{
    g.fillAll (AppTheme::background());

    drawLogo (g);

    auto title = titleArea;
    g.setColour (AppTheme::text());
    g.setFont (juce::Font (15.5f, juce::Font::bold));
    g.drawText ("AUTO AUTOTUNE AI PRO", title.removeFromTop (22), juce::Justification::left);

    g.setColour (AppTheme::subText());
    g.setFont (juce::Font (9.5f));
    g.drawText ("ULTRA MINI", title, juce::Justification::left);

    drawStatusBox (g);
    drawActionButtons (g);
}

void HeaderComponent::resized()
{
    auto r = getLocalBounds();

    logoArea = r.removeFromLeft (54).reduced (9, 6);
    titleArea = r.removeFromLeft (250).reduced (2, 7);
    r.removeFromLeft (24);

    statusArea = r.removeFromLeft (410).reduced (0, 7);
    r.removeFromLeft (18);

    actionArea = r.reduced (0, 7);
}

void HeaderComponent::drawLogo (juce::Graphics& g)
{
    auto b = logoArea.toFloat();

    g.setColour (AppTheme::panel());
    g.fillRoundedRectangle (b, 8.0f);
    g.setColour (AppTheme::purple());
    g.drawRoundedRectangle (b, 8.0f, 1.2f);

    auto bolt = logoArea.reduced (12, 6);
    juce::Path p;
    p.startNewSubPath ((float) bolt.getCentreX() + 2.0f, (float) bolt.getY());
    p.lineTo ((float) bolt.getX(), (float) bolt.getCentreY() + 3.0f);
    p.lineTo ((float) bolt.getCentreX() - 1.0f, (float) bolt.getCentreY() + 3.0f);
    p.lineTo ((float) bolt.getCentreX() - 3.0f, (float) bolt.getBottom());
    p.lineTo ((float) bolt.getRight(), (float) bolt.getCentreY() - 3.0f);
    p.lineTo ((float) bolt.getCentreX() + 3.0f, (float) bolt.getCentreY() - 3.0f);
    p.closeSubPath();

    g.setColour (AppTheme::purple());
    g.fillPath (p);
}

void HeaderComponent::drawStatusBox (juce::Graphics& g)
{
    auto b = statusArea.toFloat();

    g.setColour (AppTheme::panel());
    g.fillRoundedRectangle (b, 7.0f);
    g.setColour (AppTheme::border());
    g.drawRoundedRectangle (b, 7.0f, 1.0f);

    auto r = statusArea.reduced (14, 0);

    g.setColour (AppTheme::green());
    g.fillEllipse (r.removeFromLeft (9).withSizeKeepingCentre (8, 8).toFloat());
    r.removeFromLeft (8);

    g.setFont (juce::Font (12.0f, juce::Font::bold));
    g.drawText ("Connected to Cubase", r.removeFromLeft (165), juce::Justification::centredLeft);

    g.setColour (AppTheme::border());
    g.drawVerticalLine (r.getX(), (float) statusArea.getY() + 8.0f, (float) statusArea.getBottom() - 8.0f);
    r.removeFromLeft (15);

    g.setColour (AppTheme::subText());
    g.setFont (juce::Font (11.5f));
    g.drawText ("MIDI Out:", r.removeFromLeft (70), juce::Justification::centredLeft);

    g.setColour (AppTheme::purple());
    g.drawText ("loopMIDI Port", r, juce::Justification::centredLeft);
}

void HeaderComponent::drawActionButtons (juce::Graphics& g)
{
    auto r = actionArea;

    auto drawButton = [&] (juce::Rectangle<int> area, const juce::String& text)
    {
        g.setColour (AppTheme::panel());
        g.fillRoundedRectangle (area.toFloat(), 7.0f);
        g.setColour (AppTheme::border());
        g.drawRoundedRectangle (area.toFloat(), 7.0f, 1.0f);
        g.setColour (AppTheme::text());
        g.setFont (juce::Font (11.5f, juce::Font::bold));
        g.drawText (text, area, juce::Justification::centred);
    };

    drawButton (r.removeFromLeft (50), "VIEW");
    r.removeFromLeft (8);
    drawButton (r.removeFromLeft (50), "REP");
    r.removeFromLeft (8);
    drawButton (r.removeFromLeft (110), "VOL 100%");
}
