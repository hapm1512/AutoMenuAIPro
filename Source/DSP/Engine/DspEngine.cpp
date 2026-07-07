#include "DspEngine.h"

void DspEngine::prepare (double sampleRate, int blockSize, int channels)
{
    preamp.prepare (sampleRate, blockSize, channels);
    eq.prepare (sampleRate, blockSize, channels);
    gate.prepare (sampleRate, blockSize, channels);
    deesser.prepare (sampleRate, blockSize, channels);
    compressor.prepare (sampleRate, blockSize, channels);
    saturation.prepare (sampleRate, blockSize, channels);
    hiend.prepare (sampleRate, blockSize, channels);
    output.prepare (sampleRate, blockSize, channels);

    inputPeak.store (0.0f);
    outputPeak.store (0.0f);
    gainReduction.store (0.0f);
}

void DspEngine::reset()
{
    preamp.reset();
    eq.reset();
    gate.reset();
    deesser.reset();
    compressor.reset();
    saturation.reset();
    hiend.reset();
    output.reset();
}

void DspEngine::process (juce::AudioBuffer<float>& buffer,
                         juce::AudioProcessorValueTreeState& apvts)
{
    const auto in = buffer.getMagnitude (0, buffer.getNumSamples());
    inputPeak.store (juce::jlimit (0.0f, 1.0f, in));

    preamp.process (buffer, apvts);
    eq.process (buffer, apvts);
    gate.process (buffer, apvts);
    deesser.process (buffer, apvts);
    compressor.process (buffer, apvts);
    saturation.process (buffer, apvts);
    hiend.process (buffer, apvts);
    output.process (buffer, apvts);

    const auto out = buffer.getMagnitude (0, buffer.getNumSamples());
    outputPeak.store (juce::jlimit (0.0f, 1.0f, out));

    // Meter giả lập.
    gainReduction.store (0.25f);
}

float DspEngine::getInputPeak() const noexcept
{
    return inputPeak.load();
}

float DspEngine::getOutputPeak() const noexcept
{
    return outputPeak.load();
}

float DspEngine::getGainReduction() const noexcept
{
    return gainReduction.load();
}
