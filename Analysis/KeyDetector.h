#pragma once

#include <array>
#include <juce_core/juce_core.h>
#include "../Core/AnalysisTypes.h"

namespace AutoMenu
{
    class KeyDetector final
    {
    public:
        ToneResult detectFromPitch (float pitchHz, float pitchConfidence, float bpm) const;
        ToneResult detectFromChroma (const std::array<float, 12>& chroma, float bpm) const;

    private:
        static int pitchToMidi (float hz);
        static float profileScore (const std::array<float, 12>& chroma, const std::array<float, 12>& profile, int root);
    };
}
