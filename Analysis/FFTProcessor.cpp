#include "FFTProcessor.h"
#include "KeyDetector.h"
#include <cmath>

namespace automenu::analysis
{
    FFTProcessor::FFTProcessor()
        : fft (fftOrder),
          window (fftSize, juce::dsp::WindowingFunction<float>::hann),
          fftData ((size_t) fftSize * 2, 0.0f)
    {
    }

    std::array<float, 12> FFTProcessor::buildChroma (const std::vector<float>& samples, double sampleRate)
    {
        std::array<float, 12> chroma {};

        if (samples.size() < (size_t) fftSize || sampleRate <= 0.0)
            return chroma;

        std::fill (fftData.begin(), fftData.end(), 0.0f);

        const auto start = samples.size() - (size_t) fftSize;
        for (int i = 0; i < fftSize; ++i)
            fftData[(size_t) i] = samples[start + (size_t) i];

        window.multiplyWithWindowingTable (fftData.data(), fftSize);
        fft.performFrequencyOnlyForwardTransform (fftData.data());

        const int minBin = juce::jmax (1, (int) std::floor (55.0 * fftSize / sampleRate));
        const int maxBin = juce::jmin (fftSize / 2, (int) std::ceil (2000.0 * fftSize / sampleRate));

        for (int bin = minBin; bin <= maxBin; ++bin)
        {
            const auto frequency = (float) ((double) bin * sampleRate / fftSize);
            const auto pitchClass = KeyDetector::pitchClassFromFrequency (frequency);

            if (pitchClass >= 0)
                chroma[(size_t) pitchClass] += std::sqrt (juce::jmax (fftData[(size_t) bin], 0.0f));
        }

        float maxValue = 0.0f;
        for (auto v : chroma)
            maxValue = juce::jmax (maxValue, v);

        if (maxValue > 0.0f)
            for (auto& v : chroma)
                v /= maxValue;

        return chroma;
    }
}
