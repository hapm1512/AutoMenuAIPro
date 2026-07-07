#include "NoiseGate.h"
#include <cmath>

namespace AutoMenu
{
    void NoiseGate::prepare (double sampleRateToUse)
    {
        sampleRate = sampleRateToUse > 0.0 ? sampleRateToUse : 48000.0;
        reset();
    }

    void NoiseGate::reset()
    {
        envelope = 0.0f;
        lastRmsDb = -100.0f;
        signalActive = false;
    }

    void NoiseGate::processMonoBlock (juce::AudioBuffer<float>& buffer)
    {
        if (buffer.getNumChannels() <= 0 || buffer.getNumSamples() <= 0)
            return;

        auto* data = buffer.getWritePointer (0);
        const int n = buffer.getNumSamples();

        double mean = 0.0;
        for (int i = 0; i < n; ++i)
            mean += data[i];
        mean /= (double) n;

        double power = 0.0;
        for (int i = 0; i < n; ++i)
        {
            data[i] -= (float) mean;
            power += (double) data[i] * (double) data[i];
        }

        const float rms = (float) std::sqrt (power / (double) n);
        lastRmsDb = juce::Decibels::gainToDecibels (rms + 1.0e-9f, -100.0f);

        const float target = lastRmsDb >= thresholdDb ? 1.0f : 0.0f;
        const float attackMs = 8.0f;
        const float releaseMs = 220.0f;
        const float coeff = target > envelope
            ? std::exp (-1.0f / (float) (0.001 * attackMs * sampleRate))
            : std::exp (-1.0f / (float) (0.001 * releaseMs * sampleRate));

        envelope = target + coeff * (envelope - target);
        signalActive = envelope > 0.20f;

        if (! signalActive)
        {
            juce::FloatVectorOperations::multiply (data, 0.15f, n);
            return;
        }

        const float gain = juce::jlimit (0.15f, 1.0f, envelope);
        juce::FloatVectorOperations::multiply (data, gain, n);
    }
}
