#include "CoreController.h"

namespace automenu::core
{
    CoreController::CoreController()
        : analysisEngine (audioEngine.getRingBuffer())
    {
    }

    CoreController::~CoreController()
    {
        stop();
    }

    bool CoreController::start()
    {
        if (! audioEngine.start())
            return false;

        analysisEngine.start();
        return true;
    }

    void CoreController::stop()
    {
        analysisEngine.stop();
        audioEngine.stop();
    }

    bool CoreController::isRunning() const noexcept
    {
        return audioEngine.isRunning();
    }

    ToneAnalysisResult CoreController::getLatestResult() const
    {
        return analysisEngine.getLatestResult();
    }
}
