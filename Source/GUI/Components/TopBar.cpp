#include "TopBar.h"
#include "GUI/Theme.h"

TopBar::TopBar()
{
    auto setupLabel = [&] (juce::Label& l, const juce::String& text, float size, juce::Colour c)
    {
        l.setText (text, juce::dontSendNotification);
        l.setColour (juce::Label::textColourId, c);
        l.setFont (Theme::bold (size));
        l.setJustificationType (juce::Justification::centredLeft);
        addAndMakeVisible (l);
    };

    setupLabel (title, "VOCAL SUITE", 25.0f, Theme::textBright);
    setupLabel (subtitle, "ULTRA PRO", 19.0f, Theme::purple.brighter (0.38f));
    setupLabel (preset, "Modern Vocal Pop", 18.0f, juce::Colour (0xfff4f4f4));
    setupLabel (version, "VS", 35.0f, Theme::purple.brighter (0.25f));
    setupLabel (status, "48 kHz  •  128 Samples  •  0 smp  •  CPU 0.8%  •  OS 1x  •  HQ", 11.0f, Theme::mutedText);

    preset.setJustificationType (juce::Justification::centred);
    status.setJustificationType (juce::Justification::centredRight);

    for (auto* x : { &prev, &next, &favorite, &save, &category, &a, &b, &copy, &undo, &redo, &theme, &settings, &help, &bypass })
    {
        x->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff111213));
        x->setColour (juce::TextButton::buttonOnColourId, Theme::gold);
        x->setColour (juce::TextButton::textColourOffId, Theme::text);
        x->setColour (juce::TextButton::textColourOnId, juce::Colours::black);
        addAndMakeVisible (*x);
    }

    prev.setButtonText ("‹");
    next.setButtonText ("›");
    favorite.setButtonText ("★");
    save.setButtonText ("SAVE");
    category.setButtonText ("VOCAL");
    theme.setButtonText ("DARK");
    settings.setButtonText ("⚙");
    help.setButtonText ("?");
    bypass.setButtonText ("BYPASS");

    a.setButtonText ("A / B");
    b.setVisible (false);
    copy.setButtonText ("A > B");

    favorite.setClickingTogglesState (true);
    favorite.setToggleState (true, juce::dontSendNotification);
    bypass.setClickingTogglesState (true);
    bypass.setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xff7c1e1e));
    bypass.setColour (juce::TextButton::textColourOnId, juce::Colours::white);

    prev.onClick = [this] { if (onPreviousPreset) onPreviousPreset(); };
    next.onClick = [this] { if (onNextPreset) onNextPreset(); };
    save.onClick = [this] { if (onSavePreset) onSavePreset(); };
    a.onClick = [this] { if (onABSwap) onABSwap(); };
    copy.onClick = [this] { if (onCopyAToB) onCopyAToB(); };
    undo.onClick = [this] { if (onUndo) onUndo(); };
    redo.onClick = [this] { if (onRedo) onRedo(); };
    theme.onClick = [this] { if (onThemeToggle) onThemeToggle(); };
}

