#pragma once

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>

#include "AutomationTypes.h"
#include "../Cubase/CubaseManager.h"

namespace AutoMenu
{
    class AutomationEngine final : private juce::Timer
    {
    public:
        explicit AutomationEngine (CubaseManager& cubase);
        ~AutomationEngine() override;

        void playSequence (const AutomationSequence& sequence);
        void stop();

        bool isRunning() const;
        juce::String getActiveSequenceName() const;

    private:
        void timerCallback() override;
        void executeStep (const AutomationStep& step);

        CubaseManager& cubaseManager;

        mutable juce::CriticalSection lock;
        AutomationSequence activeSequence;
        size_t nextStepIndex = 0;
        juce::uint32 nextStepTimeMs = 0;
        bool running = false;
    };
}
