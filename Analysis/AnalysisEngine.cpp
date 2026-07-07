#include "AnalysisEngine.h"

namespace automenu::analysis
{
    AnalysisEngine::AnalysisEngine (automenu::audio::AudioRingBuffer& sourceBuffer)
        : ringBuffer (sourceBuffer)
    {
    }

    AnalysisEngine::~AnalysisEngine()
    {
        stop();
    }

    void AnalysisEngine::start()
    {
        startTimerHz (10);
    }

    void AnalysisEngine::stop()
    {
        stopTimer();
    }

    automenu::core::ToneAnalysisResult AnalysisEngine::getLatestResult() const
    {
        const std::lock_guard<std::mutex> lock (resultMutex);
        return latestResult;
    }

    void AnalysisEngine::timerCallback()
    {
        analyseBlock();
    }

    void AnalysisEngine::analyseBlock()
    {
        constexpr int samplesToRead = 48000 * 3;
        ringBuffer.readLatest (analysisBlock, samplesToRead);

        if (analysisBlock.empty())
            return;

        const auto pitch = pitchDetector.detect (analysisBlock, sampleRate);
        const auto bpm = bpmDetector.detect (analysisBlock, sampleRate);
        const auto chroma = fftProcessor.buildChroma (analysisBlock, sampleRate);
        auto result = keyDetector.detect (chroma, pitch.frequencyHz, pitch.confidence, bpm);

        if (! pitch.valid)
        {
            result.confidence *= 0.65f;
            result.hasSignal = result.confidence > 0.10f;
        }

        const std::lock_guard<std::mutex> lock (resultMutex);
        latestResult = result;
    }
}
