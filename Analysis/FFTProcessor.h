#pragma once

#include <juce_dsp/juce_dsp.h>
#include <array>
#include <vector>

namespace automenu::analysis
{
    class FFTProcessor final
    {
    public:
        FFTProcessor();

        std::array<float, 12> buildChroma (const std::vector<float>& samples, double sampleRate);

    private:
        static constexpr int fftOrder = 12;
        static constexpr int fftSize = 1 << fftOrder;

        juce::dsp::FFT fft;
        juce::dsp::WindowingFunction<float> window;
        std::vector<float> fftData;
    };
}
