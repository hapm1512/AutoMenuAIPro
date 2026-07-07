#include "PitchDetector.h"
#include <cmath>
#include <vector>

namespace AutoMenu
{
    float PitchDetector::rms (const float* samples, int numSamples)
    {
        if (samples == nullptr || numSamples <= 0)
            return 0.0f;

        double sum = 0.0;
        for (int i = 0; i < numSamples; ++i)
            sum += (double) samples[i] * samples[i];

        return (float) std::sqrt (sum / (double) numSamples);
    }

    PitchDetector::Result PitchDetector::detect (const float* samples, int numSamples, double sampleRate) const
    {
        Result result;

        if (samples == nullptr || numSamples < 1024 || sampleRate <= 0.0)
            return result;

        const float level = rms (samples, numSamples);
        if (level < 0.004f)
            return result;

        const int minLag = juce::jmax (1, (int) (sampleRate / 1200.0));
        const int maxLag = juce::jmin (numSamples / 2 - 2, (int) (sampleRate / 45.0));

        if (maxLag <= minLag + 4)
            return result;

        std::vector<float> cmnd ((size_t) maxLag + 1, 1.0f);
        double runningSum = 0.0;

        for (int lag = 1; lag <= maxLag; ++lag)
        {
            double diff = 0.0;
            const int count = numSamples - lag;

            for (int i = 0; i < count; ++i)
            {
                const double d = (double) samples[i] - (double) samples[i + lag];
                diff += d * d;
            }

            runningSum += diff;
            cmnd[(size_t) lag] = runningSum > 1.0e-12
                ? (float) ((double) lag * diff / runningSum)
                : 1.0f;
        }

        int bestLag = 0;
        float bestValue = 1.0f;
        const float threshold = 0.18f;

        for (int lag = minLag; lag <= maxLag; ++lag)
        {
            const float v = cmnd[(size_t) lag];
            if (v < threshold)
            {
                while (lag + 1 <= maxLag && cmnd[(size_t) (lag + 1)] < v)
                    ++lag;
                bestLag = lag;
                bestValue = cmnd[(size_t) lag];
                break;
            }

            if (v < bestValue)
            {
                bestValue = v;
                bestLag = lag;
            }
        }

        if (bestLag <= 0 || bestValue > 0.42f)
            return result;

        float refinedLag = (float) bestLag;
        if (bestLag > 1 && bestLag < maxLag - 1)
        {
            const float y0 = cmnd[(size_t) (bestLag - 1)];
            const float y1 = cmnd[(size_t) bestLag];
            const float y2 = cmnd[(size_t) (bestLag + 1)];
            const float denom = (y0 - 2.0f * y1 + y2);
            if (std::abs (denom) > 1.0e-6f)
                refinedLag += 0.5f * (y0 - y2) / denom;
        }

        const float hz = (float) (sampleRate / (double) refinedLag);
        if (hz < 45.0f || hz > 1200.0f)
            return result;

        result.hz = hz;
        result.confidence = juce::jlimit (0.0f, 1.0f, 1.0f - bestValue * 2.1f);
        result.valid = result.confidence > 0.22f;
        return result;
    }
}
