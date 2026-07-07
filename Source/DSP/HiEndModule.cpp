#include "HiEndModule.h"

void HiEndModule::prepare (double sr, int bs, int ch)
{
    DspModuleBase::prepare (sr, bs, ch);
    reset();
}

void HiEndModule::reset()
{
    for (auto& v : airLp) v = 0.0f;
    for (auto& v : warmLp) v = 0.0f;
}

void HiEndModule::process (juce::AudioBuffer<float>& buffer,
                           juce::AudioProcessorValueTreeState& apvts)
{
    if (! getOn (apvts, "hiendOn"))
        return;

    const auto warm = getParam (apvts, "hiWarm", 25.0f) / 100.0f;
    const auto width = getParam (apvts, "hiWidth", 100.0f) / 100.0f;
    const auto tame = getParam (apvts, "hiTame", 20.0f) / 100.0f;
    const auto glue = getParam (apvts, "hiGlue", 15.0f) / 100.0f;

    const auto airCoeff = freqToOnePoleCoeff (9500.0f);
    const auto warmCoeff = freqToOnePoleCoeff (240.0f);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* x = buffer.getWritePointer (ch);
        const auto idx = ch % 2;

        for (int n = 0; n < buffer.getNumSamples(); ++n)
        {
            const auto input = x[n];

            warmLp[idx] = (1.0f - warmCoeff) * input + warmCoeff * warmLp[idx];
            airLp[idx] = (1.0f - airCoeff) * input + airCoeff * airLp[idx];

            const auto air = input - airLp[idx];
            const auto warmed = input + warmLp[idx] * warm * 0.16f;
            const auto tamed = warmed - air * tame * 0.18f;
            x[n] = tamed * (1.0f - glue * 0.03f);
        }
    }

    if (buffer.getNumChannels() >= 2)
    {
        auto* l = buffer.getWritePointer (0);
        auto* r = buffer.getWritePointer (1);
        const auto widthGain = juce::jlimit (0.0f, 2.0f, width);

        for (int n = 0; n < buffer.getNumSamples(); ++n)
        {
            const auto mid = (l[n] + r[n]) * 0.5f;
            const auto side = (l[n] - r[n]) * 0.5f * widthGain;

            l[n] = mid + side;
            r[n] = mid - side;
        }
    }
}
