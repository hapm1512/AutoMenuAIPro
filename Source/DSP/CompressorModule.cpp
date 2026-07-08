#include "CompressorModule.h"

void CompressorModule::prepare (double sr, int bs, int ch)
{
    DspModuleBase::prepare (sr, bs, ch);
    reset();
}

void CompressorModule::reset()
{
    for (auto& v : fet.detector) v = 0.0f;
    for (auto& v : fet.gain) v = 1.0f;
    for (auto& v : fet.hp) v = 0.0f;

    for (auto& v : opto.detector) v = 0.0f;
    for (auto& v : opto.optoMemory) v = 0.0f;
    for (auto& v : opto.gain) v = 1.0f;

    inputSmooth = 1.0f;
    outputSmooth = 1.0f;
    mixSmooth = 1.0f;
    lastGainReduction = 0.0f;
}

float CompressorModule::analogColour (float x, float amount) noexcept
{
    const auto a = juce::jlimit (0.0f, 1.0f, amount);
    const auto drive = 1.0f + a * 1.35f;
    const auto shaped = std::tanh (x * drive) / std::tanh (drive);
    const auto even = 0.035f * a * (x * x) * (x >= 0.0f ? 1.0f : -1.0f);
    return shaped + even;
}

float CompressorModule::process1176Sample (float x, int ch, float thresholdDb, float ratio,
                                           float attackMs, float releaseMs, float sidechainHp,
                                           float analog, float& grDb) noexcept
{
    const auto idx = ch % 2;
    const auto hpCoeff = freqToOnePoleCoeff (sidechainHp);
    fet.hp[idx] = x + hpCoeff * fet.hp[idx];
    const auto side = x - fet.hp[idx] * 0.08f;

    const auto peak = std::abs (side);
    const auto detCoeff = peak > fet.detector[idx] ? msToCoeff (0.15f) : msToCoeff (18.0f);
    fet.detector[idx] = peak + detCoeff * (fet.detector[idx] - peak);

    const auto envDb = gainToDb (fet.detector[idx]);
    float targetGain = 1.0f;

    if (envDb > thresholdDb)
    {
        const auto over = envDb - thresholdDb;
        const auto knee = 3.0f;
        const auto softOver = over < knee ? (over * over) / (2.0f * knee) : over - knee * 0.5f;
        const auto reduceDb = softOver * (1.0f - 1.0f / ratio);
        targetGain = dbToGain (-reduceDb);
    }

    const auto coeff = targetGain < fet.gain[idx] ? msToCoeff (attackMs) : msToCoeff (releaseMs);
    fet.gain[idx] = targetGain + coeff * (fet.gain[idx] - targetGain);

    grDb = -gainToDb (fet.gain[idx]);
    return analogColour (x * fet.gain[idx], analog * 0.55f);
}

float CompressorModule::processLA2ASample (float x, int ch, float peakReduction,
                                           float speed, float analog, float& grDb) noexcept
{
    const auto idx = ch % 2;
    const auto reduction = juce::jlimit (0.0f, 1.0f, peakReduction / 100.0f);
    const auto speed01 = juce::jlimit (0.0f, 1.0f, speed / 100.0f);

    const auto attack = juce::jmap (speed01, 18.0f, 4.0f);
    const auto releaseFast = juce::jmap (speed01, 130.0f, 55.0f);
    const auto releaseSlow = juce::jmap (speed01, 1100.0f, 420.0f);

    const auto level = std::abs (x);
    const auto detCoeff = level > opto.detector[idx] ? msToCoeff (attack) : msToCoeff (releaseFast);
    opto.detector[idx] = level + detCoeff * (opto.detector[idx] - level);

    const auto memoryCoeff = msToCoeff (releaseSlow);
    opto.optoMemory[idx] = juce::jmax (opto.detector[idx], opto.optoMemory[idx] * memoryCoeff);

    const auto envDb = gainToDb (opto.detector[idx] * 0.65f + opto.optoMemory[idx] * 0.35f);
    const auto thresholdDb = -32.0f + (1.0f - reduction) * 24.0f;

    float targetGain = 1.0f;
    if (envDb > thresholdDb)
    {
        const auto over = envDb - thresholdDb;
        const auto ratio = 2.1f + reduction * 2.4f;
        targetGain = dbToGain (-over * (1.0f - 1.0f / ratio));
    }

    const auto gainCoeff = targetGain < opto.gain[idx] ? msToCoeff (attack) : msToCoeff (releaseFast);
    opto.gain[idx] = targetGain + gainCoeff * (opto.gain[idx] - targetGain);

    grDb = -gainToDb (opto.gain[idx]);
    return analogColour (x * opto.gain[idx], analog * 0.45f);
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
    const auto ratio = getParam (apvts, "comp1176Ratio", 4.0f);
    const auto fetAmount = getParam (apvts, "comp1176Amount", 35.0f);
    const auto optoAmount = getParam (apvts, "compLA2APeak", 25.0f);
    const auto optoGainDb = getParam (apvts, "compLA2AGain", 0.0f);
    const auto optoSpeed = getParam (apvts, "compLA2ASpeed", 45.0f);
    const auto parallel = getParam (apvts, "compParallel", 100.0f) / 100.0f;
    const auto sidechainHp = getParam (apvts, "compSideHp", 90.0f);
    const auto analog = getParam (apvts, "compAnalog", 30.0f) / 100.0f;
    const auto autoGain = getParam (apvts, "compAutoGain", 55.0f) / 100.0f;

    const auto targetInput = dbToGain (inputDb);
    const auto targetOutput = dbToGain (outputDb + optoGainDb * 0.50f);
    const auto smoothCoeff = msToCoeff (22.0f);
    const auto fetThreshold = -6.0f - fetAmount * 0.34f;

    float maxGrDb = 0.0f;

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* x = buffer.getWritePointer (ch);

        for (int n = 0; n < buffer.getNumSamples(); ++n)
        {
            inputSmooth = targetInput + smoothCoeff * (inputSmooth - targetInput);
            outputSmooth = targetOutput + smoothCoeff * (outputSmooth - targetOutput);
            mixSmooth = parallel + smoothCoeff * (mixSmooth - parallel);

            const auto dry = x[n];
            float gr76 = 0.0f;
            float gr2a = 0.0f;

            auto y = dry * inputSmooth;
            y = process1176Sample (y, ch, fetThreshold, ratio, attackMs, releaseMs, sidechainHp, analog, gr76);
            y = processLA2ASample (y, ch, optoAmount, optoSpeed, analog, gr2a);

            const auto totalGr = gr76 + gr2a;
            const auto makeup = dbToGain (juce::jlimit (0.0f, 9.0f, totalGr * 0.38f) * autoGain);
            y *= makeup * outputSmooth;

            x[n] = dry + (y - dry) * juce::jlimit (0.0f, 1.0f, mixSmooth);
            x[n] = juce::jlimit (-1.25f, 1.25f, x[n]);

            maxGrDb = juce::jmax (maxGrDb, totalGr);
        }
    }

    lastGainReduction = juce::jlimit (0.0f, 1.0f, maxGrDb / 24.0f);
}
