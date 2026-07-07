#pragma once
#include <JuceHeader.h>

class PopupValue final : public juce::Component,
                         private juce::Timer
{
public:
    PopupValue();

    void showValue (const juce::String& title,
                    const juce::String& value,
                    juce::Rectangle<int> anchor);

    void hideValue();

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;

    juce::Label titleLabel;
    juce::Label valueLabel;
    int framesLeft = 0;
};
