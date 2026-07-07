#include "ToneStabilizer.h"

namespace AutoMenu
{
    void ToneStabilizer::reset()
    {
        votes.fill (0.0f);
        stable = ToneResult{};
        smoothedConfidence = 0.0f;
        smoothedBpm = 0.0f;
    }

    int ToneStabilizer::keyIndex (const juce::String& keyName)
    {
        static const char* names[] = { "C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B" };
        for (int i = 0; i < 12; ++i)
            if (keyName.equalsIgnoreCase (names[i]))
                return i;
        return -1;
    }

    ToneResult ToneStabilizer::process (const ToneResult& raw, bool hasSignal)
    {
        for (auto& v : votes)
            v *= hasSignal ? 0.965f : 0.92f;

        if (! hasSignal || ! raw.valid)
        {
            smoothedConfidence *= 0.88f;
            stable.confidence = smoothedConfidence;
            if (smoothedConfidence < 0.08f)
                stable.valid = false;
            return stable;
        }

        const int root = keyIndex (raw.keyName);
        const bool minor = raw.scaleName.equalsIgnoreCase ("Minor");

        if (root >= 0)
        {
            const int slot = root + (minor ? 12 : 0);
            votes[(size_t) slot] += juce::jlimit (0.03f, 1.0f, raw.confidence);
        }

        int bestSlot = 0;
        float best = -1.0f;
        float second = -1.0f;
        for (int i = 0; i < 24; ++i)
        {
            const float v = votes[(size_t) i];
            if (v > best)
            {
                second = best;
                best = v;
                bestSlot = i;
            }
            else if (v > second)
            {
                second = v;
            }
        }

        if (best > 0.02f)
        {
            const int stableRoot = bestSlot % 12;
            const bool stableMinor = bestSlot >= 12;
            stable.keyName = midiNoteName (stableRoot);
            stable.scaleName = stableMinor ? "Minor" : "Major";
            stable.camelot = camelotName (stableRoot, stableMinor);
            stable.valid = true;
        }

        stable.pitchHz = raw.pitchHz;
        smoothedBpm = raw.bpm > 1.0f
            ? (smoothedBpm <= 1.0f ? raw.bpm : smoothedBpm * 0.92f + raw.bpm * 0.08f)
            : smoothedBpm * 0.995f;
        stable.bpm = smoothedBpm;

        const float separation = best > 0.001f ? juce::jlimit (0.0f, 1.0f, (best - second) / best) : 0.0f;
        const float targetConfidence = juce::jlimit (0.0f, 1.0f, raw.confidence * 0.55f + separation * 0.45f);
        smoothedConfidence = smoothedConfidence * 0.86f + targetConfidence * 0.14f;
        stable.confidence = smoothedConfidence;

        return stable;
    }
}
