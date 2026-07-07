#include "PitchDetector.h"
#include <cmath>

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

        if (samples == nullptr || numSamples < 512 || sampleRate <= 0.0)
            return result;

        const float level = rms (samples, numSamples);
        if (level < 0.003f)
            return result;

        const int minLag = juce::jmax (1, (int) (sampleRate / 1200.0));
        const int maxLag = juce::jmin (numSamples / 2, (int) (sampleRate / 55.0));

        float bestCorr = 0.0f;
        int bestLag = 0;

        for (int lag = minLag; lag <= maxLag; ++lag)
        {
            double corr = 0.0;
            double energyA = 0.0;
            double energyB = 0.0;

            const int count = numSamples - lag;
            for (int i = 0; i < count; ++i)
            {
                const double a = samples[i];
                const double b = samples[i + lag];
                corr += a * b;
                energyA += a * a;
                energyB += b * b;
            }

            const double denom = std::sqrt (energyA * energyB) + 1.0e-12;
            const float norm = (float) (corr / denom);

            if (norm > bestCorr)
            {
                bestCorr = norm;
                bestLag = lag;
            }
        }

        if (bestLag > 0 && bestCorr > 0.35f)
        {
            result.hz = (float) (sampleRate / (double) bestLag);
            result.confidence = juce::jlimit (0.0f, 1.0f, (bestCorr - 0.35f) / 0.55f);
            result.valid = true;
        }

        return result;
    }
}
