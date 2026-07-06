#include "ToneDetectorComponent.h"
#include "../../Theme/AppTheme.h"

ToneDetectorComponent::ToneDetectorComponent() {}

void ToneDetectorComponent::paint(juce::Graphics& g)
{
    drawPanel(g);

    auto area = getLocalBounds().reduced(18);

    g.setColour(AppTheme::text());
    g.setFont(juce::Font(15.0f, juce::Font::bold));
    g.drawText("AI TONE DETECTOR", area.removeFromTop(32), juce::Justification::centredLeft);

    area.removeFromTop(8);

    auto card = area.removeFromTop(250);

    g.setColour(AppTheme::panelDark());
    g.fillRoundedRectangle(card.toFloat(), 10.0f);

    g.setColour(AppTheme::border());
    g.drawRoundedRectangle(card.toFloat(), 10.0f, 1.0f);

    auto inner = card.reduced(16);

    g.setColour(AppTheme::purple());
    g.setFont(juce::Font(30.0f, juce::Font::bold));
    g.drawText("D Minor", inner.removeFromTop(44), juce::Justification::centred);

    g.setColour(AppTheme::subText());
    g.setFont(juce::Font(12.0f));
    g.drawText("Confidence", inner.removeFromTop(24), juce::Justification::centred);

    drawRing(g, inner.removeFromTop(118));

    auto info = inner.removeFromTop(42);

    g.setColour(AppTheme::text());
    g.setFont(juce::Font(13.0f, juce::Font::bold));
    g.drawText("BPM 128     4/4     Camelot 7A", info, juce::Justification::centred);

    area.removeFromTop(16);

    auto buttons = area.removeFromTop(54);
    auto analyze = buttons.removeFromLeft((buttons.getWidth() - 12) / 2);
    buttons.removeFromLeft(12);

    drawButton(g, analyze, "ANALYZE", AppTheme::purple());
    drawButton(g, buttons, "LOAD AUDIO", AppTheme::blue());
}

void ToneDetectorComponent::resized() {}

void ToneDetectorComponent::drawPanel(juce::Graphics& g)
{
    auto area = getLocalBounds();

    g.setColour(AppTheme::panel());
    g.fillRoundedRectangle(area.toFloat(), 10.0f);

    g.setColour(AppTheme::border());
    g.drawRoundedRectangle(area.toFloat(), 10.0f, 1.0f);
}

void ToneDetectorComponent::drawRing(juce::Graphics& g, juce::Rectangle<int> area)
{
    auto ring = area.withSizeKeepingCentre(104, 104).toFloat();

    g.setColour(AppTheme::border());
    g.drawEllipse(ring, 8.0f);

    juce::Path arc;
    arc.addCentredArc(
        ring.getCentreX(),
        ring.getCentreY(),
        ring.getWidth() * 0.5f,
        ring.getHeight() * 0.5f,
        0.0f,
        juce::degreesToRadians(135.0f),
        juce::degreesToRadians(405.0f),
        true
    );

    g.setColour(AppTheme::purple());
    g.strokePath(arc, juce::PathStrokeType(8.0f));

    g.setColour(AppTheme::text());
    g.setFont(juce::Font(26.0f, juce::Font::bold));
    g.drawText("96%", area, juce::Justification::centred);
}

void ToneDetectorComponent::drawButton(
    juce::Graphics& g,
    juce::Rectangle<int> area,
    const juce::String& text,
    juce::Colour colour
)
{
    g.setColour(AppTheme::panelDark().interpolatedWith(colour, 0.18f));
    g.fillRoundedRectangle(area.toFloat(), 8.0f);

    g.setColour(colour);
    g.drawRoundedRectangle(area.toFloat(), 8.0f, 1.2f);

    g.setColour(colour);
    g.setFont(juce::Font(13.0f, juce::Font::bold));
    g.drawText(text, area, juce::Justification::centred);
}