#pragma once

#include <juce_core/juce_core.h>
#include <map>

namespace AutoMenu
{
    struct TonePresetEntry
    {
        juce::String tone;
        juce::String presetName;
        int macroIndex = -1;
    };

    class TonePresetMapping final
    {
    public:
        TonePresetMapping();

        void resetToDefaults();
        void setWorkspaceName (const juce::String& name);
        juce::String getWorkspaceName() const;

        void setMapping (const juce::String& tone, const juce::String& presetName, int macroIndex);
        TonePresetEntry findPresetForTone (const juce::String& tone) const;

    private:
        static juce::String normaliseTone (juce::String tone);

        juce::String workspaceName = "Live";
        std::map<juce::String, TonePresetEntry> mappings;
    };
}
