#include "ModulePanel.h"
#include "GUI/Theme.h"

ModulePanel::ModulePanel (juce::AudioProcessorValueTreeState& state,
                          const juce::String& title,
                          const juce::String& onParam)
    : apvts (state), titleText (title)
{
    onButton.setClickingTogglesState (true);
    onButton.setColour (juce::TextButton::buttonColourId, juce::Colour (0xff121212));
    onButton.setColour (juce::TextButton::buttonOnColourId, Theme::green);
    onButton.setColour (juce::TextButton::textColourOffId, Theme::text);
    onButton.setColour (juce::TextButton::textColourOnId, juce::Colours::black);
    onButton.setTooltip ("Enable module");

    bypassButton.setClickingTogglesState (true);
    bypassButton.setColour (juce::TextButton::buttonColourId, juce::Colour (0xff121212));
    bypassButton.setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xff702020));
    bypassButton.setColour (juce::TextButton::textColourOffId, Theme::mutedText);
    bypassButton.setColour (juce::TextButton::textColourOnId, juce::Colours::white);
    bypassButton.setTooltip ("Bypass module");

    onAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>
        (apvts, onParam, onButton);

    addAndMakeVisible (onButton);
    addAndMakeVisible (bypassButton);
}

void ModulePanel::addKnob (const juce::String& text,
                           const juce::String& paramId,
                           juce::Colour fill)
{
    auto* k = new Knob (apvts, paramId, text, fill);
    knobs.add (k);
    addAndMakeVisible (k);
}

void ModulePanel::paint (juce::Graphics& g)
{
    auto full = getLocalBounds().toFloat();

    juce::Colour headerTint = Theme::text;
    if (titleText.containsIgnoreCase ("PREAMP")) headerTint = Theme::red.brighter (0.35f);
    if (titleText == "EQ") headerTint = Theme::blue.brighter (0.20f);
    if (titleText.containsIgnoreCase ("GATE")) headerTint = Theme::purple.brighter (0.25f);
    if (titleText.containsIgnoreCase ("DE-ESSER")) headerTint = juce::Colour (0xff4fe3e9);
    if (titleText.containsIgnoreCase ("COMP")) headerTint = Theme::blue.brighter (0.18f);
    if (titleText.containsIgnoreCase ("SATURATION")) headerTint = Theme::gold.brighter (0.25f);
    if (titleText.containsIgnoreCase ("HI-END")) headerTint = Theme::purple.brighter (0.35f);
    if (titleText.containsIgnoreCase ("LIMITER")) headerTint = Theme::textBright;

    g.setColour (juce::Colour (0xc8000000));
    g.fillRoundedRectangle (full.reduced (1.0f).translated (0.0f, 2.8f), 7.0f);

    juce::ColourGradient grad (juce::Colour (0xff35383a), full.getX(), full.getY(),
                               juce::Colour (0xff08090a), full.getX(), full.getBottom(), false);
    grad.addColour (0.18, juce::Colour (0xff1a1c1d));
    grad.addColour (0.73, juce::Colour (0xff101112));
    g.setGradientFill (grad);
    g.fillRoundedRectangle (full.reduced (1.0f), 7.0f);

    g.setColour (Theme::panelLine.withAlpha (0.84f));
    g.drawRoundedRectangle (full.reduced (1.0f), 7.0f, 1.0f);

    const bool on = onButton.getToggleState();
    const bool bypassed = bypassButton.getToggleState();
    const bool active = on && ! bypassed;

    auto header = getLocalBounds().removeFromTop (50);

    g.setColour (juce::Colour (0x65000000));
    g.fillRoundedRectangle (header.toFloat().reduced (3.0f), 5.0f);

    g.setColour (headerTint.withAlpha (0.17f));
    g.fillRoundedRectangle (header.toFloat().reduced (5.0f, 6.0f), 4.0f);

    g.setColour (headerTint);
    g.setFont (Theme::bold (13.4f));
    g.drawText (titleText, header.reduced (9, 0).withTrimmedRight (30), juce::Justification::centredLeft);

    auto led = juce::Rectangle<float> ((float) getWidth() - 25.0f, 18.0f, 10.0f, 10.0f);
    Theme::drawSmallLed (g, led, active, bypassed ? Theme::red : Theme::green);

    if (active)
    {
        g.setColour (headerTint.withAlpha (0.060f));
        g.fillRoundedRectangle (full.reduced (5.0f, 56.0f).withTrimmedBottom (6.0f), 6.0f);

        g.setColour (headerTint.withAlpha (0.38f));
        g.drawRoundedRectangle (full.reduced (2.0f), 7.0f, 1.0f);
    }

    auto bottomLed = juce::Rectangle<float> (full.getCentreX() - 5.0f, full.getBottom() - 17.0f, 10.0f, 10.0f);
    Theme::drawSmallLed (g, bottomLed, active, headerTint);

    g.setColour (headerTint.withAlpha (0.22f));
    g.drawLine (7.0f, 53.0f, full.getRight() - 7.0f, 53.0f, 1.0f);

    g.setColour (Theme::mutedText.withAlpha (0.54f));
    g.setFont (Theme::regular (8.8f));
    g.drawText (bypassed ? "BYPASSED" : (on ? "ACTIVE" : "OFF"),
                getLocalBounds().removeFromBottom (18).reduced (8, 0),
                juce::Justification::centredRight);
}

void ModulePanel::resized()
{
    auto r = getLocalBounds();
    auto top = r.removeFromTop (50).reduced (8, 10);

    onButton.setBounds (top.removeFromLeft (34));
    bypassButton.setBounds (top.removeFromRight (0));

    auto body = r.reduced (7, 8);
    const int count = juce::jmax (1, knobs.size());
    const int knobH = juce::jlimit (62, 94, body.getHeight() / count);

    for (auto* k : knobs)
        k->setBounds (body.removeFromTop (knobH).reduced (0, 1));
}


void ModulePanel::setThemeIsLight (bool shouldUseLightTheme)
{
    lightTheme = shouldUseLightTheme;
    onButton.setColour (juce::TextButton::buttonColourId, lightTheme ? juce::Colour (0xffded8c8) : juce::Colour (0xff121212));
    bypassButton.setColour (juce::TextButton::buttonColourId, lightTheme ? juce::Colour (0xffded8c8) : juce::Colour (0xff121212));
    repaint();
}
