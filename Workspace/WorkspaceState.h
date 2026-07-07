#pragma once

#include <juce_core/juce_core.h>

namespace AutoMenu
{
    class WorkspaceState final
    {
    public:
        WorkspaceState();

        void resetToDefaults();
        bool load();
        bool save() const;

        juce::String workspaceName = "Live Vocal";
        juce::String lastMidiOutput;
        juce::String lastAudioDevice;

        bool performanceMode = false;
        bool autoStartAudio = true;
        bool autoConnectMidi = true;
        bool restoreOnStartup = true;

        float guiScale = 0.85f;

        static juce::File getConfigFile();
    };
}
