#include "MacroPanelComponent.h"
#include "../../Theme/AppTheme.h"

MacroPanelComponent::MacroPanelComponent()
{
    items = {
        { "AI",       IconType::ai,     AppTheme::purple(), true  },
        { "Delay",    IconType::delay,  AppTheme::green(),  false },
        { "Fix Meo",  IconType::fix,    AppTheme::blue(),   false },
        { "Luu Bai",  IconType::save,   AppTheme::blue(),   false },
        { "Lofi",     IconType::lofi,   AppTheme::purple(), false },
        { "Tat Vang", IconType::off,    AppTheme::red(),    false },
        { "Vang",     IconType::reverb, AppTheme::green(),  false },
        { "Tune",     IconType::tune,   AppTheme::green(),  false },
        { "Nhac",     IconType::music,  AppTheme::green(),  false },
        { "Mic",      IconType::mic,    AppTheme::green(),  false },
        { "Remix",    IconType::remix,  AppTheme::blue(),   false },
        { "Dan Ca",   IconType::danca,  AppTheme::blue(),   false },
    };
}

void MacroPanelComponent::paint (juce::Graphics& g)
{
    auto area = getLocalBounds();
    g.setColour (AppTheme::panel());
    g.fillRoundedRectangle (area.toFloat(), 8.0f);
    g.setColour (AppTheme::border());
    g.drawRoundedRectangle (area.toFloat(), 8.0f, 1.0f);

    auto titleArea = area.removeFromTop (30).reduced (16, 0);
    drawSectionIcon (g, titleArea.removeFromLeft (16));
    titleArea.removeFromLeft (7);
    g.setColour (AppTheme::text());
    g.setFont (juce::Font (12.5f, juce::Font::bold));
    g.drawText ("PRESET / MACRO", titleArea, juce::Justification::centredLeft);

    auto grid = area.reduced (14, 7);
    constexpr int columns = 6;
    constexpr int rows = 2;
    constexpr int gapX = 7;
    constexpr int gapY = 7;
    const int cardW = (grid.getWidth() - gapX * (columns - 1)) / columns;
    const int cardH = (grid.getHeight() - gapY * (rows - 1)) / rows;

    int index = 0;
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < columns; ++col)
        {
            if (index >= (int) items.size()) return;
            auto card = juce::Rectangle<int> (grid.getX() + col * (cardW + gapX),
                                              grid.getY() + row * (cardH + gapY),
                                              cardW, cardH);
            drawCard (g, card, items[(size_t) index], index);
            ++index;
        }
    }
}

void MacroPanelComponent::resized() {}

void MacroPanelComponent::mouseUp (const juce::MouseEvent& event)
{
    const auto position = event.getPosition();

    for (int i = 0; i < (int) macroBounds.size(); ++i)
    {
        if (macroBounds[(size_t) i].contains (position))
        {
            setActiveMacro (i);

            if (onMacroPressed)
                onMacroPressed (i);

            return;
        }
    }
}

void MacroPanelComponent::setActiveMacro (int index)
{
    for (int i = 0; i < (int) items.size(); ++i)
        items[(size_t) i].active = (i == index);

    repaint();
}

void MacroPanelComponent::drawCard (juce::Graphics& g, juce::Rectangle<int> area, const MacroItem& item, int index)
{
    if (index >= 0 && index < (int) macroBounds.size())
        macroBounds[(size_t) index] = area;

    auto bg = AppTheme::panelDark().interpolatedWith (item.colour, item.active ? 0.24f : 0.11f);
    g.setColour (bg);
    g.fillRoundedRectangle (area.toFloat(), 6.0f);
    g.setColour (item.active ? item.colour : AppTheme::border());
    g.drawRoundedRectangle (area.toFloat(), 6.0f, item.active ? 1.35f : 1.0f);

    auto iconArea = area.removeFromTop ((int) (area.getHeight() * 0.58f)).reduced (0, 4);
    drawIcon (g, iconArea.withSizeKeepingCentre (28, 28), item.icon, item.colour);

    g.setColour (AppTheme::text());
    g.setFont (juce::Font (10.2f, juce::Font::bold));
    g.drawText (item.title, area.removeFromTop (16), juce::Justification::centred);

    auto line = juce::Rectangle<float> ((float) area.getCentreX() - 12.0f,
                                        (float) area.getBottom() - 5.0f,
                                        24.0f, 1.7f);
    g.setColour (item.colour);
    g.fillRoundedRectangle (line, 1.0f);
}

