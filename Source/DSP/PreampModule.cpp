#include "PreampModule.h"

void PreampModule::process (juce::AudioBuffer<float>& buffer,
                     juce::AudioProcessorValueTreeState& apvts)
{
    auto* on = apvts.getRawParameterValue ("preampOn");
    if (on != nullptr && on->load() < 0.5f)
        return;

    // Sprint 1: DSP shell.
    // Sprint sau thêm thuật toán thật.
    juce::ignoreUnused (buffer);
}
