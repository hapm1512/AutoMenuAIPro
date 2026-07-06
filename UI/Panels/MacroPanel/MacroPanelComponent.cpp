#include "MacroPanelComponent.h"
#include "../../Theme/AppTheme.h"

MacroPanelComponent::MacroPanelComponent()
{
    items = {
        { "AI",       "AI",  AppTheme::purple(), true  },
        { "Delay",    "DL",  AppTheme::green(),  false },
        { "Fix Meo",  "FX",  AppTheme::blue(),   false },
        { "Luu Bai",  "SV",  AppTheme::blue(),   false },
        { "Lofi",     "LO",  AppTheme::purple(), false },
        { "Tat Vang", "OFF", AppTheme::red(),    false },
        { "Vang",     "VG",  AppTheme::green(),  false },
        { "Tune",     "TU",  AppTheme::green(),  false },
        { "Nhac",     "NH",  AppTheme::green(),  false },
        { "Mic",      "MC",  AppTheme::green(),  false },
        { "Remix",    "RM",  AppTheme::blue(),   false },
        { "Dan Ca",   "DC",  AppTheme::blue(),   false },
    };
}

void MacroPanelComponent::paint (juce::Graphics& g)
{
    auto area = getLocalBounds();

    g.setColour (AppTheme::panel());
    g.fillRoundedRectangle (area.toFloat(), 10.0f);
    g.setColour (AppTheme::border());
    g.drawRoundedRectangle (area.toFloat(), 10.0f, 1.0f);

    auto titleArea = area.removeFromTop (38).reduced (18, 0);
    g.setColour (AppTheme::text());
    g.setFont (juce::Font (14.0f, juce::Font::bold));
    g.drawText ("PRESET / MACRO", titleArea, juce::Justification::centredLeft);

    auto grid = area.reduced (18, 8);
    constexpr int columns = 6;
    constexpr int rows = 2;
    constexpr int gap = 8;

    const int cardW = (grid.getWidth() - gap * (columns - 1)) / columns;
    const int cardH = juce::jmin (72, (grid.getHeight() - gap * (rows - 1)) / rows);
    const int totalH = rows * cardH + gap;
    const int startY = grid.getY() + juce::jmax (0, (grid.getHeight() - totalH) / 2);

    int index = 0;
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < columns; ++col)
        {
            if (index >= (int) items.size()) return;

            auto card = juce::Rectangle<int> (grid.getX() + col * (cardW + gap),
                                              startY + row * (cardH + gap),
                                              cardW,
                                              cardH);
            drawCard (g, card, items[(size_t) index]);
            ++index;
        }
    }
}

void MacroPanelComponent::resized() {}

void MacroPanelComponent::drawCard (juce::Graphics& g, juce::Rectangle<int> area, const MacroItem& item)
{
    auto b = area.toFloat();
    auto bg = AppTheme::panelDark().interpolatedWith (item.colour, item.active ? 0.28f : 0.13f);

    g.setColour (bg);
    g.fillRoundedRectangle (b, 7.0f);
    g.setColour (item.active ? item.colour : AppTheme::border());
    g.drawRoundedRectangle (b, 7.0f, item.active ? 1.4f : 1.0f);

    auto iconArea = area.removeFromTop (32);
    g.setColour (item.colour);
    g.setFont (juce::Font (15.5f, juce::Font::bold));
    g.drawText (item.icon, iconArea, juce::Justification::centred);

    g.setColour (AppTheme::text());
    g.setFont (juce::Font (11.0f, juce::Font::bold));
    g.drawText (item.title, area.removeFromTop (21), juce::Justification::centred);

    auto line = juce::Rectangle<float> ((float) area.getCentreX() - 14.0f,
                                        (float) area.getBottom() - 7.0f,
                                        28.0f,
                                        2.0f);
    g.setColour (item.colour);
    g.fillRoundedRectangle (line, 1.0f);
}
