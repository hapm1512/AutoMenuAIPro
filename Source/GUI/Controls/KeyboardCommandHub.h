#pragma once
#include <JuceHeader.h>

class KeyboardCommandHub
{
public:
    std::function<void()> onUndo;
    std::function<void()> onRedo;
    std::function<void()> onAB;
    std::function<void()> onBypass;
    std::function<void()> onTheme;

    bool handleKeyPress (const juce::KeyPress& key)
    {
        const auto mods = key.getModifiers();

        if (mods.isCommandDown() && key.getKeyCode() == 'z')
        {
            if (onUndo) onUndo();
            return true;
        }

        if (mods.isCommandDown() && key.getKeyCode() == 'y')
        {
            if (onRedo) onRedo();
            return true;
        }

        if (key.getKeyCode() == 'a')
        {
            if (onAB) onAB();
            return true;
        }

        if (key.getKeyCode() == 'b')
        {
            if (onBypass) onBypass();
            return true;
        }

        if (key.getKeyCode() == 't')
        {
            if (onTheme) onTheme();
            return true;
        }

        return false;
    }
};
