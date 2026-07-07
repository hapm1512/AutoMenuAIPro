#pragma once

#include <juce_core/juce_core.h>

namespace AutoMenu
{
    class PitchDetector final
    {
    public:
        struct Result
        {
            float hz = 0.0f;
            float confidence = 0.0f;
            bool valid = false;
        };

        Result detect (const float* samples, int numSamples, double sampleRate) const;

    private:
        static float rms (const float* samples, int numSamples);
    };
}
