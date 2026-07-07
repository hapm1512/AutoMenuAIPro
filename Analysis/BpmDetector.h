#pragma once

#include <juce_core/juce_core.h>
#include <deque>

namespace AutoMenu
{
    class BPMDetector final
    {
    public:
        void reset();
        float processBlock (const float* samples, int numSamples, double sampleRate);
        float getBpm() const noexcept { return bpm; }

    private:
        float bpm = 0.0f;
        float previousEnergy = 0.0f;
        double timeSeconds = 0.0;
        double lastOnsetSeconds = -1.0;
        std::deque<double> intervals;
    };
}
