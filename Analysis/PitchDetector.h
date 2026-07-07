#pragma once

#include <juce_core/juce_core.h>
#include <vector>

namespace automenu::analysis
{
    class PitchDetector final
    {
    public:
        struct Result
        {
            float frequencyHz = 0.0f;
            float confidence = 0.0f;
            bool valid = false;
        };

        Result detect (const std::vector<float>& samples, double sampleRate) const;

    private:
        static float rms (const std::vector<float>& samples);
    };
}
