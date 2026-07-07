#include "MacroManager.h"

namespace AutoMenu
{
    MacroManager::MacroManager(CubaseManager& cubase)
        : cubaseManager(cubase)
    {
    }

    void MacroManager::triggerMacro(int macroIndex)
    {
        cubaseManager.triggerMacro(macroIndex);
    }
}