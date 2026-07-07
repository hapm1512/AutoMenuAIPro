#include "BpmDetector.h"
#include <cmath>
#include <algorithm>

namespace automenu::analysis
{
    float BpmDetector::detect (const std::vector<float>& samples, double sampleRate)
    {
        if (samples.size() < 4096 || sampleRate <= 0.0)
            return lastBpm;

        const int hop = 512;
        std::vector<float> envelope;

        for (size_t pos = 0; pos + (size_t) hop < samples.size(); pos += (size_t) hop)
        {
            double sum = 0.0;
            for (int i = 0; i < hop; ++i)
                sum += std::abs (samples[pos + (size_t) i]);

            envelope.push_back ((float) (sum / hop));
        }

        if (envelope.size() < 16)
            return lastBpm;

        const double envRate = sampleRate / hop;
        int bestLag = 0;
        float bestScore = 0.0f;

        const int minLag = (int) (envRate * 60.0 / 180.0);
        const int maxLag = (int) (envRate * 60.0 / 60.0);

        for (int lag = minLag; lag <= maxLag; ++lag)
        {
            float score = 0.0f;
            for (size_t i = 0; i + (size_t) lag < envelope.size(); ++i)
                score += envelope[i] * envelope[i + (size_t) lag];

            if (score > bestScore)
            {
                bestScore = score;
                bestLag = lag;
            }
        }

        if (bestLag > 0)
        {
            const auto bpm = (float) (60.0 * envRate / bestLag);
            if (bpm >= 60.0f && bpm <= 180.0f)
                lastBpm = lastBpm <= 0.0f ? bpm : (0.85f * lastBpm + 0.15f * bpm);
        }

        return lastBpm;
    }
}
