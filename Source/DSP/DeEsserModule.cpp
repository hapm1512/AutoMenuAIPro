#include "DeEsserModule.h"

void DeEsserModule::prepare (double sr, int bs, int ch)
{
    DspModuleBase::prepare (sr, bs, ch);
    reset();
}

void DeEsserModule::reset()
{
    for (auto& v : splitLp) v = 0.0f;
    for (auto& v : env) v = 0.0f;
    for (auto& v : reductionGain) v = 1.0f;
}

void DeEsserModule::process (juce::AudioBuffer<float>& buffer,
                             juce::AudioProcessorValueTreeState& apvts)
{
    if (! getOn (apvts, "deesserOn"))
        return;

    const auto freq = getParam (apvts, "deessFreq", 6000.0f);
    const auto threshold = getParam (apvts, "deessThreshold", -28.0f);
    const auto rangeDb = getParam (apvts, "deessRange", -8.0f);
    const auto releaseMs = getParam (apvts, "deessRelease", 80.0f);

    const auto splitCoeff = freqToOnePoleCoeff (freq);
    const auto detectorCoeff = msToCoeff (3.0f);
    const auto releaseCoeff = msToCoeff (releaseMs);
    const auto maxReduction = dbToGain (rangeDb);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* x = buffer.getWritePointer (ch);
        const auto idx = ch % 2;

        for (int n = 0; n < buffer.getNumSamples(); ++n)
        {
            const auto input = x[n];

            splitLp[idx] = (1.0f - splitCoeff) * input + splitCoeff * splitLp[idx];
            const auto high = input - splitLp[idx];

            env[idx] = (1.0f - detectorCoeff) * std::abs (high) + detectorCoeff * env[idx];

            float target = 1.0f;
            const auto envDb = gainToDb (env[idx]);

            if (envDb > threshold)
            {
                const auto over = juce::jlimit (0.0f, 1.0f, (envDb - threshold) / 18.0f);
                target = 1.0f + over * (maxReduction - 1.0f);
            }

            const auto coeff = target < reductionGain[idx] ? 0.25f : releaseCoeff;
            reductionGain[idx] = target + coeff * (reductionGain[idx] - target);

            x[n] = splitLp[idx] + high * reductionGain[idx];
        }
    }
}
