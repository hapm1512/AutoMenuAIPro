#pragma once
#include <JuceHeader.h>

namespace ValidationReport
{
    struct RuntimeSnapshot
    {
        double sampleRate = 0.0;
        int blockSize = 0;
        int channels = 0;
        float inputPeak = 0.0f;
        float outputPeak = 0.0f;
        float outputRms = 0.0f;
        float truePeak = 0.0f;
        float gainReduction = 0.0f;
        float lufsMomentary = -100.0f;
        float lufsShortTerm = -100.0f;
        float lufsIntegrated = -100.0f;
        float stereoCorrelation = 0.0f;
    };

    inline bool isFinite (float value) noexcept
    {
        return std::isfinite (value);
    }

    inline bool isSnapshotSafe (const RuntimeSnapshot& s) noexcept
    {
        return s.sampleRate > 0.0
            && s.blockSize > 0
            && s.channels > 0
            && isFinite (s.inputPeak)
            && isFinite (s.outputPeak)
            && isFinite (s.outputRms)
            && isFinite (s.truePeak)
            && isFinite (s.gainReduction)
            && isFinite (s.lufsMomentary)
            && isFinite (s.lufsShortTerm)
            && isFinite (s.lufsIntegrated)
            && isFinite (s.stereoCorrelation);
    }

    inline juce::String createTextReport (const RuntimeSnapshot& s)
    {
        juce::String r;
        r << "Vocal Suite Ultra Pro - Runtime Validation\n";
        r << "==========================================\n";
        r << "Sample Rate: " << s.sampleRate << " Hz\n";
        r << "Block Size: " << s.blockSize << " samples\n";
        r << "Channels: " << s.channels << "\n";
        r << "Input Peak: " << s.inputPeak << "\n";
        r << "Output Peak: " << s.outputPeak << "\n";
        r << "Output RMS: " << s.outputRms << "\n";
        r << "True Peak: " << s.truePeak << "\n";
        r << "Gain Reduction: " << s.gainReduction << "\n";
        r << "LUFS M: " << s.lufsMomentary << "\n";
        r << "LUFS S: " << s.lufsShortTerm << "\n";
        r << "LUFS I: " << s.lufsIntegrated << "\n";
        r << "Stereo Correlation: " << s.stereoCorrelation << "\n";
        r << "Safe: " << (isSnapshotSafe (s) ? "YES" : "NO") << "\n";
        return r;
    }
}
