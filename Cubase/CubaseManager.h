#pragma once

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>

#include "MidiDeviceManager.h"
#include "MidiCommandQueue.h"

namespace AutoMenu
{
    class CubaseManager final : private juce::Timer
    {
    public:
        CubaseManager();
        ~CubaseManager() override;

        bool connectToMidiOutput(const juce::String& outputName);
        void disconnect();

        bool isConnected() const;
        juce::String getConnectedOutputName() const;

        juce::StringArray getAvailableMidiOutputs() const;

        void sendCommand(const MidiCommand& command);
        void sendControlChange(int channel, int controller, int value);
        void sendNotePulse(int channel, int noteNumber, int velocity);

        void triggerMacro(int macroIndex);

    private:
        void timerCallback() override;
        void processQueue();

        MidiDeviceManager midiDevice;
        MidiCommandQueue commandQueue;

        bool connected = false;
    };
}