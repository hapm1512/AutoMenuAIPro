#pragma once
#include <JuceHeader.h>

class TopBar final : public juce::Component
{
public:
    TopBar();

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Label title, subtitle, preset, version, status;
    juce::TextButton prev { "<" }, next { ">" };
    juce::TextButton favorite { "★" }, save { "SAVE" }, category { "VOCAL" };
    juce::TextButton a { "A" }, b { "B" }, copy { "A > B" };
    juce::TextButton undo { "UNDO" }, redo { "REDO" };
    juce::TextButton settings { "SETTINGS" }, help { "HELP" }, bypass { "BYPASS ALL" };
};
