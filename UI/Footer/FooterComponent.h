#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

class FooterComponent final : public juce::Component
{
public:
    FooterComponent();
    void paint (juce::Graphics& g) override;
    void resized() override;
};
