#include "MixerPanelComponent.h"
#include "../../Theme/AppTheme.h"

MixerPanelComponent::MixerPanelComponent()
{
    knobs = {
        { "Nhac", "70", AppTheme::green(), 0.70f },
        { "Mic", "82", AppTheme::green(), 0.82f },
        { "Vang Dai", "45", AppTheme::blue(), 0.45f },
        { "Vang Ngan", "38", AppTheme::blue(), 0.38f },
        { "Delay", "32", AppTheme::purple(), 0.32f },
        { "Tune", "64", AppTheme::purple(), 0.64f },
        { "Flex", "50", AppTheme::yellow(), 0.50f },
    };
}

void MixerPanelComponent::paint (juce::Graphics& g)
{
    auto area = getLocalBounds();
    g.setColour (AppTheme::panel());
    g.fillRoundedRectangle (area.toFloat(), 8.0f);
    g.setColour (AppTheme::border());
    g.drawRoundedRectangle (area.toFloat(), 8.0f, 1.0f);

    auto title = area.removeFromTop (30).reduced (16, 0);
    g.setColour (AppTheme::text());
    g.setFont (juce::Font (12.5f, juce::Font::bold));
    g.drawText ("MIX & EFFECT CONTROL", title, juce::Justification::centredLeft);

    auto grid = area.reduced (14, 8);
    const int count = (int) knobs.size();
    const int gap = 8;
    const int w = (grid.getWidth() - gap * (count - 1)) / count;

    for (int i = 0; i < count; ++i)
    {
        auto box = juce::Rectangle<int> (grid.getX() + i * (w + gap), grid.getY(), w, grid.getHeight());
        drawKnob (g, box, knobs[(size_t) i]);
    }
}

void MixerPanelComponent::resized() {}

void MixerPanelComponent::drawKnob (juce::Graphics& g, juce::Rectangle<int> area, const KnobItem& k)
{
    auto top = area.removeFromTop (70);
    auto knob = top.withSizeKeepingCentre (54, 54).toFloat();

    g.setColour (AppTheme::panelDark());
    g.fillEllipse (knob);
    g.setColour (AppTheme::border());
    g.drawEllipse (knob, 2.0f);

    juce::Path arc;
    arc.addCentredArc (knob.getCentreX(), knob.getCentreY(), 26.0f, 26.0f, 0.0f,
                       juce::degreesToRadians (135.0f),
                       juce::degreesToRadians (135.0f + 270.0f * k.pos), true);
    g.setColour (k.colour);
    g.strokePath (arc, juce::PathStrokeType (3.2f));

    const float angle = juce::degreesToRadians (135.0f + 270.0f * k.pos);
    const float px = knob.getCentreX() + std::cos (angle) * 17.0f;
    const float py = knob.getCentreY() + std::sin (angle) * 17.0f;
    g.fillEllipse (px - 2.0f, py - 2.0f, 4.0f, 4.0f);

    g.setColour (AppTheme::text());
    g.setFont (juce::Font (11.0f, juce::Font::bold));
    g.drawText (k.value, knob.toNearestInt(), juce::Justification::centred);

    g.setColour (AppTheme::subText());
    g.setFont (juce::Font (10.0f));
    g.drawText (k.label, area.removeFromTop (18), juce::Justification::centred);
}