void TopBar::paint (juce::Graphics& g)
{
    auto r = getLocalBounds().toFloat().reduced (2.0f);

    g.setColour (juce::Colour (0xc3000000));
    g.fillRoundedRectangle (r.translated (0.0f, 2.0f), 7.0f);

    juce::ColourGradient grad (juce::Colour (0xff1d1f20), r.getX(), r.getY(),
                               juce::Colour (0xff070808), r.getX(), r.getBottom(), false);
    grad.addColour (0.42, juce::Colour (0xff111314));
    g.setGradientFill (grad);
    g.fillRoundedRectangle (r, 7.0f);

    g.setColour (juce::Colour (0xff2d3031));
    g.drawRoundedRectangle (r, 7.0f, 1.0f);

    auto logoBox = juce::Rectangle<float> (32.0f, 11.0f, 62.0f, 46.0f);
    g.setColour (Theme::purple.withAlpha (0.14f));
    g.fillEllipse (logoBox.expanded (12.0f));
    g.setColour (Theme::purple.withAlpha (0.26f));
    g.fillRoundedRectangle (logoBox, 7.0f);
    g.setColour (Theme::purple.withAlpha (0.62f));
    g.drawRoundedRectangle (logoBox, 7.0f, 1.0f);

    juce::Path v;
    v.startNewSubPath (logoBox.getX() + 12.0f, logoBox.getY() + 10.0f);
    v.lineTo (logoBox.getCentreX(), logoBox.getBottom() - 10.0f);
    v.lineTo (logoBox.getRight() - 12.0f, logoBox.getY() + 10.0f);
    g.setColour (Theme::textBright);
    g.strokePath (v, juce::PathStrokeType (3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    auto presetBox = juce::Rectangle<int> (515, 12, 360, 46).toFloat();

    g.setColour (juce::Colour (0xff050606));
    g.fillRoundedRectangle (presetBox, 6.0f);
    g.setColour (Theme::panelLine);
    g.drawRoundedRectangle (presetBox, 6.0f, 1.0f);

    g.setColour (juce::Colour (0x18ffffff));
    g.fillRoundedRectangle (presetBox.withHeight (presetBox.getHeight() * 0.38f).reduced (1.0f), 5.0f);

    g.setColour (Theme::mutedText.withAlpha (0.62f));
    g.setFont (Theme::regular (8.8f));
    g.drawText ("PRESET", presetBox.withTrimmedBottom (27.0f).reduced (12, 0), juce::Justification::centredLeft);

    g.setColour (Theme::gold);
    g.setFont (Theme::bold (22.0f));
    g.drawText ("★", presetBox.removeFromRight (48.0f), juce::Justification::centred);

    auto cpuBox = juce::Rectangle<float> (1095.0f, 15.0f, 66.0f, 42.0f);
    g.setColour (juce::Colour (0xff0b0d0e));
    g.fillRoundedRectangle (cpuBox, 5.0f);
    g.setColour (Theme::green.withAlpha (0.44f));
    g.drawRoundedRectangle (cpuBox, 5.0f, 1.0f);
    g.setColour (Theme::green);
    g.setFont (Theme::bold (12.0f));
    g.drawText ("CPU", cpuBox.withTrimmedBottom (18.0f), juce::Justification::centred);
    g.setColour (Theme::textBright);
    g.drawText ("0.8%", cpuBox.withTrimmedTop (17.0f), juce::Justification::centred);

    auto hqBox = juce::Rectangle<float> (1166.0f, 15.0f, 42.0f, 42.0f);
    g.setColour (juce::Colour (0xff0b0d0e));
    g.fillRoundedRectangle (hqBox, 5.0f);
    g.setColour (Theme::gold.withAlpha (0.45f));
    g.drawRoundedRectangle (hqBox, 5.0f, 1.0f);
    g.setColour (Theme::gold);
    g.setFont (Theme::bold (14.0f));
    g.drawText ("HQ", hqBox, juce::Justification::centred);

    g.setColour (Theme::mutedText.withAlpha (0.55f));
    g.drawVerticalLine (815, 18.0f, 52.0f);
}

void TopBar::resized()
{
    version.setBounds (34, 10, 70, 50);
    title.setBounds (118, 11, 180, 32);
    subtitle.setBounds (305, 13, 135, 30);
    status.setBounds (910, 49, 260, 20);

    prev.setBounds (518, 18, 38, 34);
    preset.setBounds (558, 19, 255, 32);
    next.setBounds (836, 18, 34, 34);

    a.setBounds (910, 15, 78, 42);
    copy.setBounds (995, 15, 82, 42);

    undo.setBounds (1216, 15, 50, 42);
    redo.setBounds (1268, 15, 50, 42);
    theme.setBounds (1324, 15, 70, 42);
    settings.setBounds (1400, 15, 42, 42);
    help.setBounds (1446, 15, 42, 42);
    bypass.setBounds (1494, 15, 80, 42);

    favorite.setBounds (0, 0, 0, 0);
    save.setBounds (0, 0, 0, 0);
    category.setBounds (0, 0, 0, 0);
}

void TopBar::setPresetName (const juce::String& name)
{
    preset.setText (name, juce::dontSendNotification);
}

void TopBar::setStatusText (const juce::String& text)
{
    status.setText (text, juce::dontSendNotification);
}


void TopBar::setThemeIsLight (bool shouldUseLightTheme)
{
    lightTheme = shouldUseLightTheme;
    theme.setButtonText (lightTheme ? "LIGHT" : "DARK");
    repaint();
}
