#pragma once
#include <JuceHeader.h>
#include "LevelMeter.h"

class BottomMeterBar final : public juce::Component
{
public:
    BottomMeterBar();

    void setLevels (float input, float output, float gr);
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    float inputLevel = 0.0f;
    float outputLevel = 0.0f;
    float grLevel = 0.0f;

    LevelMeter inputMeter;
    LevelMeter outputMeter;
    LevelMeter grMeter;

    juce::Label inputLabel;
    juce::Label outputLabel;
    juce::Label grLabel;
    juce::Label lufsLabel;
    juce::Label peakLabel;
    juce::Label clipInLabel;
    juce::Label clipOutLabel;
};
