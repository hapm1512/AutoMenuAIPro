#include "PreampModule.h"

void PreampModule::prepare (double sr, int bs, int ch)
{
    DspModuleBase::prepare (sr, bs, ch);

    driveGain.reset (sampleRate, 0.02);
    outputGain.reset (sampleRate, 0.02);
    harmonic.reset (sampleRate, 0.02);

    reset();
}

void PreampModule::reset()
{
    driveGain.setCurrentAndTargetValue (1.0f);
    outputGain.setCurrentAndTargetValue (1.0f);
    harmonic.setCurrentAndTargetValue (0.0f);
}

void PreampModule::process (juce::AudioBuffer<float>& buffer,
                            juce::AudioProcessorValueTreeState& apvts)
{
    if (! getOn (apvts, "preampOn"))
        return;

    const auto drive = getParam (apvts, "preampDrive", 23.0f);
    const auto harm = getParam (apvts, "preampHarmonic", 35.0f);
    const auto outDb = getParam (apvts, "preampOutput", 0.0f);

    driveGain.setTargetValue (dbToGain (drive * 0.12f));
    outputGain.setTargetValue (dbToGain (outDb - drive * 0.04f));
    harmonic.setTargetValue (juce::jlimit (0.0f, 1.0f, harm / 100.0f));

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* x = buffer.getWritePointer (ch);

        for (int n = 0; n < buffer.getNumSamples(); ++n)
        {
            const auto dg = driveGain.getNextValue();
            const auto og = outputGain.getNextValue();
            const auto h = harmonic.getNextValue();

            const auto driven = x[n] * dg;
            const auto asym = driven + 0.06f * h * driven * driven;
            const auto tube = std::tanh (asym);
            const auto blend = 0.15f + h * 0.35f;

            x[n] = juce::jlimit (-1.0f, 1.0f, ((1.0f - blend) * driven + blend * tube) * og);
        }
    }
}
