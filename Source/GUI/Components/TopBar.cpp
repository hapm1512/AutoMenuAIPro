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

    setupLabel (title, "VOCAL SUITE", 29.0f, Theme::textBright);
    setupLabel (subtitle, "ULTRA PRO", 23.0f, Theme::gold);
    setupLabel (preset, "Modern Vocal - Clean & Wide", 20.0f, juce::Colour (0xffe5c875));
    setupLabel (version, "v0.3 GUI PRO", 11.0f, Theme::mutedText);
    setupLabel (status, "48 kHz  •  128 Samples  •  CPU 0.8%  •  OS 1x", 12.0f, Theme::mutedText);
    preset.setJustificationType (juce::Justification::centred);
    version.setJustificationType (juce::Justification::centredLeft);
    status.setJustificationType (juce::Justification::centredRight);

    for (auto* x : { &prev, &next, &favorite, &save, &category, &a, &b, &copy, &undo, &redo, &settings, &help, &bypass })
    {
        x->setColour (juce::TextButton::buttonColourId, juce::Colour (0xff111111));
        x->setColour (juce::TextButton::buttonOnColourId, Theme::gold);
        x->setColour (juce::TextButton::textColourOffId, Theme::text);
        x->setColour (juce::TextButton::textColourOnId, juce::Colours::black);
        addAndMakeVisible (*x);
    }

    a.setClickingTogglesState (true);
    b.setClickingTogglesState (true);
    a.setToggleState (true, juce::dontSendNotification);
    favorite.setClickingTogglesState (true);
    bypass.setClickingTogglesState (true);
    bypass.setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xff7c1e1e));
    bypass.setColour (juce::TextButton::textColourOnId, juce::Colours::white);
}

void TopBar::paint (juce::Graphics& g)
{
    auto r = getLocalBounds().toFloat().reduced (2.0f);
    g.setColour (juce::Colour (0x77000000));
    g.fillRoundedRectangle (r.translated (0.0f, 2.0f), 8.0f);

    juce::ColourGradient grad (juce::Colour (0xff1c1d1d), r.getX(), r.getY(),
                               juce::Colour (0xff090a0a), r.getX(), r.getBottom(), false);
    g.setGradientFill (grad);
    g.fillRoundedRectangle (r, 8.0f);

    g.setColour (juce::Colour (0xff353535));
    g.drawRoundedRectangle (r, 8.0f, 1.0f);

    juce::Path logo;
    logo.startNewSubPath (25, 18);
    logo.lineTo (65, 78);
    logo.lineTo (105, 18);
    logo.lineTo (90, 18);
    logo.lineTo (65, 56);
    logo.lineTo (40, 18);
    logo.closeSubPath();

    g.setColour (Theme::gold.withAlpha (0.18f));
    g.strokePath (logo, juce::PathStrokeType (8.0f));
    g.setColour (Theme::textBright);
    g.strokePath (logo, juce::PathStrokeType (3.0f));

    auto p = juce::Rectangle<int> (418, 13, 570, 47).toFloat();
    g.setColour (juce::Colour (0xff080909));
    g.fillRoundedRectangle (p, 5.0f);
    g.setColour (Theme::gold.withAlpha (0.55f));
    g.drawRoundedRectangle (p, 5.0f, 1.0f);

    g.setColour (Theme::mutedText);
    g.setFont (Theme::regular (10.0f));
    g.drawText ("PRESET BROWSER", p.withTrimmedBottom (26.0f), juce::Justification::centred);

    Theme::drawSmallLed (g, juce::Rectangle<float> (1168.0f, 24.0f, 9.0f, 9.0f), true, Theme::green);
    Theme::drawSmallLed (g, juce::Rectangle<float> (1168.0f, 45.0f, 9.0f, 9.0f), false, Theme::red);
}

void TopBar::resized()
{
    title.setBounds (112, 7, 245, 37);
    subtitle.setBounds (112, 45, 220, 32);
    version.setBounds (270, 51, 140, 22);
    status.setBounds (1000, 60, 260, 24);

    prev.setBounds (424, 19, 38, 35);
    category.setBounds (466, 19, 74, 35);
    preset.setBounds (544, 20, 330, 33);
    favorite.setBounds (879, 19, 38, 35);
    save.setBounds (920, 19, 62, 35);
    next.setBounds (991, 19, 38, 35);

    a.setBounds (500, 66, 55, 26);
    b.setBounds (558, 66, 55, 26);
    copy.setBounds (625, 66, 75, 26);
    undo.setBounds (750, 66, 82, 26);
    redo.setBounds (835, 66, 82, 26);

    settings.setBounds (1190, 20, 90, 70);
    help.setBounds (1295, 20, 90, 70);
    bypass.setBounds (1425, 20, 95, 70);
}
