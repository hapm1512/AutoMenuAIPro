#include "AutoMenuCore.h"

namespace AutoMenu
{
    AutoMenuCore::AutoMenuCore()
        : macroManager (cubaseManager)
    {
        audioEngine.setAnalysisManager (&analysisManager);
    }

    AutoMenuCore::~AutoMenuCore()
    {
        stopAudio();
        disconnectCubase();
    }

    bool AutoMenuCore::startAudio()
    {
        return audioEngine.initialise (2, 0);
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
                return cubaseManager.connectToMidiOutput (name);
            }
        }

        if (outputs.size() > 0)
            return cubaseManager.connectToMidiOutput (outputs[0]);

        return false;
    }

    bool AutoMenuCore::connectCubaseMidiOutput (const juce::String& outputName)
    {
        return cubaseManager.connectToMidiOutput (outputName);
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
}
