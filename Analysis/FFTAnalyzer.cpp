#include "FFTAnalyzer.h"

namespace AutoMenu
{
    FFTAnalyzer::FFTAnalyzer()
        : fft (fftOrder),
          window (fftSize, juce::dsp::WindowingFunction<float>::hann)
    {
    }

    void FFTAnalyzer::prepare (double sampleRateToUse)
    {
        sampleRate = sampleRateToUse > 0.0 ? sampleRateToUse : 48000.0;
        juce::ignoreUnused (sampleRate);
    }

    SpectrumResult FFTAnalyzer::analyze (const float* samples, int numSamples)
    {
        SpectrumResult result;
        fftData.fill (0.0f);

        if (samples == nullptr || numSamples <= 0)
            return result;

        const int copyCount = juce::jmin (numSamples, fftSize);
        const int offset = juce::jmax (0, numSamples - copyCount);

        for (int i = 0; i < copyCount; ++i)
            fftData[(size_t) i] = samples[offset + i];

        window.multiplyWithWindowingTable (fftData.data(), fftSize);
        fft.performFrequencyOnlyForwardTransform (fftData.data());

        for (int i = 0; i < SpectrumResult::numBins; ++i)
        {
            const int start = 1 + i * ((fftSize / 2 - 1) / SpectrumResult::numBins);
            const int end = 1 + (i + 1) * ((fftSize / 2 - 1) / SpectrumResult::numBins);
            float peak = 0.0f;

            for (int bin = start; bin < end; ++bin)
                peak = juce::jmax (peak, fftData[(size_t) bin]);

            result.bins[(size_t) i] = juce::jlimit (0.0f, 1.0f, juce::Decibels::gainToDecibels (peak + 1.0e-8f, -80.0f) / 80.0f + 1.0f);
        }

        return result;
    }
}
