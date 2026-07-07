#pragma once
#include <JuceHeader.h>

class KnobLookAndFeel final : public juce::LookAndFeel_V4
{
public:
    KnobLookAndFeel();
    void drawRotarySlider (juce::Graphics&, int x, int y, int w, int h,
                           float pos, float start, float end,
                           juce::Slider&) override;
};

class Knob final : public juce::Component
{
public:
    Knob (juce::AudioProcessorValueTreeState& state,
          const juce::String& paramId,
          const juce::String& labelText,
          juce::Colour fill);

    ~Knob() override;
    void resized() override;

private:
    KnobLookAndFeel laf;
    juce::Label label;
    juce::Slider slider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
};
