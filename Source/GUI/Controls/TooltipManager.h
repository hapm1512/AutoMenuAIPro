#pragma once
#include <JuceHeader.h>

class TooltipManager final : public juce::Component,
                             private juce::Timer
{
public:
    TooltipManager();

    void showTip (const juce::String& text, juce::Point<int> screenPos);
    void hideTip();

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    juce::Label textLabel;
    int framesLeft = 0;
};