void MacroPanelComponent::drawIcon (juce::Graphics& g, juce::Rectangle<int> area, IconType icon, juce::Colour colour)
{
    auto r = area.toFloat();
    g.setColour (colour.withAlpha (0.15f));
    g.fillEllipse (r.expanded (3.0f));
    g.setColour (colour);

    juce::Path p;
    juce::PathStrokeType stroke (2.1f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);
    const float x = r.getX(), y = r.getY(), w = r.getWidth(), h = r.getHeight();
    const float cx = r.getCentreX(), cy = r.getCentreY();

    switch (icon)
    {
        case IconType::ai:
        {
            auto face = r.reduced (5.0f);
            g.drawRoundedRectangle (face, 4.0f, 2.0f);
            g.fillEllipse (face.getX() + 7.0f, face.getY() + 8.0f, 3.0f, 3.0f);
            g.fillEllipse (face.getRight() - 10.0f, face.getY() + 8.0f, 3.0f, 3.0f);
            g.drawLine (face.getX() + 7.0f, face.getBottom() - 7.0f, face.getRight() - 7.0f, face.getBottom() - 7.0f, 1.8f);
            g.drawLine (cx, y, cx, y + 5.0f, 1.8f);
            g.fillEllipse (cx - 1.8f, y, 3.6f, 3.6f);
            break;
        }
        case IconType::delay:
            g.drawEllipse (r.reduced (5.0f), 2.1f); g.drawLine (cx, cy, cx, y + 8.0f, 2.0f); g.drawLine (cx, cy, cx + 7.0f, cy, 2.0f); break;
        case IconType::fix:
            p.startNewSubPath (x + 2, cy); p.lineTo (x + 8, cy); p.lineTo (x + 11, y + 7); p.lineTo (x + 15, y + h - 7); p.lineTo (x + 19, y + 8); p.lineTo (x + 23, cy); p.lineTo (x + w - 2, cy); g.strokePath (p, stroke); break;
        case IconType::save:
            g.drawRoundedRectangle (r.reduced (5.0f), 2.0f, 2.0f); g.drawRect (juce::Rectangle<float> (x + 10, y + 6, 10, 7), 1.8f); g.drawLine (x + 9, y + h - 7, x + w - 9, y + h - 7, 1.8f); break;
        case IconType::lofi:
        case IconType::music:
            p.startNewSubPath (cx - 4, y + 7); p.lineTo (cx - 4, y + h - 7); p.lineTo (cx + 10, y + h - 11); p.lineTo (cx + 10, y + 11); g.strokePath (p, stroke); g.fillEllipse (cx - 12, y + h - 10, 8, 6); g.fillEllipse (cx + 2, y + h - 14, 8, 6); break;
        case IconType::off:
            g.drawEllipse (r.reduced (5.0f), 2.1f); g.drawLine (x + 8, y + 8, x + w - 8, y + h - 8, 2.1f); break;
        case IconType::reverb:
            for (int i = 0; i < 3; ++i) { juce::Path arc; float rr = 6.0f + i * 4.4f; arc.addCentredArc (cx - 7, cy, rr, rr, 0.0f, -1.1f, 1.1f, true); g.strokePath (arc, juce::PathStrokeType (1.9f)); } g.fillEllipse (cx + 5, cy - 3, 6, 6); break;
        case IconType::tune:
            g.drawLine (cx, y + 5, cx, y + h - 7, 2.0f); g.drawEllipse (juce::Rectangle<float> (cx - 6, y + 5, 12, 19), 2.0f); g.drawLine (cx - 6, y + h - 7, cx + 6, y + h - 7, 2.0f); break;
        case IconType::mic:
        {
            g.drawRoundedRectangle (juce::Rectangle<float> (cx - 4.5f, y + 4, 9, 17), 5.0f, 2.0f);
            juce::Path micArc; micArc.addCentredArc (cx, y + 21.5f, 10.5f, 7.5f, 0.0f, 0.15f, 3.0f, true); g.strokePath (micArc, juce::PathStrokeType (2.0f));
            g.drawLine (cx, y + h - 8, cx, y + h - 4, 1.8f); g.drawLine (cx - 6, y + h - 4, cx + 6, y + h - 4, 1.8f); break;
        }
        case IconType::remix:
        {
            juce::Path a; a.addArc (x + 5, y + 7, w - 10, h - 10, 0.2f, 4.8f, true); g.strokePath (a, stroke);
            p.startNewSubPath (x + w - 6, y + 11); p.lineTo (x + w - 11, y + 8); p.lineTo (x + w - 10, y + 15); g.fillPath (p); break;
        }
        case IconType::danca:
            g.drawLine (x + 10, y + 7, x + w - 7, y + h - 10, 2.0f); g.drawEllipse (juce::Rectangle<float> (x + 5, y + h - 13, 11, 8), 2.0f); g.drawEllipse (juce::Rectangle<float> (x + w - 14, y + 6, 8, 8), 1.8f); break;
    }
}

void MacroPanelComponent::drawSectionIcon (juce::Graphics& g, juce::Rectangle<int> area)
{
    auto r = area.toFloat().reduced (2.0f);
    g.setColour (AppTheme::subText());
    g.drawRoundedRectangle (r, 2.0f, 1.1f);
    g.drawLine (r.getX(), r.getCentreY(), r.getCentreX(), r.getBottom(), 1.1f);
    g.drawLine (r.getRight(), r.getCentreY(), r.getCentreX(), r.getBottom(), 1.1f);
    g.drawLine (r.getCentreX(), r.getY(), r.getCentreX(), r.getBottom(), 1.1f);
}
