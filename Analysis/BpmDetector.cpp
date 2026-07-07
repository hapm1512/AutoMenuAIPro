#include "BPMDetector.h"
#include <cmath>

namespace AutoMenu
{
    void BPMDetector::reset()
    {
        bpm = 0.0f;
        previousEnergy = 0.0f;
        timeSeconds = 0.0;
        lastOnsetSeconds = -1.0;
        intervals.clear();
    }

    float BPMDetector::processBlock (const float* samples, int numSamples, double sampleRate)
    {
        if (samples == nullptr || numSamples <= 0 || sampleRate <= 0.0)
            return bpm;

        double sum = 0.0;
        for (int i = 0; i < numSamples; ++i)
            sum += (double) samples[i] * samples[i];

        const float energy = (float) std::sqrt (sum / (double) numSamples);
        const float threshold = juce::jmax (0.018f, previousEnergy * 1.55f);
        const bool onset = energy > threshold;

        if (onset)
        {
            if (lastOnsetSeconds > 0.0)
            {
                const double interval = timeSeconds - lastOnsetSeconds;
                if (interval >= 0.25 && interval <= 1.5)
                {
                    intervals.push_back (interval);
                    while (intervals.size() > 16)
                        intervals.pop_front();

                    double avg = 0.0;
                    for (auto v : intervals)
                        avg += v;

                    if (! intervals.empty())
                    {
                        avg /= (double) intervals.size();
                        bpm = (float) juce::jlimit (60.0, 220.0, 60.0 / avg);
                    }
                }
            }

            lastOnsetSeconds = timeSeconds;
        }

        previousEnergy = previousEnergy * 0.92f + energy * 0.08f;
        timeSeconds += (double) numSamples / sampleRate;
        return bpm;
    }
}
