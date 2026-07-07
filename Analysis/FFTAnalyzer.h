#pragma once

#include <juce_dsp/juce_dsp.h>
#include "../Core/AnalysisTypes.h"

namespace AutoMenu
{
    class FFTAnalyzer final
    {
    public:
        static constexpr int fftOrder = 11;
        static constexpr int fftSize = 1 << fftOrder;

        FFTAnalyzer();

        void prepare (double sampleRateToUse);
        SpectrumResult analyze (const float* samples, int numSamples);

    private:
        double sampleRate = 48000.0;
        juce::dsp::FFT fft;
        juce::dsp::WindowingFunction<float> window;
        std::array<float, fftSize * 2> fftData{};
    };
}
