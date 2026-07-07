#include "GateModule.h"

void GateModule::prepare (double sr, int bs, int ch)
{
    DspModuleBase::prepare (sr, bs, ch);
    reset();
}

void GateModule::reset()
{
    for (auto& v : env) v = 0.0f;
    for (auto& v : gain) v = 1.0f;
    for (auto& v : holdCounter) v = 0;
}

void GateModule::process (juce::AudioBuffer<float>& buffer,
                          juce::AudioProcessorValueTreeState& apvts)
{
    if (! getOn (apvts, "gateOn"))
        return;

    const auto threshold = getParam (apvts, "gateThreshold", -42.0f);
    const auto rangeDb = getParam (apvts, "gateRange", -24.0f);
    const auto hyst = getParam (apvts, "gateHyst", 6.0f);
    const auto attackMs = getParam (apvts, "gateAttack", 1.0f);
    const auto holdMs = getParam (apvts, "gateHold", 60.0f);
    const auto releaseMs = getParam (apvts, "gateRelease", 120.0f);

    const auto attackCoeff = msToCoeff (attackMs);
    const auto releaseCoeff = msToCoeff (releaseMs);
    const auto detectorCoeff = msToCoeff (8.0f);
    const int holdSamples = juce::roundToInt ((holdMs * 0.001f) * (float) sampleRate);
    const auto closedGain = dbToGain (rangeDb);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* x = buffer.getWritePointer (ch);
        const auto idx = ch % 2;

        for (int n = 0; n < buffer.getNumSamples(); ++n)
        {
            const auto absx = std::abs (x[n]);
            env[idx] = (1.0f - detectorCoeff) * absx + detectorCoeff * env[idx];

            const auto envDb = gainToDb (env[idx]);
            const bool openNow = envDb > threshold;
            const bool keepOpen = envDb > (threshold - hyst);

            if (openNow)
                holdCounter[idx] = holdSamples;
            else if (holdCounter[idx] > 0 && keepOpen)
                --holdCounter[idx];

            const auto target = (openNow || holdCounter[idx] > 0) ? 1.0f : closedGain;
            const auto coeff = target > gain[idx] ? attackCoeff : releaseCoeff;

            gain[idx] = target + coeff * (gain[idx] - target);
            x[n] *= gain[idx];
        }
    }
}
