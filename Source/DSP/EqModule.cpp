#include "EqModule.h"

void EqModule::prepare (double sr, int bs, int ch)
{
    DspModuleBase::prepare (sr, bs, ch);
    reset();
}

void EqModule::reset()
{
    for (auto& v : lowState)  v = 0.0f;
    for (auto& v : highState) v = 0.0f;
    for (auto& v : airState)  v = 0.0f;
}

void EqModule::process (juce::AudioBuffer<float>& buffer,
                        juce::AudioProcessorValueTreeState& apvts)
{
    if (! getOn (apvts, "eqOn"))
        return;

    const auto lowGain  = dbToGain (getParam (apvts, "eqLow", 0.0f));
    const auto midGain  = dbToGain (getParam (apvts, "eqMid", 0.0f));
    const auto highGain = dbToGain (getParam (apvts, "eqHigh", 0.0f));
    const auto airGain  = dbToGain (getParam (apvts, "eqAir", 0.0f));

    const auto lowCoeff = freqToOnePoleCoeff (180.0f);
    const auto highCoeff = freqToOnePoleCoeff (4800.0f);
    const auto airCoeff = freqToOnePoleCoeff (11000.0f);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* x = buffer.getWritePointer (ch);
        auto idx = ch % 2;

        for (int n = 0; n < buffer.getNumSamples(); ++n)
        {
            const auto input = x[n];

            lowState[idx] = (1.0f - lowCoeff) * input + lowCoeff * lowState[idx];
            highState[idx] = (1.0f - highCoeff) * input + highCoeff * highState[idx];
            airState[idx] = (1.0f - airCoeff) * input + airCoeff * airState[idx];

            const auto low = lowState[idx];
            const auto high = input - highState[idx];
            const auto air = input - airState[idx];
            const auto mid = input - low - high;

            x[n] = low * lowGain
                 + mid * midGain
                 + high * highGain
                 + air * (airGain - 1.0f) * 0.55f;
        }
    }
}
