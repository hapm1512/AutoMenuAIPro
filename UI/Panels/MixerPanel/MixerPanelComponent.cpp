#include "MixerPanelComponent.h"
#include "../../Theme/AppTheme.h"

MixerPanelComponent::MixerPanelComponent()
{
    knobs = {
        { "Nhac", 70, AppTheme::green()  },
        { "Mic",  65, AppTheme::green()  },
        { "Vang Dai", 55, AppTheme::purple() },
        { "Vang Ngan", 45, AppTheme::yellow() },
        { "Delay", 35, AppTheme::blue()   },
        { "Tune", 60, AppTheme::purple() },
        { "Flex", 40, AppTheme::blue()   }
    };
}

void MixerPanelComponent::paint (juce::Graphics& g)
{
    auto area = getLocalBounds();
    g.setColour (AppTheme::panel());
    g.fillRoundedRectangle (area.toFloat(), 10.0f);
    g.setColour (AppTheme::border());
    g.drawRoundedRectangle (area.toFloat(), 10.0f, 1.0f);

    auto title = area.removeFromTop (38).reduced (18, 0);
    g.setColour (AppTheme::text());
    g.setFont (juce::Font (14.0f, juce::Font::bold));
    g.drawText ("MIX & EFFECT CONTROL", title, juce::Justification::centredLeft);

    auto grid = area.reduced (18, 10);
    const int count = (int) knobs.size();
    const int cellW = grid.getWidth() / count;

    for (int i = 0; i < count; ++i)
    {
        auto cell = juce::Rectangle<int> (grid.getX() + i * cellW, grid.getY(), cellW, grid.getHeight()).reduced (6, 0);
        drawKnob (g, cell, knobs[(size_t) i]);

        if (i > 0)
        {
            g.setColour (AppTheme::border().withAlpha (0.65f));
            g.drawVerticalLine (cell.getX() - 6, (float) cell.getY() + 18.0f, (float) cell.getBottom() - 12.0f);
        }
    }
}

void MixerPanelComponent::resized() {}

void MixerPanelComponent::drawKnob (juce::Graphics& g, juce::Rectangle<int> area, const KnobItem& item)
{
    auto label = area.removeFromTop (24);
    g.setColour (item.colour);
    g.setFont (juce::Font (11.5f, juce::Font::bold));
    g.drawText (item.name, label, juce::Justification::centred);

    const int knobSize = juce::jmin (62, juce::jmin (area.getWidth(), area.getHeight() - 26));
    auto k = juce::Rectangle<int> (0, 0, knobSize, knobSize).withCentre (area.getCentre()).translated (0, -8);
    auto bounds = k.toFloat();

    g.setColour (juce::Colours::black.withAlpha (0.35f));
    g.fillEllipse (bounds.expanded (4.0f));

    g.setColour (juce::Colour::fromRGB (20, 26, 34));
    g.fillEllipse (bounds);
    g.setColour (AppTheme::border());
    g.drawEllipse (bounds, 2.0f);

    const float start = juce::MathConstants<float>::pi * 0.78f;
    const float end   = juce::MathConstants<float>::pi * 2.22f;
    const float pos   = start + (end - start) * ((float) item.value / 100.0f);

    juce::Path arc;
    arc.addCentredArc (bounds.getCentreX(), bounds.getCentreY(), knobSize * 0.44f, knobSize * 0.44f, 0.0f, start, pos, true);
    g.setColour (item.colour);
    g.strokePath (arc, juce::PathStrokeType (4.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    juce::Point<float> p1 (bounds.getCentreX(), bounds.getCentreY());
    juce::Point<float> p2 (bounds.getCentreX() + std::cos (pos - juce::MathConstants<float>::halfPi) * knobSize * 0.28f,
                           bounds.getCentreY() + std::sin (pos - juce::MathConstants<float>::halfPi) * knobSize * 0.28f);
    g.setColour (AppTheme::text());
    g.drawLine ({ p1, p2 }, 2.0f);

    g.setColour (AppTheme::text());
    g.setFont (juce::Font (12.0f));
    g.drawText (juce::String (item.value) + "%", area.removeFromBottom (22), juce::Justification::centred);
}
