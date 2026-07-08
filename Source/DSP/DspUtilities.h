#pragma once
#include <JuceHeader.h>

namespace VocalDSP
{
    // Audio-thread safe utility helpers for Epic 4A.
    struct ProcessingStats
    {
        float peak = 0.0f;
        float rms = 0.0f;
    };

    inline float sanitizeSample (float x) noexcept
    {
        if (! std::isfinite (x))
            return 0.0f;

        // Kill denormal/subnormal values before they burn CPU.
        if (std::abs (x) < 1.0e-20f)
            return 0.0f;

        return x;
    }

    inline void sanitizeBuffer (juce::AudioBuffer<float>& buffer) noexcept
    {
        const int numChannels = buffer.getNumChannels();
        const int numSamples = buffer.getNumSamples();

        for (int ch = 0; ch < numChannels; ++ch)
        {
            auto* data = buffer.getWritePointer (ch);

            for (int i = 0; i < numSamples; ++i)
                data[i] = sanitizeSample (data[i]);
        }
    }

    inline ProcessingStats getStats (const juce::AudioBuffer<float>& buffer) noexcept
    {
        const int numChannels = buffer.getNumChannels();
        const int numSamples = buffer.getNumSamples();

        if (numChannels <= 0 || numSamples <= 0)
            return {};

        double sumSquares = 0.0;
        float peak = 0.0f;

        for (int ch = 0; ch < numChannels; ++ch)
        {
            const auto* data = buffer.getReadPointer (ch);

            for (int i = 0; i < numSamples; ++i)
            {
                const float x = sanitizeSample (data[i]);
                const float ax = std::abs (x);
                peak = juce::jmax (peak, ax);
                sumSquares += (double) x * (double) x;
            }
        }

        const double denom = (double) numChannels * (double) numSamples;
        const float rms = denom > 0.0 ? (float) std::sqrt (sumSquares / denom) : 0.0f;

        return { juce::jlimit (0.0f, 8.0f, peak), juce::jlimit (0.0f, 8.0f, rms) };
    }

    inline void clearUnusedOutputChannels (juce::AudioBuffer<float>& buffer, int activeChannels) noexcept
    {
        const int totalChannels = buffer.getNumChannels();
        const int start = juce::jlimit (0, totalChannels, activeChannels);

        for (int ch = start; ch < totalChannels; ++ch)
            buffer.clear (ch, 0, buffer.getNumSamples());
    }
}
