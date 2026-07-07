#pragma once

#include <vector>

namespace automenu::analysis
{
    class BpmDetector final
    {
    public:
        float detect (const std::vector<float>& samples, double sampleRate);

    private:
        float lastBpm = 0.0f;
    };
}
