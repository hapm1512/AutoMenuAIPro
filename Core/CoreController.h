#pragma once

#include <juce_core/juce_core.h>
#include "AnalysisTypes.h"
#include "../Audio/AudioEngine.h"
#include "../Analysis/AnalysisEngine.h"

namespace automenu::core
{
    class CoreController final
    {
    public:
        CoreController();
        ~CoreController();

        bool start();
        void stop();

        bool isRunning() const noexcept;
        ToneAnalysisResult getLatestResult() const;

        audio::AudioEngine& getAudioEngine() noexcept { return audioEngine; }
        analysis::AnalysisEngine& getAnalysisEngine() noexcept { return analysisEngine; }

    private:
        audio::AudioEngine audioEngine;
        analysis::AnalysisEngine analysisEngine;
    };
}
