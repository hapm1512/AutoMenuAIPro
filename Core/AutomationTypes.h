#pragma once

#include <juce_core/juce_core.h>
#include <vector>

namespace AutoMenu
{
    enum class AutomationActionType
    {
        midiControlChange,
        midiProgramChange,
        midiNotePulse,
        delay
    };

    struct AutomationStep
    {
        AutomationActionType type = AutomationActionType::delay;

        int channel = 1;
        int data1 = 0;
        int data2 = 0;
        int delayMs = 0;

        static AutomationStep cc (int channelToUse,
                                  int controller,
                                  int value,
                                  int delayAfterMs = 0)
        {
            AutomationStep step;
            step.type = AutomationActionType::midiControlChange;
            step.channel = channelToUse;
            step.data1 = controller;
            step.data2 = value;
            step.delayMs = delayAfterMs;
            return step;
        }

        static AutomationStep program (int channelToUse,
                                       int programNumber,
                                       int delayAfterMs = 0)
        {
            AutomationStep step;
            step.type = AutomationActionType::midiProgramChange;
            step.channel = channelToUse;
            step.data1 = programNumber;
            step.delayMs = delayAfterMs;
            return step;
        }

        static AutomationStep notePulse (int channelToUse,
                                         int noteNumber,
                                         int velocity,
                                         int delayAfterMs = 0)
        {
            AutomationStep step;
            step.type = AutomationActionType::midiNotePulse;
            step.channel = channelToUse;
            step.data1 = noteNumber;
            step.data2 = velocity;
            step.delayMs = delayAfterMs;
            return step;
        }

        static AutomationStep wait (int milliseconds)
        {
            AutomationStep step;
            step.type = AutomationActionType::delay;
            step.delayMs = milliseconds;
            return step;
        }
    };

    struct AutomationSequence
    {
        juce::String name;
        std::vector<AutomationStep> steps;
    };
}
