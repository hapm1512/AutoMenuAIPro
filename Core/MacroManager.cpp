#include "MacroManager.h"

namespace AutoMenu
{
    MacroManager::MacroManager (CubaseManager& cubase)
        : cubaseManager (cubase),
          automationEngine (cubaseManager)
    {
        buildDefaultSequences();
    }

    void MacroManager::triggerMacro (int macroIndex)
    {
        if (macroIndex < 0 || macroIndex >= (int) macroSequences.size())
            return;

        automationEngine.playSequence (macroSequences[(size_t) macroIndex]);
    }

    int MacroManager::getMacroCount() const
    {
        return (int) macroSequences.size();
    }

    juce::String MacroManager::getMacroName (int macroIndex) const
    {
        if (macroIndex < 0 || macroIndex >= (int) macroSequences.size())
            return {};

        return macroSequences[(size_t) macroIndex].name;
    }

    bool MacroManager::isAutomationRunning() const
    {
        return automationEngine.isRunning();
    }

    juce::String MacroManager::getActiveAutomationName() const
    {
        return automationEngine.getActiveSequenceName();
    }

    void MacroManager::buildDefaultSequences()
    {
        macroSequences.clear();

        auto makeSingleCC = [] (const juce::String& name, int cc)
        {
            AutomationSequence sequence;
            sequence.name = name;
            sequence.steps = {
                AutomationStep::cc (1, cc, 127, 30),
                AutomationStep::cc (1, cc, 0, 0)
            };
            return sequence;
        };

        macroSequences.push_back ({ "AI", {
            AutomationStep::cc (1, 20, 127, 25),
            AutomationStep::cc (1, 48, 127, 25),
            AutomationStep::cc (1, 20, 0, 0)
        }});

        macroSequences.push_back (makeSingleCC ("Delay", 21));
        macroSequences.push_back (makeSingleCC ("Fix Meo", 22));
        macroSequences.push_back (makeSingleCC ("Luu Bai", 23));
        macroSequences.push_back (makeSingleCC ("Lofi", 24));
        macroSequences.push_back (makeSingleCC ("Tat Vang", 25));
        macroSequences.push_back (makeSingleCC ("Vang", 26));
        macroSequences.push_back (makeSingleCC ("Tune", 27));
        macroSequences.push_back (makeSingleCC ("Nhac", 28));
        macroSequences.push_back (makeSingleCC ("Mic", 29));

        macroSequences.push_back ({ "Remix", {
            AutomationStep::cc (1, 30, 127, 20),
            AutomationStep::program (1, 3, 20),
            AutomationStep::cc (1, 30, 0, 0)
        }});

        macroSequences.push_back (makeSingleCC ("Dan Ca", 31));
    }
}
