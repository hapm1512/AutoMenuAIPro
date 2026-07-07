#pragma once
#include <JuceHeader.h>

class MainLookAndFeel final : public juce::LookAndFeel_V4
{
public:
    MainLookAndFeel();

    void drawButtonBackground (juce::Graphics&,
                               juce::Button&,
                               const juce::Colour&,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override;
};
