#include "ModulePanel.h"
#include "GUI/Theme.h"

ModulePanel::ModulePanel (juce::AudioProcessorValueTreeState& state,
                          const juce::String& title,
                          const juce::String& onParam)
    : apvts (state), titleText (title)
{
    onButton.setClickingTogglesState (true);
    onButton.setColour (juce::TextButton::buttonColourId, juce::Colour (0xff121212));
    onButton.setColour (juce::TextButton::buttonOnColourId, Theme::gold);
    onButton.setColour (juce::TextButton::textColourOffId, Theme::text);
    onButton.setColour (juce::TextButton::textColourOnId, juce::Colours::black);

    bypassButton.setClickingTogglesState (true);
    bypassButton.setColour (juce::TextButton::buttonColourId, juce::Colour (0xff121212));
    bypassButton.setColour (juce::TextButton::buttonOnColourId, juce::Colour (0xff6b1b1b));
    bypassButton.setColour (juce::TextButton::textColourOffId, Theme::mutedText);
    bypassButton.setColour (juce::TextButton::textColourOnId, juce::Colours::white);

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
    auto r = getLocalBounds().toFloat();
    Theme::drawInsetPanel (g, r.reduced (1.0f), 7.0f);

    auto header = getLocalBounds().removeFromTop (46);
    Theme::drawSectionHeader (g, header, titleText);

    const bool on = onButton.getToggleState();
    const bool bypassed = bypassButton.getToggleState();
    auto led = juce::Rectangle<float> (17.0f, 17.0f, 10.0f, 10.0f);
    Theme::drawSmallLed (g, led, on && ! bypassed, bypassed ? Theme::red : Theme::green);

    g.setColour ((on && ! bypassed ? Theme::gold : Theme::mutedText).withAlpha (0.42f));
    g.fillRoundedRectangle (juce::Rectangle<float> (4.0f, 53.0f, 3.0f, (float) getHeight() - 66.0f), 1.5f);

    g.setColour (Theme::mutedText.withAlpha (0.58f));
    g.setFont (Theme::regular (9.5f));
    g.drawText (bypassed ? "BYPASSED" : (on ? "ACTIVE" : "OFF"),
                getLocalBounds().removeFromBottom (20).reduced (8, 0),
                juce::Justification::centredRight);

    auto body = getLocalBounds().withTrimmedTop (48).reduced (8, 8).toFloat();
    g.setColour (juce::Colour (0x24000000));
    g.fillRoundedRectangle (body, 5.0f);
}

void ModulePanel::resized()
{
    auto r = getLocalBounds();
    auto top = r.removeFromTop (46).reduced (10, 9);

    onButton.setBounds (top.removeFromLeft (38));
    bypassButton.setBounds (top.removeFromRight (73));

    auto body = r.reduced (9, 10);
    const int knobH = juce::jlimit (88, 104, body.getHeight() / juce::jmax (1, knobs.size()));

    for (auto* k : knobs)
        k->setBounds (body.removeFromTop (knobH).reduced (0, 2));
}
