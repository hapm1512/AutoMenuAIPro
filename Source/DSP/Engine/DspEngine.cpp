#include "DspEngine.h"

void DspEngine::prepare (double sampleRate, int blockSize, int channels)
{
    preamp.prepare (sampleRate, blockSize, channels);
    gate.prepare (sampleRate, blockSize, channels);
    eq.prepare (sampleRate, blockSize, channels);
    deesser.prepare (sampleRate, blockSize, channels);
    compressor.prepare (sampleRate, blockSize, channels);
    saturation.prepare (sampleRate, blockSize, channels);
    hiend.prepare (sampleRate, blockSize, channels);
    output.prepare (sampleRate, blockSize, channels);
    professionalMetering.prepare (sampleRate);

    inputPeak.store (0.0f);
    outputPeak.store (0.0f);
    inputRms.store (0.0f);
    outputRms.store (0.0f);
    gainReduction.store (0.0f);
    truePeak.store (0.0f);
    lufsMomentary.store (-70.0f);
    lufsShortTerm.store (-70.0f);
    lufsIntegrated.store (-70.0f);
    stereoCorrelation.store (1.0f);
}

void DspEngine::reset()
{
    preamp.reset();
    gate.reset();
    eq.reset();
    deesser.reset();
    compressor.reset();
    saturation.reset();
    hiend.reset();
    output.reset();
    professionalMetering.reset();
}

void DspEngine::process (juce::AudioBuffer<float>& buffer,
                         juce::AudioProcessorValueTreeState& apvts)
{
    juce::ScopedNoDenormals noDenormals;

    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    if (numSamples <= 0 || numChannels <= 0)
        return;

    // Epic 4A: safety pass before the DSP chain.
    VocalDSP::sanitizeBuffer (buffer);

    const auto in = VocalDSP::getStats (buffer);
    inputPeak.store (juce::jlimit (0.0f, 1.0f, in.peak));
    inputRms.store (juce::jlimit (0.0f, 1.0f, in.rms));

    preamp.process (buffer, apvts);
    gate.process (buffer, apvts);
    eq.process (buffer, apvts);
    deesser.process (buffer, apvts);
    compressor.process (buffer, apvts);
    saturation.process (buffer, apvts);
    hiend.process (buffer, apvts);
    output.process (buffer, apvts);

    // Epic 4A: safety pass after nonlinear/oversampled stages.
    VocalDSP::sanitizeBuffer (buffer);

    const float grDb = juce::jmax (compressor.getGainReduction(), output.getLimiterGainReductionDb());
    const auto frame = professionalMetering.analyse (in.peak, in.rms, buffer, grDb);

    outputPeak.store (juce::jlimit (0.0f, 1.0f, frame.outputPeak));
    outputRms.store (juce::jlimit (0.0f, 1.0f, frame.outputRms));
    gainReduction.store (frame.gainReduction);
    truePeak.store (frame.truePeak);
    lufsMomentary.store (frame.lufsMomentary);
    lufsShortTerm.store (frame.lufsShortTerm);
    lufsIntegrated.store (frame.lufsIntegrated);
    stereoCorrelation.store (frame.correlation);
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

float DspEngine::getInputRms() const noexcept
{
    return inputRms.load();
}

float DspEngine::getOutputRms() const noexcept
{
    return outputRms.load();
}


float DspEngine::getTruePeak() const noexcept
{
    return truePeak.load();
}

float DspEngine::getLufsMomentary() const noexcept
{
    return lufsMomentary.load();
}

float DspEngine::getLufsShortTerm() const noexcept
{
    return lufsShortTerm.load();
}

float DspEngine::getLufsIntegrated() const noexcept
{
    return lufsIntegrated.load();
}

float DspEngine::getStereoCorrelation() const noexcept
{
    return stereoCorrelation.load();
}
