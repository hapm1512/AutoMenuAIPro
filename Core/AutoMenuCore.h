#pragma once

#include <juce_events/juce_events.h>

#include "RealtimeAnalysisState.h"
#include "RealtimeToneState.h"
#include "SuggestionEngine.h"
#include "SuggestionTypes.h"
#include "MacroManager.h"
#include "../Audio/AudioEngine.h"
#include "../Analysis/AnalysisManager.h"
#include "../Cubase/CubaseManager.h"
#include "../Workspace/TonePresetMapping.h"
#include "../Workspace/WorkspaceState.h"

namespace AutoMenu
{
    class AutoMenuCore final
    {
    public:
        AutoMenuCore();
        ~AutoMenuCore();

        bool initialiseLiveSession();
        void shutdownLiveSession();
        void saveRuntimeState();
        void reconnectIfNeeded();

        bool startAudio();
        void stopAudio();

        bool connectCubaseMidiAuto();
        bool connectCubaseMidiOutput (const juce::String& outputName);
        void disconnectCubase();
        bool isCubaseConnected() const;
        juce::String getCubaseMidiOutputName() const;

        void triggerMacro (int macroIndex);
        bool applyCurrentSuggestion();

        AnalysisResult getAnalysisResult() const;
        RealtimeAnalysisState getRealtimeAnalysisState() const;
        RealtimeToneState getRealtimeToneState() const;
        SuggestedPreset updateSuggestion();
        SuggestedPreset getLatestSuggestion() const;

        void setPerformanceMode (bool shouldEnable);
        bool isPerformanceMode() const noexcept;

        juce::String getWorkspaceName() const;
        juce::String getLiveStatusText() const;

        AudioEngine& getAudioEngine() noexcept { return audioEngine; }
        AnalysisManager& getAnalysisManager() noexcept { return analysisManager; }
        CubaseManager& getCubaseManager() noexcept { return cubaseManager; }
        MacroManager& getMacroManager() noexcept { return macroManager; }
        TonePresetMapping& getTonePresetMapping() noexcept { return tonePresetMapping; }
        SuggestionEngine& getSuggestionEngine() noexcept { return suggestionEngine; }
        WorkspaceState& getWorkspaceState() noexcept { return workspaceState; }

    private:
        AudioEngine audioEngine;
        AnalysisManager analysisManager;
        CubaseManager cubaseManager;
        MacroManager macroManager;
        TonePresetMapping tonePresetMapping;
        SuggestionEngine suggestionEngine;
        WorkspaceState workspaceState;

        juce::uint32 lastReconnectAttemptMs = 0;
    };
}
