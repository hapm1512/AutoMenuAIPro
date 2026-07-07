#include "OutputMeterComponent.h"
#include "../../Theme/AppTheme.h"

OutputMeterComponent::OutputMeterComponent() {}

void OutputMeterComponent::paint (juce::Graphics& g)
{
    auto area = getLocalBounds();
    g.setColour (AppTheme::panel());
    g.fillRoundedRectangle (area.toFloat(), 8.0f);
    g.setColour (AppTheme::border());
    g.drawRoundedRectangle (area.toFloat(), 8.0f, 1.0f);

    auto r = area.reduced (14, 8);
    g.setColour (AppTheme::text());
    g.setFont (juce::Font (11.5f, juce::Font::bold));
    g.drawText ("OUTPUT LEVEL", r.removeFromTop (18), juce::Justification::centredLeft);
    r.removeFromTop (4);
    drawLedRow (g, r.removeFromTop (18), "L", 0.72f);
    r.removeFromTop (5);
    drawLedRow (g, r.removeFromTop (18), "R", 0.64f);
}

void OutputMeterComponent::resized() {}

void OutputMeterComponent::drawLedRow (juce::Graphics& g, juce::Rectangle<int> area, const juce::String& label, float level)
{
    auto lab = area.removeFromLeft (18);
    g.setColour (AppTheme::subText());
    g.setFont (juce::Font (10.5f, juce::Font::bold));
    g.drawText (label, lab, juce::Justification::centredLeft);

    const int count = 24;
    const int gap = 2;
    const int ledW = (area.getWidth() - gap * (count - 1)) / count;
    const int active = (int) std::round (level * (float) count);

    for (int i = 0; i < count; ++i)
    {
        auto led = juce::Rectangle<int> (area.getX() + i * (ledW + gap), area.getY() + 4, ledW, area.getHeight() - 8);
        juce::Colour c = AppTheme::green();
        if (i > 17) c = AppTheme::yellow();
        if (i > 21) c = AppTheme::red();
        g.setColour (i < active ? c : AppTheme::panelDark());
        g.fillRoundedRectangle (led.toFloat(), 2.0f);
    }
}
