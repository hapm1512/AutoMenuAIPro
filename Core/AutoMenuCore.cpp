#include "AutoMenuCore.h"

namespace AutoMenu
{
    AutoMenuCore::AutoMenuCore()
    {
        audioEngine.setAnalysisManager (&analysisManager);
    }

    AutoMenuCore::~AutoMenuCore()
    {
        stopAudio();
    }

    bool AutoMenuCore::startAudio()
    {
        return audioEngine.initialise (2, 0);
    }

    void AutoMenuCore::stopAudio()
    {
        audioEngine.shutdown();
    }

    AnalysisResult AutoMenuCore::getAnalysisResult() const
    {
        return analysisManager.getLatestResult();
    }
}
