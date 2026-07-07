#include "ToneDetectorComponent.h"
#include "../../Theme/AppTheme.h"

ToneDetectorComponent::ToneDetectorComponent() {}

void ToneDetectorComponent::paint (juce::Graphics& g)
{
    auto area = getLocalBounds();

    g.setColour (AppTheme::panel());
    g.fillRoundedRectangle (area.toFloat(), 10.0f);

    g.setColour (AppTheme::border());
    g.drawRoundedRectangle (area.toFloat(), 10.0f, 1.0f);

    auto title = area.removeFromTop (34).reduced (16, 0);

    g.setColour (AppTheme::text());
    g.setFont (juce::Font (13.0f, juce::Font::bold));
    g.drawText ("AI TONE DETECTOR", title, juce::Justification::centredLeft);

    auto inner = area.reduced (18, 8);
    auto card = inner.removeFromTop (318);

    g.setColour (AppTheme::panelDark());
    g.fillRoundedRectangle (card.toFloat(), 8.0f);

    g.setColour (AppTheme::border());
    g.drawRoundedRectangle (card.toFloat(), 8.0f, 1.0f);

    auto top = card.reduced (14, 8);

    g.setColour (AppTheme::purple());
    g.setFont (juce::Font (23.0f, juce::Font::bold));
    g.drawText ("D Minor", top.removeFromTop (38), juce::Justification::centred);

    g.setColour (AppTheme::subText());
    g.setFont (juce::Font (11.0f));
    g.drawText ("Confidence", top.removeFromTop (18), juce::Justification::centred);

    auto ringArea = top.removeFromTop (70).withSizeKeepingCentre (72, 64).toFloat();

    g.setColour (AppTheme::border());
    g.drawEllipse (ringArea.withSizeKeepingCentre (60.0f, 60.0f), 4.2f);

    juce::Path arc;
    arc.addCentredArc (ringArea.getCentreX(), ringArea.getCentreY(), 29.0f, 29.0f,
                       0.0f, 2.4f, 6.15f, true);

    g.setColour (AppTheme::purple());
    g.strokePath (arc, juce::PathStrokeType (4.2f));

    g.setColour (AppTheme::text());
    g.setFont (juce::Font (20.0f, juce::Font::bold));
    g.drawText ("96%", ringArea.toNearestInt(), juce::Justification::centred);

    auto scale = top.removeFromTop (28).reduced (16, 2);
    auto major = scale.removeFromLeft (scale.getWidth() / 2 - 4);
    scale.removeFromLeft (8);
    auto minor = scale;

    g.setColour (AppTheme::panel());
    g.fillRoundedRectangle (major.toFloat(), 5.0f);
    g.setColour (AppTheme::border());
    g.drawRoundedRectangle (major.toFloat(), 5.0f, 1.0f);

    g.setColour (AppTheme::text());
    g.setFont (juce::Font (10.5f, juce::Font::bold));
    g.drawText ("Major", major, juce::Justification::centred);

    g.setColour (AppTheme::purple());
    g.fillRoundedRectangle (minor.toFloat(), 5.0f);
    g.setColour (AppTheme::text());
    g.drawText ("Minor", minor, juce::Justification::centred);

    top.removeFromTop (8);

    auto toneBar = top.removeFromTop (36).reduced (12, 0);
    auto leftBtn = toneBar.removeFromLeft (40);
    auto rightBtn = toneBar.removeFromRight (40);
    auto center = toneBar.reduced (8, 0);

    g.setColour (AppTheme::panel());
    g.fillRoundedRectangle (leftBtn.toFloat(), 6.0f);
    g.setColour (AppTheme::purple());
    g.drawRoundedRectangle (leftBtn.toFloat(), 6.0f, 1.1f);
    g.setFont (juce::Font (18.0f, juce::Font::bold));
    g.drawText ("<", leftBtn, juce::Justification::centred);

    g.setColour (AppTheme::panel());
    g.fillRoundedRectangle (rightBtn.toFloat(), 6.0f);
    g.setColour (AppTheme::purple());
    g.drawRoundedRectangle (rightBtn.toFloat(), 6.0f, 1.1f);
    g.drawText (">", rightBtn, juce::Justification::centred);

    g.setColour (AppTheme::subText());
    g.setFont (juce::Font (9.5f));
    g.drawText ("TONE", center.removeFromTop (12), juce::Justification::centred);

    g.setColour (AppTheme::text());
    g.setFont (juce::Font (13.5f, juce::Font::bold));
    g.drawText ("D Minor", center, juce::Justification::centred);

    top.removeFromTop (8);

    auto buttons = top.removeFromTop (36);
    auto analyze = buttons.removeFromLeft (buttons.getWidth() / 2 - 5);
    buttons.removeFromLeft (10);
    auto load = buttons;

    drawButton (g, analyze, "ANALYZE", AppTheme::purple());
    drawButton (g, load, "LOAD", AppTheme::blue());

    top.removeFromTop (8);

    drawLedMeter (g, top.removeFromTop (48));
}

void ToneDetectorComponent::drawButton (juce::Graphics& g, juce::Rectangle<int> area,
                                        const juce::String& text, juce::Colour colour)
{
    g.setColour (AppTheme::panelDark().interpolatedWith (colour, 0.18f));
    g.fillRoundedRectangle (area.toFloat(), 6.0f);

    g.setColour (colour);
    g.drawRoundedRectangle (area.toFloat(), 6.0f, 1.0f);

    g.setFont (juce::Font (10.5f, juce::Font::bold));
    g.drawText (text, area, juce::Justification::centred);
}

void ToneDetectorComponent::drawLedMeter (juce::Graphics& g, juce::Rectangle<int> area)
{
    auto drawRow = [&] (juce::Rectangle<int> row, const juce::String& label, float level)
    {
        auto lab = row.removeFromLeft (16);
        g.setColour (AppTheme::subText());
        g.setFont (juce::Font (10.0f, juce::Font::bold));
        g.drawText (label, lab, juce::Justification::centredLeft);

        const int segments = 18;
        const int gap = 2;
        const int segW = (row.getWidth() - gap * (segments - 1)) / segments;
        const int active = (int) std::round (level * (float) segments);

        for (int i = 0; i < segments; ++i)
        {
            auto seg = juce::Rectangle<int> (row.getX() + i * (segW + gap), row.getY() + 3,
                                             segW, row.getHeight() - 6);

            if (i < active)
            {
                if (i > 14)      g.setColour (AppTheme::red());
                else if (i > 11) g.setColour (juce::Colour::fromRGB (235, 190, 45));
                else             g.setColour (AppTheme::green());
            }
            else
            {
                g.setColour (AppTheme::border().withAlpha (0.55f));
            }

            g.fillRoundedRectangle (seg.toFloat(), 1.6f);
        }
    };

    g.setColour (AppTheme::panel());
    g.fillRoundedRectangle (area.toFloat(), 7.0f);
    g.setColour (AppTheme::border());
    g.drawRoundedRectangle (area.toFloat(), 7.0f, 1.0f);

    auto inner = area.reduced (8, 4);
    drawRow (inner.removeFromTop (18), "L", 0.72f);
    drawRow (inner.removeFromTop (18), "R", 0.61f);
}

void ToneDetectorComponent::resized() {}
