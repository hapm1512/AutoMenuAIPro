#include "OutputMeterComponent.h"
#include "../../Theme/AppTheme.h"

OutputMeterComponent::OutputMeterComponent() {}

void OutputMeterComponent::paint (juce::Graphics& g)
{
    auto area = getLocalBounds();
    g.setColour (AppTheme::panel());
    g.fillRoundedRectangle (area.toFloat(), 10.0f);
    g.setColour (AppTheme::border());
    g.drawRoundedRectangle (area.toFloat(), 10.0f, 1.0f);

    auto title = area.removeFromTop (34).reduced (18, 0);
    g.setColour (AppTheme::text());
    g.setFont (juce::Font (14.0f, juce::Font::bold));
    g.drawText ("OUTPUT LEVEL", title, juce::Justification::centredLeft);

    auto meter = area.reduced (26, 12);
    const int bars = 14;
    for (int ch = 0; ch < 2; ++ch)
    {
        auto col = meter.removeFromLeft (45);
        for (int i = 0; i < bars; ++i)
        {
            auto seg = juce::Rectangle<int> (col.getX(), col.getBottom() - (i + 1) * 7, 26, 5);
            g.setColour (i > 11 ? AppTheme::red() : i > 8 ? AppTheme::yellow() : AppTheme::green());
            g.fillRoundedRectangle (seg.toFloat(), 1.5f);
        }
        g.setColour (AppTheme::text());
        g.setFont (juce::Font (10.0f));
        g.drawText (ch == 0 ? "L" : "R", col.removeFromBottom (14), juce::Justification::centredLeft);
    }

    g.setColour (AppTheme::text());
    g.setFont (juce::Font (17.0f, juce::Font::bold));
    g.drawText ("0.0 dB", meter, juce::Justification::centredRight);
}

void OutputMeterComponent::resized() {}
