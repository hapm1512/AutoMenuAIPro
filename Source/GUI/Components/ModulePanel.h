#pragma once
#include <JuceHeader.h>
#include "Knob.h"

class ModulePanel final : public juce::Component
{
public:
    ModulePanel (juce::AudioProcessorValueTreeState& state,
                 const juce::String& title,
                 const juce::String& onParam);

    void addKnob (const juce::String& text,
                  const juce::String& paramId,
                  juce::Colour fill);

    void paint (juce::Graphics&) override;
    void resized() override;
    void setThemeIsLight (bool shouldUseLightTheme);

private:
    juce::AudioProcessorValueTreeState& apvts;
    juce::String titleText;
    bool lightTheme { false };

    juce::TextButton onButton { "ON" };
    juce::TextButton bypassButton { "BYPASS" };

    juce::OwnedArray<Knob> knobs;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> onAttachment;
};
