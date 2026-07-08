#pragma once
#include <JuceHeader.h>

class TopBar final : public juce::Component
{
public:
    TopBar();

    void paint (juce::Graphics&) override;
    void resized() override;

    void setPresetName (const juce::String& name);
    void setStatusText (const juce::String& text);
    void setThemeIsLight (bool shouldUseLightTheme);

    std::function<void()> onPreviousPreset;
    std::function<void()> onNextPreset;
    std::function<void()> onSavePreset;
    std::function<void()> onABSwap;
    std::function<void()> onCopyAToB;
    std::function<void()> onUndo;
    std::function<void()> onRedo;
    std::function<void()> onThemeToggle;

private:
    juce::Label title, subtitle, preset, version, status;
    juce::TextButton prev { "<" }, next { ">" };
    juce::TextButton favorite { "★" }, save { "SAVE" }, category { "VOCAL" };
    juce::TextButton a { "A" }, b { "B" }, copy { "A > B" };
    juce::TextButton undo { "UNDO" }, redo { "REDO" };
    juce::TextButton theme { "THEME" }, settings { "SETTINGS" }, help { "HELP" }, bypass { "BYPASS ALL" };
    bool lightTheme { false };
};
