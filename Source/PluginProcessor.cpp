#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters/ParameterLayout.h"

VocalSuiteUltraProAudioProcessor::VocalSuiteUltraProAudioProcessor()
    : AudioProcessor (BusesProperties()
        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "PARAMETERS", Parameters::createLayout()),
      presetManager (apvts)
{
}

void VocalSuiteUltraProAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    dsp.prepare (sampleRate, samplesPerBlock, getTotalNumOutputChannels());
}

void VocalSuiteUltraProAudioProcessor::releaseResources()
{
    dsp.reset();
}

bool VocalSuiteUltraProAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto in = layouts.getMainInputChannelSet();
    const auto out = layouts.getMainOutputChannelSet();

    return in == out && (out == juce::AudioChannelSet::mono()
                      || out == juce::AudioChannelSet::stereo());
}

void VocalSuiteUltraProAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                                     juce::MidiBuffer& midi)
{
    juce::ignoreUnused (midi);
    juce::ScopedNoDenormals noDenormals;

    dsp.process (buffer, apvts);
}

juce::AudioProcessorEditor* VocalSuiteUltraProAudioProcessor::createEditor()
{
    return new VocalSuiteUltraProAudioProcessorEditor (*this);
}

void VocalSuiteUltraProAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    state.setProperty ("presetName", presetManager.getCurrentPresetName(), nullptr);

    if (auto xml = state.createXml())
        copyXmlToBinary (*xml, destData);
}

void VocalSuiteUltraProAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

float VocalSuiteUltraProAudioProcessor::getInputPeak() const noexcept
{
    return dsp.getInputPeak();
}

float VocalSuiteUltraProAudioProcessor::getOutputPeak() const noexcept
{
    return dsp.getOutputPeak();
}

float VocalSuiteUltraProAudioProcessor::getGainReduction() const noexcept
{
    return dsp.getGainReduction();
}

float VocalSuiteUltraProAudioProcessor::getInputRms() const noexcept
{
    return dsp.getInputRms();
}

float VocalSuiteUltraProAudioProcessor::getOutputRms() const noexcept
{
    return dsp.getOutputRms();
}

float VocalSuiteUltraProAudioProcessor::getTruePeak() const noexcept
{
    return dsp.getTruePeak();
}

float VocalSuiteUltraProAudioProcessor::getLufsMomentary() const noexcept
{
    return dsp.getLufsMomentary();
}

float VocalSuiteUltraProAudioProcessor::getLufsShortTerm() const noexcept
{
    return dsp.getLufsShortTerm();
}

float VocalSuiteUltraProAudioProcessor::getLufsIntegrated() const noexcept
{
    return dsp.getLufsIntegrated();
}

float VocalSuiteUltraProAudioProcessor::getStereoCorrelation() const noexcept
{
    return dsp.getStereoCorrelation();
}

juce::AudioProcessor* createPluginFilter()
{
    return new VocalSuiteUltraProAudioProcessor();
}
