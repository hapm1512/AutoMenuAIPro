#include "ToneEngine.h"
#include <cmath>

namespace AutoMenu
{
    void ToneEngine::prepare (double sampleRateToUse)
    {
        sampleRate = sampleRateToUse > 0.0 ? sampleRateToUse : 48000.0;
        noiseGate.prepare (sampleRate);
        reset();
    }

    void ToneEngine::reset()
    {
        noiseGate.reset();
        bpmDetector.reset();
        chroma.fill (0.0f);
        history.clear();
        state = RealtimeToneState{};
        stableTone = ToneResult{};
    }

    AnalysisResult ToneEngine::processBlock (juce::AudioBuffer<float>& monoBlock, double timestampSeconds)
    {
        AnalysisResult result;
        result.timestampSeconds = timestampSeconds;

        if (monoBlock.getNumSamples() <= 0 || monoBlock.getNumChannels() <= 0)
        {
            state.signalPresent = false;
            state.stable = false;
            return result;
        }

        noiseGate.processMonoBlock (monoBlock);
        const bool hasSignal = noiseGate.hasSignal();

        if (! hasSignal)
        {
            history.clear();
            chroma.fill (0.0f);
            state.signalPresent = false;
            state.stable = false;
            state.valid = false;
            state.confidence *= 0.86f;
            result.tone = stableTone;
            result.tone.valid = false;
            result.tone.confidence = state.confidence;
            return result;
        }

        const auto* samples = monoBlock.getReadPointer (0);
        const int numSamples = monoBlock.getNumSamples();

        auto pitch = pitchDetector.detect (samples, numSamples, sampleRate);
        const float bpm = bpmDetector.processBlock (samples, numSamples, sampleRate);

        ToneResult raw;
        if (pitch.valid)
        {
            updateChromaFromPitch (pitch.hz, pitch.confidence);

            raw = keyDetector.detectFromChroma (chroma, bpm);
            raw.pitchHz = pitch.hz;

            if (! raw.valid || raw.confidence < 0.12f)
                raw = keyDetector.detectFromPitch (pitch.hz, pitch.confidence, bpm);

            if (raw.valid)
            {
                raw.pitchHz = pitch.hz;
                raw.confidence = juce::jlimit (0.0f, 1.0f, raw.confidence * 0.65f + pitch.confidence * 0.35f);
                pushCandidate (raw, timestampSeconds);
            }
        }

        const auto stable = updateStableTone (timestampSeconds);
        result.tone = stable;
        return result;
    }

    RealtimeToneState ToneEngine::getToneState() const
    {
        return state;
    }

    int ToneEngine::keyIndexFromName (const juce::String& keyName)
    {
        static const char* names[12] = { "C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B" };
        for (int i = 0; i < 12; ++i)
            if (keyName.equalsIgnoreCase (names[i]))
                return i;
        return -1;
    }

    juce::String ToneEngine::scaleName (bool minor)
    {
        return minor ? "Minor" : "Major";
    }

    ToneResult ToneEngine::makeToneResult (int keyIndex, bool minor, float confidence, float pitchHz, double)
    {
        ToneResult tone;
        tone.keyName = midiNoteName (keyIndex);
        tone.scaleName = scaleName (minor);
        tone.camelot = camelotName (keyIndex, minor);
        tone.confidence = juce::jlimit (0.0f, 1.0f, confidence);
        tone.pitchHz = pitchHz;
        tone.bpm = 0.0f;
        tone.valid = true;
        return tone;
    }

    void ToneEngine::updateChromaFromPitch (float hz, float amount)
    {
        if (hz <= 0.0f)
            return;

        const int midi = (int) std::round (69.0 + 12.0 * std::log2 ((double) hz / 440.0));
        const int pc = ((midi % 12) + 12) % 12;

        for (auto& v : chroma)
            v *= 0.992f;

        chroma[(size_t) pc] += juce::jlimit (0.0f, 1.0f, amount);
    }

    void ToneEngine::pushCandidate (const ToneResult& raw, double timestampSeconds)
    {
        const int key = keyIndexFromName (raw.keyName);
        if (key < 0 || raw.confidence < minRawConfidence)
            return;

        const bool minor = raw.scaleName.equalsIgnoreCase ("Minor");
        history.push_back ({ key, minor, raw.confidence, raw.pitchHz, timestampSeconds });

        while ((int) history.size() > maxHistoryFrames)
            history.pop_front();

        while (! history.empty() && timestampSeconds - history.front().timeSeconds > maxHistorySeconds)
            history.pop_front();
    }

    ToneResult ToneEngine::updateStableTone (double timestampSeconds)
    {
        if (history.empty())
        {
            state.valid = false;
            state.stable = false;
            return stableTone;
        }

        std::array<int, 24> votes{};
        std::array<float, 24> confidenceSum{};
        std::array<float, 24> pitchSum{};

        for (const auto& item : history)
        {
            const int slot = item.keyIndex + (item.minor ? 12 : 0);
            votes[(size_t) slot] += 1;
            confidenceSum[(size_t) slot] += item.confidence;
            pitchSum[(size_t) slot] += item.pitchHz;
        }

        int bestSlot = -1;
        int bestVotes = 0;
        for (int i = 0; i < 24; ++i)
        {
            if (votes[(size_t) i] > bestVotes)
            {
                bestVotes = votes[(size_t) i];
                bestSlot = i;
            }
        }

        if (bestSlot < 0 || bestVotes <= 0)
            return stableTone;

        const float share = (float) bestVotes / (float) juce::jmax (1, (int) history.size());
        const float avgConfidence = confidenceSum[(size_t) bestSlot] / (float) bestVotes;
        const float avgPitch = pitchSum[(size_t) bestSlot] / (float) bestVotes;

        const bool stable = bestVotes >= minStableVotes &&
                            share >= minStableShare &&
                            avgConfidence >= minStableConfidence;

        if (stable)
        {
            const int key = bestSlot % 12;
            const bool minor = bestSlot >= 12;
            stableTone = makeToneResult (key, minor, avgConfidence * share, avgPitch, timestampSeconds);
            state = RealtimeToneState::fromToneResult (stableTone, true, true, timestampSeconds);
        }
        else
        {
            state.signalPresent = true;
            state.stable = false;
            state.valid = stableTone.valid;
            state.confidence = juce::jlimit (0.0f, 1.0f, avgConfidence * share);
            state.detectedTime = timestampSeconds;
        }

        return stableTone;
    }
}
