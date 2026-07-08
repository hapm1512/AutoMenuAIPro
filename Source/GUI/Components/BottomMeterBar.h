#pragma once
#include <JuceHeader.h>
#include "LevelMeter.h"

class BottomMeterBar final : public juce::Component
{
public:
    BottomMeterBar();

    void setLevels (float input, float output, float gr);
    void setProfessionalMetering (float inputPeak, float outputPeak,
                                  float inputRms, float outputRms,
                                  float truePeak,
                                  float gainReductionDb,
                                  float lufsIntegrated,
                                  float lufsShortTerm,
                                  float lufsMomentary,
                                  float correlation);
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    static juce::String dbText (float amp);
    static juce::String lufsText (float value);

    float inputLevel = 0.0f;
    float outputLevel = 0.0f;
    float inputRmsLevel = 0.0f;
    float outputRmsLevel = 0.0f;
    float grLevel = 0.0f;
    float truePeakLevel = 0.0f;
    float correlationValue = 1.0f;
    float lufsI = -70.0f;
    float lufsS = -70.0f;
    float lufsM = -70.0f;

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

    juce::Label lufsShortLabel;
    juce::Label momentaryLabel;
    juce::Label corrLabel;
    juce::TextButton monoButton { "MONO" };
    juce::TextButton stereoButton { "STEREO" };
    juce::TextButton midButton { "MID" };
    juce::TextButton sideButton { "SIDE" };
};
