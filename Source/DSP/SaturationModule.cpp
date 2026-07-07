#include "SaturationModule.h"

void SaturationModule::prepare (double sr, int bs, int ch)
{
    DspModuleBase::prepare (sr, bs, ch);
    reset();
}

void SaturationModule::reset()
{
    for (auto& v : toneLp) v = 0.0f;
}

void SaturationModule::process (juce::AudioBuffer<float>& buffer,
                                juce::AudioProcessorValueTreeState& apvts)
{
    if (! getOn (apvts, "saturationOn"))
        return;

    const auto drive = getParam (apvts, "satDrive", 24.0f) / 100.0f;
    const auto tone = getParam (apvts, "satTone", 12.0f) / 100.0f;
    const auto mix = getParam (apvts, "satMix", 100.0f) / 100.0f;

    const auto driveGain = 1.0f + drive * 6.0f;
    const auto outputTrim = 1.0f / (1.0f + drive * 1.8f);
    const auto toneCoeff = freqToOnePoleCoeff (juce::jmap (tone, 1200.0f, 9000.0f));

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* x = buffer.getWritePointer (ch);
        const auto idx = ch % 2;

        for (int n = 0; n < buffer.getNumSamples(); ++n)
        {
            const auto dry = x[n];
            const auto driven = dry * driveGain;
            const auto sat = std::tanh (driven + 0.15f * driven * driven) * outputTrim;

            toneLp[idx] = (1.0f - toneCoeff) * sat + toneCoeff * toneLp[idx];
            const auto shaped = toneLp[idx] + (sat - toneLp[idx]) * (0.65f + tone);

            x[n] = dry + (shaped - dry) * mix;
        }
    }
}
