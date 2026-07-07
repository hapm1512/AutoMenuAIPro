#pragma once

#include <array>
#include <vector>
#include "../Core/AnalysisTypes.h"

namespace automenu::analysis
{
    class KeyDetector final
    {
    public:
        automenu::core::ToneAnalysisResult detect (const std::array<float, 12>& chroma,
                                                   float pitchHz,
                                                   float pitchConfidence,
                                                   float bpm) const;

        static int pitchClassFromFrequency (float frequencyHz);
        static juce::String keyNameForPitchClass (int pitchClass);
        static juce::String camelotForKey (int root, automenu::core::MusicalMode mode);

    private:
        static float scoreTemplate (const std::array<float, 12>& chroma,
                                    const std::array<float, 12>& keyTemplate,
                                    int shift);
    };
}
