#pragma once

#include <juce_core/juce_core.h>
#include <array>

namespace AutoMenu
{
    struct AudioBlockInfo
    {
        double sampleRate = 48000.0;
        int numChannels = 0;
        int numSamples = 0;
    };

    struct ToneResult
    {
        juce::String keyName = "--";
        juce::String scaleName = "--";
        juce::String camelot = "--";
        float confidence = 0.0f;
        float pitchHz = 0.0f;
        float bpm = 0.0f;
        bool valid = false;
    };

    struct SpectrumResult
    {
        static constexpr int numBins = 64;
        std::array<float, numBins> bins{};
    };

    struct AnalysisResult
    {
        ToneResult tone;
        SpectrumResult spectrum;
        double timestampSeconds = 0.0;
    };

    inline juce::String midiNoteName (int note)
    {
        static const char* names[] = { "C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B" };
        return names[((note % 12) + 12) % 12];
    }

    inline juce::String camelotName (int root, bool minor)
    {
        // Index = semitone root C=0. Values follow common Camelot wheel.
        static const char* major[] = { "8B", "3B", "10B", "5B", "12B", "7B", "2B", "9B", "4B", "11B", "6B", "1B" };
        static const char* minorKeys[] = { "5A", "12A", "7A", "2A", "9A", "4A", "11A", "6A", "1A", "8A", "3A", "10A" };
        root = ((root % 12) + 12) % 12;
        return minor ? minorKeys[root] : major[root];
    }
}
