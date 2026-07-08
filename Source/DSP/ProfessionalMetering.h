#pragma once
#include <JuceHeader.h>

namespace VocalDSP
{
    struct MeteringFrame
    {
        float inputPeak = 0.0f;
        float outputPeak = 0.0f;
        float inputRms = 0.0f;
        float outputRms = 0.0f;
        float truePeak = 0.0f;
        float gainReduction = 0.0f;
        float lufsMomentary = -70.0f;
        float lufsShortTerm = -70.0f;
        float lufsIntegrated = -70.0f;
        float correlation = 1.0f;
    };

    inline float ampToDb (float amp) noexcept
    {
        return juce::Decibels::gainToDecibels (juce::jmax (amp, 1.0e-9f), -90.0f);
    }

    inline float dbToMeter (float db) noexcept
    {
        return juce::jlimit (0.0f, 1.0f, (db + 60.0f) / 60.0f);
    }

    class ProfessionalMetering final
    {
    public:
        void prepare (double newSampleRate) noexcept
        {
            sampleRate = juce::jmax (1.0, newSampleRate);
            reset();
        }

        void reset() noexcept
        {
            shortTermLufs = -70.0f;
            integratedEnergy = 0.0;
            integratedSamples = 0.0;
        }

        MeteringFrame analyse (float inputPeak, float inputRms,
                               const juce::AudioBuffer<float>& outBuffer,
                               float grDb) noexcept
        {
            MeteringFrame f;

            const auto outStats = getStats (outBuffer);

            f.inputPeak = inputPeak;
            f.inputRms = inputRms;
            f.outputPeak = outStats.peak;
            f.outputRms = outStats.rms;
            f.truePeak = estimateTruePeak (outBuffer);
            f.gainReduction = juce::jlimit (0.0f, 36.0f, grDb);
            f.correlation = estimateCorrelation (outBuffer);

            const float momentary = estimateLufsFromRms (outStats.rms);
            f.lufsMomentary = momentary;

            const float blockSeconds = (float) outBuffer.getNumSamples() / (float) sampleRate;
            const float shortCoeff = std::exp (-blockSeconds / 3.0f);
            shortTermLufs = shortTermLufs * shortCoeff + momentary * (1.0f - shortCoeff);
            f.lufsShortTerm = juce::jlimit (-70.0f, 6.0f, shortTermLufs);

            const double blockEnergy = (double) outStats.rms * (double) outStats.rms;
            if (momentary > -70.0f)
            {
                integratedEnergy += blockEnergy * (double) outBuffer.getNumSamples();
                integratedSamples += (double) outBuffer.getNumSamples();
            }

            if (integratedSamples > 0.0)
            {
                const auto integratedRms = (float) std::sqrt (integratedEnergy / integratedSamples);
                f.lufsIntegrated = estimateLufsFromRms (integratedRms);
            }

            return f;
        }


    private:
        struct Stats { float peak = 0.0f; float rms = 0.0f; };

        static Stats getStats (const juce::AudioBuffer<float>& buffer) noexcept
        {
            double sum = 0.0;
            float peak = 0.0f;
            const int chans = buffer.getNumChannels();
            const int samples = buffer.getNumSamples();

            if (chans <= 0 || samples <= 0)
                return {};

            for (int ch = 0; ch < chans; ++ch)
            {
                const auto* data = buffer.getReadPointer (ch);
                for (int i = 0; i < samples; ++i)
                {
                    const float x = std::isfinite (data[i]) ? data[i] : 0.0f;
                    peak = juce::jmax (peak, std::abs (x));
                    sum += (double) x * (double) x;
                }
            }

            const double denom = (double) chans * (double) samples;
            return { juce::jlimit (0.0f, 8.0f, peak),
                     denom > 0.0 ? juce::jlimit (0.0f, 8.0f, (float) std::sqrt (sum / denom)) : 0.0f };
        }

        static float estimateLufsFromRms (float rms) noexcept
        {
            // Lightweight BS.1770-style display approximation.
            return juce::jlimit (-70.0f, 6.0f, ampToDb (rms) - 0.691f);
        }

        static float estimateTruePeak (const juce::AudioBuffer<float>& buffer) noexcept
        {
            float peak = 0.0f;
            const int chans = buffer.getNumChannels();
            const int samples = buffer.getNumSamples();

            for (int ch = 0; ch < chans; ++ch)
            {
                const auto* data = buffer.getReadPointer (ch);
                for (int i = 0; i < samples; ++i)
                {
                    peak = juce::jmax (peak, std::abs (data[i]));
                    if (i > 0)
                    {
                        const float mid = 0.5f * (data[i - 1] + data[i]);
                        peak = juce::jmax (peak, std::abs (mid));
                    }
                }
            }

            return juce::jlimit (0.0f, 8.0f, peak);
        }

        static float estimateCorrelation (const juce::AudioBuffer<float>& buffer) noexcept
        {
            if (buffer.getNumChannels() < 2 || buffer.getNumSamples() <= 0)
                return 1.0f;

            const auto* l = buffer.getReadPointer (0);
            const auto* r = buffer.getReadPointer (1);
            double lr = 0.0, ll = 0.0, rr = 0.0;

            for (int i = 0; i < buffer.getNumSamples(); ++i)
            {
                lr += (double) l[i] * (double) r[i];
                ll += (double) l[i] * (double) l[i];
                rr += (double) r[i] * (double) r[i];
            }

            const double denom = std::sqrt (ll * rr);
            if (denom <= 1.0e-12)
                return 1.0f;

            return juce::jlimit (-1.0f, 1.0f, (float) (lr / denom));
        }

        double sampleRate = 44100.0;
        float shortTermLufs = -70.0f;
        double integratedEnergy = 0.0;
        double integratedSamples = 0.0;
    };
}
