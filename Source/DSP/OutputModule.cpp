#include "OutputModule.h"

void OutputModule::prepare (double sr, int bs, int ch)
{
    DspModuleBase::prepare (sr, bs, ch);
    reset();
}

void OutputModule::reset()
{
    for (auto& v : limiterGain) v = 1.0f;
}

void OutputModule::process (juce::AudioBuffer<float>& buffer,
                            juce::AudioProcessorValueTreeState& apvts)
{
    if (! getOn (apvts, "outputOn"))
        return;

    const auto ceilingDb = getParam (apvts, "outCeiling", -1.0f);
    const auto thresholdDb = getParam (apvts, "outThreshold", -2.0f);

    const auto ceiling = dbToGain (ceilingDb);
    const auto threshold = dbToGain (thresholdDb);
    const auto releaseCoeff = msToCoeff (50.0f);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* x = buffer.getWritePointer (ch);
        const auto idx = ch % 2;

        for (int n = 0; n < buffer.getNumSamples(); ++n)
        {
            auto v = x[n];
            const auto av = std::abs (v);

            float target = 1.0f;
            if (av > threshold && av > 0.000001f)
                target = juce::jmin (1.0f, ceiling / av);

            limiterGain[idx] = target < limiterGain[idx]
                             ? target
                             : target + releaseCoeff * (limiterGain[idx] - target);

            v *= limiterGain[idx];
            x[n] = juce::jlimit (-ceiling, ceiling, v);
        }
    }
}
