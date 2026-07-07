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
        pendingMono.setSize (1, FFTAnalyzer::fftSize * 2);
        pendingMono.clear();
        pendingWrite = 0;
        hasEnoughSamples = false;
        timestamp = 0.0;
        chroma.fill (0.0f);
        latest = AnalysisResult{};
        fftAnalyzer.prepare (sampleRate);
        bpmDetector.reset();
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

    void AnalysisManager::timerCallback()
    {
        analyzePendingBuffer();
    }

    void AnalysisManager::analyzePendingBuffer()
    {
        juce::AudioBuffer<float> snapshot;
        double sr = 48000.0;
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

            sr = sampleRate;
            ts = timestamp;
        }

        auto spectrum = fftAnalyzer.analyze (snapshot.getReadPointer (0), snapshot.getNumSamples());
        auto pitch = pitchDetector.detect (snapshot.getReadPointer (0), snapshot.getNumSamples(), sr);
        const float bpm = bpmDetector.processBlock (snapshot.getReadPointer (0), snapshot.getNumSamples(), sr);

        ToneResult tone;
        if (pitch.valid)
        {
            updateChromaFromPitch (pitch.hz, pitch.confidence);
            tone = keyDetector.detectFromChroma (chroma, bpm);
            tone.pitchHz = pitch.hz;

            if (! tone.valid || tone.confidence < 0.12f)
                tone = keyDetector.detectFromPitch (pitch.hz, pitch.confidence, bpm);
        }

        AnalysisResult result;
        result.spectrum = spectrum;
        result.tone = tone;
        result.timestampSeconds = ts;

        const juce::ScopedLock sl (lock);
        latest = result;
    }

    void AnalysisManager::updateChromaFromPitch (float hz, float amount)
    {
        if (hz <= 0.0f)
            return;

        const int midi = (int) std::round (69.0 + 12.0 * std::log2 ((double) hz / 440.0));
        const int pc = ((midi % 12) + 12) % 12;

        for (auto& v : chroma)
            v *= 0.992f;

        chroma[(size_t) pc] += juce::jlimit (0.0f, 1.0f, amount);
    }
}
