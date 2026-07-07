#include "ToneAnalysisBridge.h"

namespace automenu::ui
{
    ToneAnalysisBridge::ToneAnalysisBridge (automenu::core::CoreController& controller)
        : core (controller)
    {
    }

    ToneAnalysisBridge::~ToneAnalysisBridge()
    {
        stop();
    }

    void ToneAnalysisBridge::addListener (Listener* listener)
    {
        listeners.add (listener);
    }

    void ToneAnalysisBridge::removeListener (Listener* listener)
    {
        listeners.remove (listener);
    }

    void ToneAnalysisBridge::start()
    {
        startTimerHz (12);
    }

    void ToneAnalysisBridge::stop()
    {
        stopTimer();
    }

    void ToneAnalysisBridge::timerCallback()
    {
        const auto result = core.getLatestResult();
        listeners.call ([&] (Listener& l) { l.toneAnalysisUpdated (result); });
    }
}
