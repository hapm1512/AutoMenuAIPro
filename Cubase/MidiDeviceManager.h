#pragma once

#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_core/juce_core.h>

namespace AutoMenu
{
    class MidiDeviceManager final
    {
    public:
        MidiDeviceManager();
        ~MidiDeviceManager();

        juce::StringArray getMidiOutputNames() const;
        juce::StringArray getMidiInputNames() const;

        bool openOutputByName(const juce::String& deviceName);
        void closeOutput();

        bool isOutputOpen() const;
        juce::String getCurrentOutputName() const;

        void sendNoteOn(int channel, int noteNumber, int velocity);
        void sendNoteOff(int channel, int noteNumber);
        void sendControlChange(int channel, int controller, int value);
        void sendProgramChange(int channel, int programNumber);

    private:
        std::unique_ptr<juce::MidiOutput> midiOutput;
        juce::String currentOutputName;

        int clampMidiChannel(int channel) const;
        int clamp7Bit(int value) const;
    };
}