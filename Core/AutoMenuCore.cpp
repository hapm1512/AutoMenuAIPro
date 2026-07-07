#include "AutoMenuCore.h"
#include "../Utils/AppLogger.h"

namespace AutoMenu
{
    AutoMenuCore::AutoMenuCore()
        : macroManager (cubaseManager)
    {
        audioEngine.setAnalysisManager (&analysisManager);
    }

    AutoMenuCore::~AutoMenuCore()
    {
        shutdownLiveSession();
    }

    bool AutoMenuCore::initialiseLiveSession()
    {
        AppLogger::initialise();
        AppLogger::write ("Live session initialising");

        workspaceState.load();
        tonePresetMapping.setWorkspaceName (workspaceState.workspaceName);

        bool ok = true;

        if (workspaceState.autoStartAudio)
            ok = startAudio() && ok;

        if (workspaceState.autoConnectMidi)
        {
            if (workspaceState.lastMidiOutput.isNotEmpty())
                ok = connectCubaseMidiOutput (workspaceState.lastMidiOutput) && ok;
            else
                ok = connectCubaseMidiAuto() && ok;
        }

        saveRuntimeState();
        return ok;
    }

    void AutoMenuCore::shutdownLiveSession()
    {
        saveRuntimeState();
        stopAudio();
        disconnectCubase();
        AppLogger::write ("Live session shutdown");
        AppLogger::shutdown();
    }

    void AutoMenuCore::saveRuntimeState()
    {
        workspaceState.workspaceName = tonePresetMapping.getWorkspaceName();
        workspaceState.lastMidiOutput = getCubaseMidiOutputName();
        workspaceState.lastAudioDevice = audioEngine.getCurrentDeviceName();
        workspaceState.save();
    }

    void AutoMenuCore::reconnectIfNeeded()
    {
        const auto now = juce::Time::getMillisecondCounter();

        if (now - lastReconnectAttemptMs < 2500)
            return;

        lastReconnectAttemptMs = now;

        if (workspaceState.autoStartAudio && ! audioEngine.isRunning())
        {
            AppLogger::write ("Audio reconnect attempt");
            startAudio();
        }

        if (workspaceState.autoConnectMidi && ! isCubaseConnected())
        {
            AppLogger::write ("MIDI reconnect attempt");

            if (workspaceState.lastMidiOutput.isNotEmpty())
                connectCubaseMidiOutput (workspaceState.lastMidiOutput);
            else
                connectCubaseMidiAuto();
        }
    }

    bool AutoMenuCore::startAudio()
    {
        const auto ok = audioEngine.initialise (2, 0);
        AppLogger::write (ok ? "Audio started" : "Audio start failed");
        return ok;
    }

    void AutoMenuCore::stopAudio()
    {
        audioEngine.shutdown();
    }

    bool AutoMenuCore::connectCubaseMidiAuto()
    {
        auto outputs = cubaseManager.getAvailableMidiOutputs();

        for (const auto& name : outputs)
        {
            if (name.containsIgnoreCase ("loopMIDI") ||
                name.containsIgnoreCase ("AutoMenu") ||
                name.containsIgnoreCase ("Cubase"))
            {
                return connectCubaseMidiOutput (name);
            }
        }

        if (outputs.size() > 0)
            return connectCubaseMidiOutput (outputs[0]);

        return false;
    }

    bool AutoMenuCore::connectCubaseMidiOutput (const juce::String& outputName)
    {
        const auto ok = cubaseManager.connectToMidiOutput (outputName);

        if (ok)
        {
            workspaceState.lastMidiOutput = outputName;
            AppLogger::write ("Cubase MIDI connected: " + outputName);
        }
        else
        {
            AppLogger::write ("Cubase MIDI connect failed: " + outputName);
        }

        return ok;
    }

    void AutoMenuCore::disconnectCubase()
    {
        cubaseManager.disconnect();
    }

    bool AutoMenuCore::isCubaseConnected() const
    {
        return cubaseManager.isConnected();
    }

    juce::String AutoMenuCore::getCubaseMidiOutputName() const
    {
        return cubaseManager.getConnectedOutputName();
    }

    void AutoMenuCore::triggerMacro (int macroIndex)
    {
        macroManager.triggerMacro (macroIndex);
    }

    bool AutoMenuCore::applyCurrentSuggestion()
    {
        const auto suggestion = getLatestSuggestion();

        if (! suggestion.canApply || suggestion.macroIndex < 0)
            return false;

        macroManager.triggerMacro (suggestion.macroIndex);
        AppLogger::write ("Applied suggestion: " + suggestion.presetName);
        return true;
    }

    AnalysisResult AutoMenuCore::getAnalysisResult() const
    {
        return analysisManager.getLatestResult();
    }

    RealtimeAnalysisState AutoMenuCore::getRealtimeAnalysisState() const
    {
        return RealtimeAnalysisState::fromToneState (analysisManager.getLatestToneState());
    }

    RealtimeToneState AutoMenuCore::getRealtimeToneState() const
    {
        return analysisManager.getLatestToneState();
    }

    SuggestedPreset AutoMenuCore::updateSuggestion()
    {
        const auto tone = analysisManager.getLatestToneState();
        const auto now = juce::Time::getMillisecondCounterHiRes() * 0.001;
        return suggestionEngine.update (tone, tonePresetMapping, now);
    }

    SuggestedPreset AutoMenuCore::getLatestSuggestion() const
    {
        return suggestionEngine.getLatestSuggestion();
    }

    void AutoMenuCore::setPerformanceMode (bool shouldEnable)
    {
        workspaceState.performanceMode = shouldEnable;
        saveRuntimeState();
    }

    bool AutoMenuCore::isPerformanceMode() const noexcept
    {
        return workspaceState.performanceMode;
    }

    juce::String AutoMenuCore::getWorkspaceName() const
    {
        return workspaceState.workspaceName;
    }

    juce::String AutoMenuCore::getLiveStatusText() const
    {
        juce::StringArray parts;
        parts.add (isPerformanceMode() ? "PERFORMANCE" : "SETUP");
        parts.add (audioEngine.isRunning() ? "Audio OK" : "Audio OFF");
        parts.add (isCubaseConnected() ? "Cubase OK" : "Cubase OFF");
        parts.add (getWorkspaceName());
        return parts.joinIntoString ("  |  ");
    }
}
