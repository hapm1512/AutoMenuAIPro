#pragma once

#include "../Audio/AudioEngine.h"
#include "../Analysis/AnalysisManager.h"

namespace AutoMenu
{
    class AutoMenuCore final
    {
    public:
        AutoMenuCore();
        ~AutoMenuCore();

        bool startAudio();
        void stopAudio();

        AnalysisResult getAnalysisResult() const;
        AudioEngine& getAudioEngine() noexcept { return audioEngine; }
        AnalysisManager& getAnalysisManager() noexcept { return analysisManager; }

    private:
        AudioEngine audioEngine;
        AnalysisManager analysisManager;
    };
}
