#include "Dashboard.h"
#include "../Theme/AppTheme.h"

Dashboard::Dashboard()
{
    setSize(920, 560);
}

void Dashboard::paint(juce::Graphics& g)
{
    g.fillAll(AppTheme::background());

    drawPanel(g, headerArea, "HEADER");
    drawPanel(g, macroArea, "PRESET / MACRO");
    drawPanel(g, mixerArea, "MIX & EFFECT CONTROL");
    drawPanel(g, toneArea, "AI TONE DETECTOR");
    drawPanel(g, footerArea, "FOOTER");
    drawPanel(g, meterArea, "OUTPUT LEVEL");
}

void Dashboard::resized()
{
    auto r = getLocalBounds().reduced(8);

    headerArea = r.removeFromTop(44);

    r.removeFromTop(8);

    auto body = r.removeFromTop(390);
    auto rightColumn = body.removeFromRight(260);

    body.removeFromRight(8);

    macroArea = body.removeFromTop(205);
    body.removeFromTop(8);
    mixerArea = body;

    toneArea = rightColumn;

    r.removeFromTop(8);

    auto bottom = r;
    meterArea = bottom.removeFromRight(260);
    bottom.removeFromRight(8);
    footerArea = bottom;
}

void Dashboard::drawPanel(
    juce::Graphics& g,
    juce::Rectangle<int> area,
    const juce::String& title
)
{
    auto bounds = area.toFloat();

    g.setColour(AppTheme::panel());
    g.fillRoundedRectangle(bounds, 10.0f);

    g.setColour(AppTheme::border());
    g.drawRoundedRectangle(bounds, 10.0f, 1.0f);

    g.setColour(AppTheme::text());
    g.setFont(juce::Font(15.0f, juce::Font::bold));
    g.drawText(title, area.reduced(14, 10), juce::Justification::topLeft);
}