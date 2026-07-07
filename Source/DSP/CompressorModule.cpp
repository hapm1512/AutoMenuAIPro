#include "CompressorModule.h"

void CompressorModule::prepare (double sr, int bs, int ch)
{
    DspModuleBase::prepare (sr, bs, ch);
    reset();
}

void CompressorModule::reset()
{
    for (auto& v : env) v = 0.0f;
    for (auto& v : gain) v = 1.0f;
    lastGainReduction = 0.0f;
}

void CompressorModule::process (juce::AudioBuffer<float>& buffer,
                                juce::AudioProcessorValueTreeState& apvts)
{
    if (! getOn (apvts, "compressorOn"))
    {
        lastGainReduction = 0.0f;
        return;
    }

    const auto inputDb = getParam (apvts, "compInput", 0.0f);
    const auto outputDb = getParam (apvts, "compOutput", 0.0f);
    const auto attackMs = getParam (apvts, "compAttack", 3.0f);
    const auto releaseMs = getParam (apvts, "compRelease", 80.0f);
    const auto amount = getParam (apvts, "compPeakReduction", 25.0f);
    const auto makeupDb = getParam (apvts, "compGain", 0.0f);

    const auto inputGain = dbToGain (inputDb);
    const auto outputGain = dbToGain (outputDb + makeupDb);
    const auto threshold = -8.0f - amount * 0.32f;
    const auto ratio = 4.0f + amount * 0.10f;
    const auto attackCoeff = msToCoeff (attackMs);
    const auto releaseCoeff = msToCoeff (releaseMs);
    const auto detectorCoeff = msToCoeff (10.0f);

    float minGain = 1.0f;

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* x = buffer.getWritePointer (ch);
        const auto idx = ch % 2;

        for (int n = 0; n < buffer.getNumSamples(); ++n)
        {
            const auto input = x[n] * inputGain;

            env[idx] = (1.0f - detectorCoeff) * std::abs (input) + detectorCoeff * env[idx];
            const auto envDb = gainToDb (env[idx]);

            float targetGain = 1.0f;
            if (envDb > threshold)
            {
                const auto compressed = threshold + (envDb - threshold) / ratio;
                targetGain = dbToGain (compressed - envDb);
            }

            const auto coeff = targetGain < gain[idx] ? attackCoeff : releaseCoeff;
            gain[idx] = targetGain + coeff * (gain[idx] - targetGain);

            minGain = juce::jmin (minGain, gain[idx]);
            x[n] = input * gain[idx] * outputGain;
        }
    }

    lastGainReduction = juce::jlimit (0.0f, 1.0f, -gainToDb (minGain) / 24.0f);
}
