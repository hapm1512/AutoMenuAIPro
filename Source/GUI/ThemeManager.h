#pragma once
#include <JuceHeader.h>

class ThemeManager
{
public:
    enum class Mode { dark, light };

    static ThemeManager& get()
    {
        static ThemeManager tm;
        return tm;
    }

    void setMode (Mode newMode) noexcept { mode = newMode; }
    Mode getMode() const noexcept { return mode; }
    void toggle() noexcept { mode = (mode == Mode::dark ? Mode::light : Mode::dark); }
    bool isLight() const noexcept { return mode == Mode::light; }

    juce::Colour backgroundTop() const noexcept
    {
        return isLight() ? juce::Colour (0xffe8e5dc) : juce::Colour (0xff252526);
    }

    juce::Colour backgroundBottom() const noexcept
    {
        return isLight() ? juce::Colour (0xffbdb8ad) : juce::Colour (0xff060707);
    }

    juce::Colour panel() const noexcept
    {
        return isLight() ? juce::Colour (0xfff4f0e6) : juce::Colour (0xff111213);
    }

    juce::Colour text() const noexcept
    {
        return isLight() ? juce::Colour (0xff222222) : juce::Colour (0xffdddddd);
    }

    juce::Colour mutedText() const noexcept
    {
        return isLight() ? juce::Colour (0xff555555) : juce::Colour (0xff8d8d8d);
    }

private:
    ThemeManager() = default;
    Mode mode { Mode::dark };
};
