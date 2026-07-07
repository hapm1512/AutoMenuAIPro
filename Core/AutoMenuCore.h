#pragma once

#include "../Audio/AudioEngine.h"
#include "../Analysis/AnalysisManager.h"
#include "../Cubase/CubaseManager.h"
#include "MacroManager.h"
#include "RealtimeAnalysisState.h"
#include "RealtimeToneState.h"
#include "SuggestionEngine.h"
#include "SuggestionTypes.h"
#include "../Workspace/TonePresetMapping.h"

namespace AutoMenu
{
    class AutoMenuCore final
    {
    public:
        AutoMenuCore();
        ~AutoMenuCore();

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

        AudioEngine& getAudioEngine() noexcept { return audioEngine; }
        AnalysisManager& getAnalysisManager() noexcept { return analysisManager; }
        CubaseManager& getCubaseManager() noexcept { return cubaseManager; }
        MacroManager& getMacroManager() noexcept { return macroManager; }
        TonePresetMapping& getTonePresetMapping() noexcept { return tonePresetMapping; }
        SuggestionEngine& getSuggestionEngine() noexcept { return suggestionEngine; }

    private:
        AudioEngine audioEngine;
        AnalysisManager analysisManager;
        CubaseManager cubaseManager;
        MacroManager macroManager;
        TonePresetMapping tonePresetMapping;
        SuggestionEngine suggestionEngine;
    };
}
