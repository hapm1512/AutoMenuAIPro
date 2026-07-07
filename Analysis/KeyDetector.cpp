#include "KeyDetector.h"
#include <cmath>

namespace automenu::analysis
{
    namespace
    {
        constexpr std::array<float, 12> majorTemplate {
            6.35f, 2.23f, 3.48f, 2.33f, 4.38f, 4.09f,
            2.52f, 5.19f, 2.39f, 3.66f, 2.29f, 2.88f
        };

        constexpr std::array<float, 12> minorTemplate {
            6.33f, 2.68f, 3.52f, 5.38f, 2.60f, 3.53f,
            2.54f, 4.75f, 3.98f, 2.69f, 3.34f, 3.17f
        };
    }

    int KeyDetector::pitchClassFromFrequency (float frequencyHz)
    {
        if (frequencyHz <= 0.0f)
            return -1;

        const auto midi = (int) std::round (69.0 + 12.0 * std::log2 ((double) frequencyHz / 440.0));
        return (midi % 12 + 12) % 12;
    }

    juce::String KeyDetector::keyNameForPitchClass (int pitchClass)
    {
        static const juce::StringArray names { "C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B" };
        return names[pitchClass % 12];
    }

    juce::String KeyDetector::camelotForKey (int root, automenu::core::MusicalMode mode)
    {
        static const juce::StringArray minorCamelot { "5A", "12A", "7A", "2A", "9A", "4A", "11A", "6A", "1A", "8A", "3A", "10A" };
        static const juce::StringArray majorCamelot { "8B", "3B", "10B", "5B", "12B", "7B", "2B", "9B", "4B", "11B", "6B", "1B" };

        if (mode == automenu::core::MusicalMode::major)
            return majorCamelot[root % 12];

        if (mode == automenu::core::MusicalMode::minor)
            return minorCamelot[root % 12];

        return "--";
    }

    float KeyDetector::scoreTemplate (const std::array<float, 12>& chroma,
                                      const std::array<float, 12>& keyTemplate,
                                      int shift)
    {
        double score = 0.0;
        double energy = 0.0;

        for (int i = 0; i < 12; ++i)
        {
            const auto v = chroma[(i + shift) % 12];
            score += (double) v * (double) keyTemplate[(size_t) i];
            energy += (double) v * (double) v;
        }

        return energy <= 0.0 ? 0.0f : (float) (score / std::sqrt (energy));
    }

    automenu::core::ToneAnalysisResult KeyDetector::detect (const std::array<float, 12>& chroma,
                                                            float pitchHz,
                                                            float pitchConfidence,
                                                            float bpm) const
    {
        automenu::core::ToneAnalysisResult result;
        result.pitchHz = pitchHz;
        result.bpm = bpm;
        result.chroma = chroma;

        float bestScore = 0.0f;
        float secondScore = 0.0f;
        int bestRoot = 0;
        automenu::core::MusicalMode bestMode = automenu::core::MusicalMode::unknown;

        for (int root = 0; root < 12; ++root)
        {
            const auto majorScore = scoreTemplate (chroma, majorTemplate, root);
            const auto minorScore = scoreTemplate (chroma, minorTemplate, root);

            auto update = [&] (float score, automenu::core::MusicalMode mode)
            {
                if (score > bestScore)
                {
                    secondScore = bestScore;
                    bestScore = score;
                    bestRoot = root;
                    bestMode = mode;
                }
                else if (score > secondScore)
                {
                    secondScore = score;
                }
            };

            update (majorScore, automenu::core::MusicalMode::major);
            update (minorScore, automenu::core::MusicalMode::minor);
        }

        const auto pitchClass = pitchClassFromFrequency (pitchHz);
        if (pitchClass >= 0)
        {
            chroma[pitchClass];
            if (chroma[(size_t) pitchClass] > chroma[(size_t) bestRoot] * 0.85f)
                bestRoot = pitchClass;
        }

        const auto separation = juce::jlimit (0.0f, 1.0f, (bestScore - secondScore) / juce::jmax (bestScore, 1.0f));
        const auto combinedConfidence = juce::jlimit (0.0f, 1.0f, 0.35f * pitchConfidence + 0.65f * separation);

        result.keyName = keyNameForPitchClass (bestRoot);
        result.mode = bestMode;
        result.modeName = automenu::core::modeToString (bestMode);
        result.camelot = camelotForKey (bestRoot, bestMode);
        result.confidence = combinedConfidence;
        result.hasSignal = combinedConfidence > 0.12f;

        return result;
    }
}
