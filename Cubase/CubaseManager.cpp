#include "CubaseManager.h"

namespace AutoMenu
{
    CubaseManager::CubaseManager()
    {
        startTimerHz(60);
    }

    CubaseManager::~CubaseManager()
    {
        stopTimer();
        disconnect();
    }

    bool CubaseManager::connectToMidiOutput(const juce::String& outputName)
    {
        connected = midiDevice.openOutputByName(outputName);
        return connected;
    }

    void CubaseManager::disconnect()
    {
        commandQueue.clear();
        midiDevice.closeOutput();
        connected = false;
    }

    bool CubaseManager::isConnected() const
    {
        return connected && midiDevice.isOutputOpen();
    }

    juce::String CubaseManager::getConnectedOutputName() const
    {
        return midiDevice.getCurrentOutputName();
    }

    juce::StringArray CubaseManager::getAvailableMidiOutputs() const
    {
        return midiDevice.getMidiOutputNames();
    }

    void CubaseManager::sendCommand(const MidiCommand& command)
    {
        commandQueue.push(command);
    }

    void CubaseManager::sendControlChange(int channel, int controller, int value)
    {
        MidiCommand command;
        command.type = MidiCommandType::controlChange;
        command.channel = channel;
        command.data1 = controller;
        command.data2 = value;

        sendCommand(command);
    }

    void CubaseManager::sendNotePulse(int channel, int noteNumber, int velocity)
    {
        MidiCommand on;
        on.type = MidiCommandType::noteOn;
        on.channel = channel;
        on.data1 = noteNumber;
        on.data2 = velocity;

        MidiCommand off;
        off.type = MidiCommandType::noteOff;
        off.channel = channel;
        off.data1 = noteNumber;
        off.data2 = 0;

        sendCommand(on);
        sendCommand(off);
    }

    void CubaseManager::triggerMacro(int macroIndex)
    {
        switch (macroIndex)
        {
            case 0:  sendControlChange(1, 20, 127); break; // AI
            case 1:  sendControlChange(1, 21, 127); break; // Delay
            case 2:  sendControlChange(1, 22, 127); break; // Fix
            case 3:  sendControlChange(1, 23, 127); break; // Save
            case 4:  sendControlChange(1, 24, 127); break; // Lofi
            case 5:  sendControlChange(1, 25, 127); break; // Off
            case 6:  sendControlChange(1, 26, 127); break; // Vang
            case 7:  sendControlChange(1, 27, 127); break; // Tune
            case 8:  sendControlChange(1, 28, 127); break; // Nhac
            case 9:  sendControlChange(1, 29, 127); break; // Mic
            case 10: sendControlChange(1, 30, 127); break; // Remix
            case 11: sendControlChange(1, 31, 127); break; // Dan Ca
            default: break;
        }
    }

    void CubaseManager::timerCallback()
    {
        processQueue();
    }

    void CubaseManager::processQueue()
    {
        if (!isConnected())
            return;

        auto commands = commandQueue.popAll();

        for (const auto& command : commands)
        {
            switch (command.type)
            {
                case MidiCommandType::noteOn:
                    midiDevice.sendNoteOn(command.channel, command.data1, command.data2);
                    break;

                case MidiCommandType::noteOff:
                    midiDevice.sendNoteOff(command.channel, command.data1);
                    break;

                case MidiCommandType::controlChange:
                    midiDevice.sendControlChange(command.channel, command.data1, command.data2);
                    break;

                case MidiCommandType::programChange:
                    midiDevice.sendProgramChange(command.channel, command.data1);
                    break;
            }
        }
    }
}