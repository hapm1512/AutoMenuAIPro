#include "TonePresetMapping.h"

namespace AutoMenu
{
    TonePresetMapping::TonePresetMapping()
    {
        resetToDefaults();
    }

    void TonePresetMapping::resetToDefaults()
    {
        mappings.clear();
        workspaceName = "Live";

        setMapping ("C Major",  "Live_C",        0);
        setMapping ("C# Major", "Live_CSharp",   0);
        setMapping ("D Major",  "Live_D",        0);
        setMapping ("Eb Major", "Live_Eb",       0);
        setMapping ("E Major",  "Live_E",        0);
        setMapping ("F Major",  "Live_F",        0);
        setMapping ("F# Major", "Live_FSharp",   0);
        setMapping ("G Major",  "Live_G",        0);
        setMapping ("Ab Major", "Live_Ab",       0);
        setMapping ("A Major",  "Live_A",        0);
        setMapping ("Bb Major", "Live_Bb",       0);
        setMapping ("B Major",  "Live_B",        0);

        setMapping ("C Minor",  "Live_C_Minor",  0);
        setMapping ("C# Minor", "Live_CSharp_Minor", 0);
        setMapping ("D Minor",  "Live_D_Minor",  0);
        setMapping ("Eb Minor", "Live_Eb_Minor", 0);
        setMapping ("E Minor",  "Live_E_Minor",  0);
        setMapping ("F Minor",  "Live_F_Minor",  0);
        setMapping ("F# Minor", "Live_FSharp_Minor", 0);
        setMapping ("G Minor",  "Live_G_Minor",  0);
        setMapping ("Ab Minor", "Live_Ab_Minor", 0);
        setMapping ("A Minor",  "Live_A_Minor",  0);
        setMapping ("Bb Minor", "Live_Bb_Minor", 0);
        setMapping ("B Minor",  "Live_B_Minor",  0);
    }

    void TonePresetMapping::setWorkspaceName (const juce::String& name)
    {
        workspaceName = name.isNotEmpty() ? name : "Live";
    }

    juce::String TonePresetMapping::getWorkspaceName() const
    {
        return workspaceName;
    }

    void TonePresetMapping::setMapping (const juce::String& tone,
                                        const juce::String& presetName,
                                        int macroIndex)
    {
        const auto key = normaliseTone (tone);

        TonePresetEntry entry;
        entry.tone = tone;
        entry.presetName = presetName;
        entry.macroIndex = macroIndex;

        mappings[key] = entry;
    }

    TonePresetEntry TonePresetMapping::findPresetForTone (const juce::String& tone) const
    {
        const auto key = normaliseTone (tone);
        const auto it = mappings.find (key);

        if (it != mappings.end())
            return it->second;

        TonePresetEntry empty;
        empty.tone = tone;
        empty.presetName = "--";
        empty.macroIndex = -1;
        return empty;
    }

    juce::String TonePresetMapping::normaliseTone (juce::String tone)
    {
        tone = tone.trim().toLowerCase();
        tone = tone.replace ("  ", " ");
        return tone;
    }
}
