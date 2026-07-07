#pragma once
#include <JuceHeader.h>

class LevelMeter final : public juce::Component
{
public:
    void setLevel (float value);
    void paint (juce::Graphics&) override;

private:
    float level = 0.0f;
    float peakHold = 0.0f;
    float rmsGhost = 0.0f;
};
