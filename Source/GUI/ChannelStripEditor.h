#pragma once
#include <JuceHeader.h>
#include "GUI/Components/TopBar.h"
#include "GUI/Components/BottomMeterBar.h"
#include "GUI/Components/ModulePanel.h"

class VocalSuiteUltraProAudioProcessor;

class ChannelStripEditor final : public juce::Component,
                                 private juce::Timer
{
public:
    explicit ChannelStripEditor (VocalSuiteUltraProAudioProcessor& processor);
    ~ChannelStripEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    VocalSuiteUltraProAudioProcessor& processor;

    TopBar topBar;
    BottomMeterBar bottomMeters;
    juce::Label footer;

    juce::OwnedArray<ModulePanel> modules;

    ModulePanel& createModule (const juce::String& name, const juce::String& onParam);
    void timerCallback() override;
};
