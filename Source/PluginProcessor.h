#pragma once
#include <JuceHeader.h>
#include "DSP/Engine/DspEngine.h"
#include "Core/PresetManager.h"

class VocalSuiteUltraProAudioProcessor final : public juce::AudioProcessor
{
public:
    VocalSuiteUltraProAudioProcessor();
    ~VocalSuiteUltraProAudioProcessor() override = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Vocal Suite Ultra Pro"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;

    float getInputPeak() const noexcept;
    float getOutputPeak() const noexcept;
    float getGainReduction() const noexcept;
    float getInputRms() const noexcept;
    float getOutputRms() const noexcept;
    float getTruePeak() const noexcept;
    float getLufsMomentary() const noexcept;
    float getLufsShortTerm() const noexcept;
    float getLufsIntegrated() const noexcept;
    float getStereoCorrelation() const noexcept;

    PresetManager& getPresetManager() noexcept { return presetManager; }
    const PresetManager& getPresetManager() const noexcept { return presetManager; }

private:
    DspEngine dsp;
    PresetManager presetManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VocalSuiteUltraProAudioProcessor)
};
