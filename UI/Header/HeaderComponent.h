#pragma once
#include <juce_gui_extra/juce_gui_extra.h>

class HeaderComponent final : public juce::Component
{
public:
    HeaderComponent();
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    juce::Rectangle<int> logoArea, titleArea, statusArea, actionArea;
    void drawLogo (juce::Graphics& g);
    void drawStatusBox (juce::Graphics& g);
    void drawActionButtons (juce::Graphics& g);
};
