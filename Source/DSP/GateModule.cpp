#include "GateModule.h"

void GateModule::process (juce::AudioBuffer<float>& buffer,
                     juce::AudioProcessorValueTreeState& apvts)
{
    auto* on = apvts.getRawParameterValue ("gateOn");
    if (on != nullptr && on->load() < 0.5f)
        return;

    // Sprint 1: DSP shell.
    // Sprint sau thêm thuật toán thật.
    juce::ignoreUnused (buffer);
}
