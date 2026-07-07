#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include <functional>

class FooterComponent final : public juce::Component
{
public:
    FooterComponent();

    void paint (juce::Graphics& g) override;
    void resized() override;
    void mouseUp (const juce::MouseEvent& event) override;

    void setStatus (const juce::String& workspace,
                    bool audioConnected,
                    bool cubaseConnected,
                    bool performanceMode,
                    const juce::String& midiName);

    std::function<void()> onSettings;

private:
    juce::String workspaceName = "Live Vocal";
    juce::String midiOutputName = "loopMIDI";
    bool audioOk = false;
    bool cubaseOk = false;
    bool performance = false;

    juce::Rectangle<int> settingsButtonArea;
};
