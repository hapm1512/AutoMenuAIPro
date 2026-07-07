#pragma once

#include <array>
#include "../Core/AnalysisTypes.h"

namespace AutoMenu
{
    class ToneStabilizer final
    {
    public:
        void reset();
        ToneResult process (const ToneResult& raw, bool hasSignal);

    private:
        static int keyIndex (const juce::String& keyName);

        std::array<float, 24> votes{};
        ToneResult stable;
        float smoothedConfidence = 0.0f;
        float smoothedBpm = 0.0f;
    };
}
