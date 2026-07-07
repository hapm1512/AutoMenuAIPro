#include "KeyDetector.h"
#include <cmath>

namespace AutoMenu
{
    static constexpr std::array<float, 12> majorProfile { 6.35f, 2.23f, 3.48f, 2.33f, 4.38f, 4.09f, 2.52f, 5.19f, 2.39f, 3.66f, 2.29f, 2.88f };
    static constexpr std::array<float, 12> minorProfile { 6.33f, 2.68f, 3.52f, 5.38f, 2.60f, 3.53f, 2.54f, 4.75f, 3.98f, 2.69f, 3.34f, 3.17f };

    int KeyDetector::pitchToMidi (float hz)
    {
        if (hz <= 0.0f)
            return -1;
        return (int) std::round (69.0 + 12.0 * std::log2 ((double) hz / 440.0));
    }

    float KeyDetector::profileScore (const std::array<float, 12>& chroma, const std::array<float, 12>& profile, int root)
    {
        float score = 0.0f;
        for (int i = 0; i < 12; ++i)
            score += chroma[(size_t) ((root + i) % 12)] * profile[(size_t) i];
        return score;
    }

    ToneResult KeyDetector::detectFromPitch (float pitchHz, float pitchConfidence, float bpm) const
    {
        ToneResult result;
        const int midi = pitchToMidi (pitchHz);

        if (midi < 0)
            return result;

        const int root = ((midi % 12) + 12) % 12;
        result.keyName = midiNoteName (root);
        result.scaleName = "Minor"; // Live vocal default. Chroma detector can override this later.
        result.camelot = camelotName (root, true);
        result.confidence = juce::jlimit (0.0f, 1.0f, pitchConfidence);
        result.pitchHz = pitchHz;
        result.bpm = bpm;
        result.valid = true;
        return result;
    }

    ToneResult KeyDetector::detectFromChroma (const std::array<float, 12>& chroma, float bpm) const
    {
        ToneResult result;

        float total = 0.0f;
        for (auto v : chroma)
            total += v;

        if (total <= 0.0001f)
            return result;

        int bestRoot = 0;
        bool bestMinor = false;
        float best = -1.0f;
        float second = -1.0f;

        for (int root = 0; root < 12; ++root)
        {
            const float maj = profileScore (chroma, majorProfile, root);
            const float min = profileScore (chroma, minorProfile, root);

            auto update = [&] (float score, bool minor)
            {
                if (score > best)
                {
                    second = best;
                    best = score;
                    bestRoot = root;
                    bestMinor = minor;
                }
                else if (score > second)
                {
                    second = score;
                }
            };

            update (maj, false);
            update (min, true);
        }

        result.keyName = midiNoteName (bestRoot);
        result.scaleName = bestMinor ? "Minor" : "Major";
        result.camelot = camelotName (bestRoot, bestMinor);
        result.confidence = juce::jlimit (0.0f, 1.0f, (best - second) / juce::jmax (best, 0.001f));
        result.bpm = bpm;
        result.valid = true;
        return result;
    }
}
