#include "AnalysisManager.h"
#include <cmath>

namespace AutoMenu
{
    AnalysisManager::AnalysisManager()
    {
        prepare (48000.0, 512);
        startTimerHz (20);
    }

    AnalysisManager::~AnalysisManager()
    {
        stopTimer();
    }

    void AnalysisManager::prepare (double sampleRateToUse, int blockSizeToUse)
    {
        const juce::ScopedLock sl (lock);

        sampleRate = sampleRateToUse > 0.0 ? sampleRateToUse : 48000.0;
        blockSize = juce::jmax (64, blockSizeToUse);

        pendingMono.setSize (1, FFTAnalyzer::fftSize * 4);
        pendingMono.clear();
        pendingWrite = 0;
        hasEnoughSamples = false;
        timestamp = 0.0;

        latest = AnalysisResult{};
        latestTone = RealtimeToneState{};

        fftAnalyzer.prepare (sampleRate);
        toneEngine.prepare (sampleRate);
    }

    void AnalysisManager::reset()
    {
        prepare (sampleRate, blockSize);
    }

    void AnalysisManager::pushAudioBlock (const float* const* input, int channels, int samples, double sampleRateToUse)
    {
        if (input == nullptr || channels <= 0 || samples <= 0)
            return;

        if (std::abs (sampleRateToUse - sampleRate) > 1.0)
            prepare (sampleRateToUse, samples);

        const juce::ScopedLock sl (lock);
        auto* dst = pendingMono.getWritePointer (0);
        const int capacity = pendingMono.getNumSamples();

        for (int i = 0; i < samples; ++i)
        {
            float mixed = 0.0f;
            for (int ch = 0; ch < channels; ++ch)
                mixed += input[ch] != nullptr ? input[ch][i] : 0.0f;

            dst[pendingWrite] = mixed / (float) channels;
            pendingWrite = (pendingWrite + 1) % capacity;

            if (pendingWrite == 0)
                hasEnoughSamples = true;
        }

        timestamp += (double) samples / sampleRate;
    }

    AnalysisResult AnalysisManager::getLatestResult() const
    {
        const juce::ScopedLock sl (lock);
        return latest;
    }

    RealtimeToneState AnalysisManager::getLatestToneState() const
    {
        const juce::ScopedLock sl (lock);
        return latestTone;
    }

    void AnalysisManager::timerCallback()
    {
        analyzePendingBuffer();
    }

    void AnalysisManager::analyzePendingBuffer()
    {
        juce::AudioBuffer<float> snapshot;
        double ts = 0.0;

        {
            const juce::ScopedLock sl (lock);

            if (! hasEnoughSamples && pendingWrite < FFTAnalyzer::fftSize)
                return;

            snapshot.setSize (1, FFTAnalyzer::fftSize);
            auto* out = snapshot.getWritePointer (0);
            const auto* src = pendingMono.getReadPointer (0);
            const int capacity = pendingMono.getNumSamples();

            int start = pendingWrite - FFTAnalyzer::fftSize;
            while (start < 0)
                start += capacity;

            for (int i = 0; i < FFTAnalyzer::fftSize; ++i)
                out[i] = src[(start + i) % capacity];

            ts = timestamp;
        }

        auto spectrumSnapshot = snapshot;
        auto result = toneEngine.processBlock (snapshot, ts);
        result.spectrum = fftAnalyzer.analyze (spectrumSnapshot.getReadPointer (0), spectrumSnapshot.getNumSamples());
        result.timestampSeconds = ts;

        const auto toneState = toneEngine.getToneState();

        const juce::ScopedLock sl (lock);
        latest = result;
        latestTone = toneState;
    }
}
