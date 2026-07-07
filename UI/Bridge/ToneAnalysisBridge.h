#pragma once

#include <juce_events/juce_events.h>
#include "../../Core/CoreController.h"

namespace automenu::ui
{
    class ToneAnalysisBridge final : private juce::Timer
    {
    public:
        class Listener
        {
        public:
            virtual ~Listener() = default;
            virtual void toneAnalysisUpdated (const automenu::core::ToneAnalysisResult& result) = 0;
        };

        explicit ToneAnalysisBridge (automenu::core::CoreController& controller);
        ~ToneAnalysisBridge() override;

        void addListener (Listener* listener);
        void removeListener (Listener* listener);

        void start();
        void stop();

    private:
        void timerCallback() override;

        automenu::core::CoreController& core;
        juce::ListenerList<Listener> listeners;
    };
}
