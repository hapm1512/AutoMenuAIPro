#include "SettingsPanel.h"
#include "../Theme/AppTheme.h"

SettingsPanel::SettingsPanel()
{
    setInterceptsMouseClicks (true, true);
}

void SettingsPanel::paint (juce::Graphics& g)
{
    auto area = getLocalBounds();

    g.setColour (juce::Colour::fromRGB (12, 14, 18).withAlpha (0.98f));
    g.fillRoundedRectangle (area.toFloat(), 10.0f);

    g.setColour (AppTheme::border());
    g.drawRoundedRectangle (area.toFloat().reduced (0.5f), 10.0f, 1.0f);

    auto content = area.reduced (12);

    drawHeader (g, content.removeFromTop (52));
    content.removeFromTop (8);

    auto audio = content.removeFromTop (132);
    drawGroup (g, audio,
               "AUDIO",
               {
                   { "Audio Device", "Default Device" },
                   { "Sample Rate",  "48.0 kHz" },
                   { "Buffer Size",  "256 samples" }
               },
               AppTheme::blue());

    content.removeFromTop (8);

    auto midi = content.removeFromTop (104);
    drawGroup (g, midi,
               "MIDI",
               {
                   { "MIDI In",  "loopMIDI" },
                   { "MIDI Out", "loopMIDI" }
               },
               AppTheme::purple());

    content.removeFromTop (8);

    auto cubase = content.removeFromTop (138);
    g.setColour (AppTheme::panel());
    g.fillRoundedRectangle (cubase.toFloat(), 8.0f);
    g.setColour (AppTheme::green());
    g.drawRoundedRectangle (cubase.toFloat(), 8.0f, 1.0f);

    auto c = cubase.reduced (12, 8);
    g.setColour (AppTheme::green());
    g.setFont (juce::Font (12.5f, juce::Font::bold));
    g.drawText ("CUBASE", c.removeFromTop (22), juce::Justification::centredLeft);
    drawRow (g, c.removeFromTop (32), "Cubase Path", "Cubase15.exe", true);
    drawCheckRow (g, c.removeFromTop (28), "Auto launch Cubase", true);
    drawCheckRow (g, c.removeFromTop (28), "Open last project", false);
    drawCheckRow (g, c.removeFromTop (28), "Auto connect MIDI", true);

    content.removeFromTop (8);

    auto general = content.removeFromTop (132);
    drawGroup (g, general,
               "GENERAL",
               {
                   { "GUI Scale", "85%" },
                   { "Theme",     "Dark" },
                   { "Language",  "English" }
               },
               AppTheme::green());

    auto bottom = area.reduced (12).removeFromBottom (38);
    saveButtonArea = bottom.removeFromLeft ((bottom.getWidth() - 14) / 3);
    bottom.removeFromLeft (7);
    resetButtonArea = bottom.removeFromLeft (bottom.getWidth() / 2);
    bottom.removeFromLeft (7);
    cancelButtonArea = bottom;

    drawBottomButton (g, saveButtonArea, "SAVE", AppTheme::green());
    drawBottomButton (g, resetButtonArea, "RESET", AppTheme::purple());
    drawBottomButton (g, cancelButtonArea, "CLOSE", AppTheme::red());
}

void SettingsPanel::resized()
{
}

void SettingsPanel::mouseUp (const juce::MouseEvent& event)
{
    const auto p = event.getPosition();

    if (closeButtonArea.contains (p) || cancelButtonArea.contains (p))
    {
        if (onClose)
            onClose();
        return;
    }

    if (saveButtonArea.contains (p))
    {
        if (onSave)
            onSave();
        return;
    }
}

void SettingsPanel::drawHeader (juce::Graphics& g, juce::Rectangle<int> area)
{
    auto icon = area.removeFromLeft (42).reduced (5);

    g.setColour (juce::Colours::white.withAlpha (0.08f));
    g.fillEllipse (icon.toFloat());
    g.setColour (AppTheme::purple());
    g.drawEllipse (icon.toFloat(), 1.5f);

    g.setColour (AppTheme::text());
    g.setFont (juce::Font (19.0f, juce::Font::bold));
    g.drawText ("CAU HINH HE THONG", area.removeFromLeft (220), juce::Justification::centredLeft);

    closeButtonArea = area.removeFromRight (32).reduced (3);

    g.setColour (AppTheme::panel());
    g.fillRoundedRectangle (closeButtonArea.toFloat(), 6.0f);

    g.setColour (AppTheme::border());
    g.drawRoundedRectangle (closeButtonArea.toFloat(), 6.0f, 1.0f);

    g.setColour (AppTheme::subText());
    g.setFont (juce::Font (17.0f, juce::Font::bold));
    g.drawText ("X", closeButtonArea, juce::Justification::centred);
}

