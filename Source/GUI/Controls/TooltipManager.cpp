#include "TooltipManager.h"
#include "GUI/Theme.h"

TooltipManager::TooltipManager()
{
    setInterceptsMouseClicks (false, false);

    textLabel.setJustificationType (juce::Justification::centred);
    textLabel.setColour (juce::Label::textColourId, Theme::text);
    textLabel.setFont (Theme::bold (13.0f));
    addAndMakeVisible (textLabel);

    setVisible (false);
}

void TooltipManager::showTip (const juce::String& text, juce::Point<int> screenPos)
{
    textLabel.setText (text, juce::dontSendNotification);

    if (auto* parent = getParentComponent())
    {
        auto local = parent->getLocalPoint (nullptr, screenPos);
        setBounds (local.x - 90, local.y - 48, 180, 34);
        toFront (false);
    }

    framesLeft = 90;
    setVisible (true);
    startTimerHz (30);
}

void TooltipManager::hideTip()
{
    framesLeft = 0;
    setVisible (false);
    stopTimer();
}

void TooltipManager::paint (juce::Graphics& g)
{
    auto r = getLocalBounds().toFloat().reduced (1.0f);

    g.setColour (juce::Colour (0xaa000000));
    g.fillRoundedRectangle (r.translated (0.0f, 2.0f), 5.0f);

    g.setColour (juce::Colour (0xff161717));
    g.fillRoundedRectangle (r, 5.0f);

    g.setColour (Theme::gold);
    g.drawRoundedRectangle (r, 5.0f, 1.0f);
}

void TooltipManager::resized()
{
    textLabel.setBounds (getLocalBounds().reduced (8, 4));
}

void TooltipManager::timerCallback()
{
    if (--framesLeft <= 0)
        hideTip();
}
