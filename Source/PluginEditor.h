#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/ChannelStripEditor.h"

class VocalSuiteUltraProAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit VocalSuiteUltraProAudioProcessorEditor (VocalSuiteUltraProAudioProcessor& processor);
    ~VocalSuiteUltraProAudioProcessorEditor() override = default;

    void resized() override;

private:
    ChannelStripEditor editor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VocalSuiteUltraProAudioProcessorEditor)
};
