#pragma once

#include <juce_core/juce_core.h>
#include "../Cubase/CubaseManager.h"

namespace AutoMenu
{
    class MacroManager final
    {
    public:
        explicit MacroManager(CubaseManager& cubase);

        void triggerMacro(int macroIndex);

    private:
        CubaseManager& cubaseManager;
    };
}