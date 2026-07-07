#pragma once

#include <juce_core/juce_core.h>
#include <vector>

#include "AutomationEngine.h"
#include "AutomationTypes.h"
#include "../Cubase/CubaseManager.h"

namespace AutoMenu
{
    class MacroManager final
    {
    public:
        explicit MacroManager (CubaseManager& cubase);

        void triggerMacro (int macroIndex);

        int getMacroCount() const;
        juce::String getMacroName (int macroIndex) const;
        bool isAutomationRunning() const;
        juce::String getActiveAutomationName() const;

    private:
        void buildDefaultSequences();

        CubaseManager& cubaseManager;
        AutomationEngine automationEngine;
        std::vector<AutomationSequence> macroSequences;
    };
}
