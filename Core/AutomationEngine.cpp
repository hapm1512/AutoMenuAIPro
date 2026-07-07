#include "AutomationEngine.h"

namespace AutoMenu
{
    AutomationEngine::AutomationEngine (CubaseManager& cubase)
        : cubaseManager (cubase)
    {
        startTimerHz (200);
    }

    AutomationEngine::~AutomationEngine()
    {
        stopTimer();
        stop();
    }

    void AutomationEngine::playSequence (const AutomationSequence& sequence)
    {
        const juce::ScopedLock sl (lock);

        activeSequence = sequence;
        nextStepIndex = 0;
        nextStepTimeMs = juce::Time::getMillisecondCounter();
        running = ! activeSequence.steps.empty();
    }

    void AutomationEngine::stop()
    {
        const juce::ScopedLock sl (lock);

        activeSequence.steps.clear();
        activeSequence.name.clear();
        nextStepIndex = 0;
        nextStepTimeMs = 0;
        running = false;
    }

    bool AutomationEngine::isRunning() const
    {
        const juce::ScopedLock sl (lock);
        return running;
    }

    juce::String AutomationEngine::getActiveSequenceName() const
    {
        const juce::ScopedLock sl (lock);
        return activeSequence.name;
    }

    void AutomationEngine::timerCallback()
    {
        AutomationStep step;
        bool shouldExecute = false;

        {
            const juce::ScopedLock sl (lock);

            if (! running)
                return;

            const auto now = juce::Time::getMillisecondCounter();

            if (now < nextStepTimeMs)
                return;

            if (nextStepIndex >= activeSequence.steps.size())
            {
                running = false;
                return;
            }

            step = activeSequence.steps[nextStepIndex++];
            shouldExecute = true;

            const auto delay = juce::jmax (0, step.delayMs);
            nextStepTimeMs = now + (juce::uint32) delay;

            if (nextStepIndex >= activeSequence.steps.size() && delay == 0)
                running = false;
        }

        if (shouldExecute)
            executeStep (step);
    }

    void AutomationEngine::executeStep (const AutomationStep& step)
    {
        switch (step.type)
        {
            case AutomationActionType::midiControlChange:
                cubaseManager.sendControlChange (step.channel, step.data1, step.data2);
                break;

            case AutomationActionType::midiProgramChange:
            {
                MidiCommand command;
                command.type = MidiCommandType::programChange;
                command.channel = step.channel;
                command.data1 = step.data1;
                command.data2 = 0;
                cubaseManager.sendCommand (command);
                break;
            }

            case AutomationActionType::midiNotePulse:
                cubaseManager.sendNotePulse (step.channel, step.data1, step.data2);
                break;

            case AutomationActionType::delay:
                break;
        }
    }
}
