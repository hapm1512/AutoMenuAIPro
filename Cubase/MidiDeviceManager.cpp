#include "MidiDeviceManager.h"

namespace AutoMenu
{
    MidiDeviceManager::MidiDeviceManager() = default;

    MidiDeviceManager::~MidiDeviceManager()
    {
        closeOutput();
    }

    juce::StringArray MidiDeviceManager::getMidiOutputNames() const
    {
        juce::StringArray names;

        auto devices = juce::MidiOutput::getAvailableDevices();

        for (const auto& device : devices)
            names.add(device.name);

        return names;
    }

    juce::StringArray MidiDeviceManager::getMidiInputNames() const
    {
        juce::StringArray names;

        auto devices = juce::MidiInput::getAvailableDevices();

        for (const auto& device : devices)
            names.add(device.name);

        return names;
    }

    bool MidiDeviceManager::openOutputByName(const juce::String& deviceName)
    {
        closeOutput();

        auto devices = juce::MidiOutput::getAvailableDevices();

        for (const auto& device : devices)
        {
            if (device.name == deviceName)
            {
                midiOutput = juce::MidiOutput::openDevice(device.identifier);

                if (midiOutput != nullptr)
                {
                    currentOutputName = device.name;
                    return true;
                }
            }
        }

        currentOutputName.clear();
        return false;
    }

    void MidiDeviceManager::closeOutput()
    {
        midiOutput.reset();
        currentOutputName.clear();
    }

    bool MidiDeviceManager::isOutputOpen() const
    {
        return midiOutput != nullptr;
    }

    juce::String MidiDeviceManager::getCurrentOutputName() const
    {
        return currentOutputName;
    }

    void MidiDeviceManager::sendNoteOn(int channel, int noteNumber, int velocity)
    {
        if (midiOutput == nullptr)
            return;

        midiOutput->sendMessageNow(
            juce::MidiMessage::noteOn(
                clampMidiChannel(channel),
                clamp7Bit(noteNumber),
                static_cast<juce::uint8>(clamp7Bit(velocity))
            )
        );
    }

    void MidiDeviceManager::sendNoteOff(int channel, int noteNumber)
    {
        if (midiOutput == nullptr)
            return;

        midiOutput->sendMessageNow(
            juce::MidiMessage::noteOff(
                clampMidiChannel(channel),
                clamp7Bit(noteNumber)
            )
        );
    }

    void MidiDeviceManager::sendControlChange(int channel, int controller, int value)
    {
        if (midiOutput == nullptr)
            return;

        midiOutput->sendMessageNow(
            juce::MidiMessage::controllerEvent(
                clampMidiChannel(channel),
                clamp7Bit(controller),
                clamp7Bit(value)
            )
        );
    }

    void MidiDeviceManager::sendProgramChange(int channel, int programNumber)
    {
        if (midiOutput == nullptr)
            return;

        midiOutput->sendMessageNow(
            juce::MidiMessage::programChange(
                clampMidiChannel(channel),
                clamp7Bit(programNumber)
            )
        );
    }

    int MidiDeviceManager::clampMidiChannel(int channel) const
    {
        return juce::jlimit(1, 16, channel);
    }

    int MidiDeviceManager::clamp7Bit(int value) const
    {
        return juce::jlimit(0, 127, value);
    }
}