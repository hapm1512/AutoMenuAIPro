#include "PitchDetector.h"
#include <cmath>

namespace automenu::analysis
{
    float PitchDetector::rms (const std::vector<float>& samples)
    {
        double sum = 0.0;
        for (auto s : samples)
            sum += (double) s * (double) s;

        return samples.empty() ? 0.0f : (float) std::sqrt (sum / (double) samples.size());
    }

    PitchDetector::Result PitchDetector::detect (const std::vector<float>& samples, double sampleRate) const
    {
        Result result;

        if (samples.size() < 1024 || sampleRate <= 0.0)
            return result;

        const auto level = rms (samples);
        if (level < 0.0025f)
            return result;

        const int minLag = (int) (sampleRate / 1200.0);
        const int maxLag = juce::jmin ((int) (sampleRate / 55.0), (int) samples.size() / 2);

        float bestCorr = 0.0f;
        int bestLag = 0;

        for (int lag = minLag; lag <= maxLag; ++lag)
        {
            double corr = 0.0;
            double energyA = 0.0;
            double energyB = 0.0;

            const int count = (int) samples.size() - lag;
            for (int i = 0; i < count; i += 2)
            {
                const auto a = samples[(size_t) i];
                const auto b = samples[(size_t) (i + lag)];
                corr += (double) a * (double) b;
                energyA += (double) a * (double) a;
                energyB += (double) b * (double) b;
            }

            const auto denom = std::sqrt (energyA * energyB) + 1.0e-9;
            const auto normalised = (float) (corr / denom);

            if (normalised > bestCorr)
            {
                bestCorr = normalised;
                bestLag = lag;
            }
        }

        if (bestLag <= 0 || bestCorr < 0.28f)
            return result;

        result.frequencyHz = (float) (sampleRate / (double) bestLag);
        result.confidence = juce::jlimit (0.0f, 1.0f, bestCorr);
        result.valid = true;
        return result;
    }
}
