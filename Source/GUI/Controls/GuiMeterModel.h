#pragma once
#include <JuceHeader.h>

struct GuiMeterModel
{
    float inputPeak = 0.0f;
    float outputPeak = 0.0f;
    float gainReduction = 0.0f;
    float inputRms = 0.0f;
    float outputRms = 0.0f;
    bool inputClip = false;
    bool outputClip = false;

    void reset()
    {
        inputPeak = outputPeak = gainReduction = 0.0f;
        inputRms = outputRms = 0.0f;
        inputClip = outputClip = false;
    }
};
