#include "FooterComponent.h"
#include "../Theme/AppTheme.h"

FooterComponent::FooterComponent() {}

void FooterComponent::paint (juce::Graphics& g)
{
    auto area = getLocalBounds();
    g.setColour (AppTheme::panel());
    g.fillRoundedRectangle (area.toFloat(), 10.0f);
    g.setColour (AppTheme::border());
    g.drawRoundedRectangle (area.toFloat(), 10.0f, 1.0f);

    auto title = area.removeFromTop (34).reduced (18, 0);
    g.setColour (AppTheme::text());
    g.setFont (juce::Font (14.0f, juce::Font::bold));
    g.drawText ("FOOTER", title, juce::Justification::centredLeft);

    auto row = area.reduced (18, 10);
    juce::StringArray labels { "SETTINGS", "TEMPLATE", "SHORTCUT", "120.0 BPM", "4/4 TIME", "C Maj KEY", "CPU 12%" };
    int widths[] = { 120, 120, 120, 95, 95, 95, 115 };

    for (int i = 0; i < labels.size(); ++i)
    {
        auto b = row.removeFromLeft (widths[i]);
        row.removeFromLeft (12);
        g.setColour (i < 3 ? AppTheme::panelDark() : AppTheme::panel());
        g.fillRoundedRectangle (b.toFloat(), 6.0f);
        g.setColour (i < 3 ? AppTheme::border() : juce::Colours::transparentBlack);
        g.drawRoundedRectangle (b.toFloat(), 6.0f, 1.0f);
        g.setColour (AppTheme::text());
        g.setFont (juce::Font (10.5f, juce::Font::bold));
        g.drawText (labels[i], b, juce::Justification::centred);
    }
}

void FooterComponent::resized() {}
