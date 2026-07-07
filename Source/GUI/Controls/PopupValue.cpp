#include "PopupValue.h"
#include "GUI/Theme.h"

PopupValue::PopupValue()
{
    setInterceptsMouseClicks (false, false);

    titleLabel.setJustificationType (juce::Justification::centred);
    titleLabel.setColour (juce::Label::textColourId, Theme::mutedText);
    titleLabel.setFont (Theme::bold (13.0f));

    valueLabel.setJustificationType (juce::Justification::centred);
    valueLabel.setColour (juce::Label::textColourId, Theme::gold);
    valueLabel.setFont (Theme::bold (23.0f));

    addAndMakeVisible (titleLabel);
    addAndMakeVisible (valueLabel);
    setVisible (false);
}

void PopupValue::showValue (const juce::String& title,
                            const juce::String& value,
                            juce::Rectangle<int> anchor)
{
    titleLabel.setText (title, juce::dontSendNotification);
    valueLabel.setText (value, juce::dontSendNotification);

    setBounds (anchor.withSizeKeepingCentre (150, 76).translated (0, -94));
    toFront (false);

    framesLeft = 45;
    setVisible (true);
    startTimerHz (30);
}

void PopupValue::hideValue()
{
    framesLeft = 0;
    setVisible (false);
    stopTimer();
}

void PopupValue::paint (juce::Graphics& g)
{
    auto r = getLocalBounds().toFloat().reduced (2.0f);

    g.setColour (juce::Colour (0xaa000000));
    g.fillRoundedRectangle (r.translated (0.0f, 3.0f), 8.0f);

    juce::ColourGradient grad (juce::Colour (0xff232424), r.getX(), r.getY(),
                               juce::Colour (0xff080909), r.getX(), r.getBottom(), false);
    g.setGradientFill (grad);
    g.fillRoundedRectangle (r, 8.0f);

    g.setColour (Theme::gold.withAlpha (0.85f));
    g.drawRoundedRectangle (r, 8.0f, 1.2f);
}

void PopupValue::resized()
{
    auto r = getLocalBounds().reduced (8, 6);
    titleLabel.setBounds (r.removeFromTop (24));
    valueLabel.setBounds (r);
}

void PopupValue::timerCallback()
{
    if (--framesLeft <= 0)
        hideValue();
}
