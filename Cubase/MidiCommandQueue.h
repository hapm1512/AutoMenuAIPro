#pragma once

#include <juce_core/juce_core.h>
#include <vector>

namespace AutoMenu
{
    enum class MidiCommandType
    {
        noteOn,
        noteOff,
        controlChange,
        programChange
    };

    struct MidiCommand
    {
        MidiCommandType type = MidiCommandType::controlChange;

        int channel = 1;
        int data1 = 0;
        int data2 = 0;
    };

    class MidiCommandQueue final
    {
    public:
        void push(const MidiCommand& command);
        std::vector<MidiCommand> popAll();
        void clear();
        bool isEmpty() const;

    private:
        mutable juce::CriticalSection lock;
        std::vector<MidiCommand> queue;
    };
}