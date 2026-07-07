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

    std::function<void()> onSettings;

private:
    juce::Rectangle<int> settingsButtonArea;
};