void SettingsPanel::drawGroup (juce::Graphics& g,
                               juce::Rectangle<int> area,
                               const juce::String& title,
                               const std::vector<std::pair<juce::String, juce::String>>& rows,
                               juce::Colour accent)
{
    g.setColour (AppTheme::panel());
    g.fillRoundedRectangle (area.toFloat(), 8.0f);

    g.setColour (accent);
    g.drawRoundedRectangle (area.toFloat(), 8.0f, 1.0f);

    auto r = area.reduced (12, 8);

    g.setColour (accent);
    g.setFont (juce::Font (12.5f, juce::Font::bold));
    g.drawText (title, r.removeFromTop (22), juce::Justification::centredLeft);

    for (const auto& row : rows)
        drawRow (g, r.removeFromTop (32), row.first, row.second, false);
}

void SettingsPanel::drawRow (juce::Graphics& g,
                             juce::Rectangle<int> area,
                             const juce::String& label,
                             const juce::String& value,
                             bool hasButton)
{
    auto labelArea = area.removeFromLeft (104);
    auto valueArea = area.reduced (0, 3);
    juce::Rectangle<int> buttonArea;

    if (hasButton)
    {
        buttonArea = valueArea.removeFromRight (56);
        valueArea.removeFromRight (8);
    }

    g.setColour (AppTheme::subText());
    g.setFont (juce::Font (11.2f));
    g.drawText (label, labelArea, juce::Justification::centredLeft);

    g.setColour (AppTheme::panelDark());
    g.fillRoundedRectangle (valueArea.toFloat(), 5.0f);
    g.setColour (AppTheme::border());
    g.drawRoundedRectangle (valueArea.toFloat(), 5.0f, 1.0f);
    g.setColour (AppTheme::text());
    g.setFont (juce::Font (11.2f, juce::Font::bold));
    g.drawText (value, valueArea.reduced (8, 0), juce::Justification::centredLeft);

    if (hasButton)
    {
        g.setColour (AppTheme::purple().withAlpha (0.65f));
        g.fillRoundedRectangle (buttonArea.toFloat(), 5.0f);
        g.setColour (AppTheme::purple());
        g.drawRoundedRectangle (buttonArea.toFloat(), 5.0f, 1.0f);
        g.setColour (AppTheme::text());
        g.setFont (juce::Font (10.0f, juce::Font::bold));
        g.drawText ("Browse", buttonArea, juce::Justification::centred);
    }
}

void SettingsPanel::drawCheckRow (juce::Graphics& g,
                                  juce::Rectangle<int> area,
                                  const juce::String& label,
                                  bool checked)
{
    auto box = area.removeFromLeft (20).withSizeKeepingCentre (13, 13);

    g.setColour (AppTheme::panelDark());
    g.fillRect (box);
    g.setColour (checked ? AppTheme::green() : AppTheme::border());
    g.drawRect (box, 1);

    if (checked)
    {
        juce::Path tick;
        tick.startNewSubPath ((float) box.getX() + 3.0f, (float) box.getCentreY());
        tick.lineTo ((float) box.getCentreX(), (float) box.getBottom() - 3.0f);
        tick.lineTo ((float) box.getRight() - 3.0f, (float) box.getY() + 3.0f);
        g.strokePath (tick, juce::PathStrokeType (1.8f));
    }

    g.setColour (AppTheme::text());
    g.setFont (juce::Font (11.0f));
    g.drawText (label, area, juce::Justification::centredLeft);
}

void SettingsPanel::drawBottomButton (juce::Graphics& g,
                                      juce::Rectangle<int> area,
                                      const juce::String& text,
                                      juce::Colour colour)
{
    g.setColour (colour.withAlpha (0.62f));
    g.fillRoundedRectangle (area.toFloat(), 6.0f);

    g.setColour (colour);
    g.drawRoundedRectangle (area.toFloat(), 6.0f, 1.0f);

    g.setColour (AppTheme::text());
    g.setFont (juce::Font (11.5f, juce::Font::bold));
    g.drawText (text, area, juce::Justification::centred);
}
