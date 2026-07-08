#include "SaturationModule.h"

namespace
{
    float smoothAmountForBlock (double sampleRate) noexcept
    {
        const auto samples = juce::jmax (1.0f, (float) sampleRate * 0.018f);
        return 1.0f - std::exp (-1.0f / samples);
    }

    float fastTanh (float x) noexcept
    {
        return std::tanh (juce::jlimit (-8.0f, 8.0f, x));
    }

    float dbToGainSafe (float db) noexcept
    {
        return juce::Decibels::decibelsToGain (juce::jlimit (-48.0f, 24.0f, db));
    }
}

void SaturationModule::prepare (double sr, int bs, int ch)
{
    DspModuleBase::prepare (sr, bs, ch);
    reset();
}

void SaturationModule::reset()
{
    for (auto& v : toneLp) v = 0.0f;
    for (auto& v : presenceLp) v = 0.0f;
    for (auto& v : airLp) v = 0.0f;
    for (auto& v : lowLp) v = 0.0f;
    for (auto& v : dynamicEnv) v = 0.0f;
    for (auto& v : previousOutput) v = 0.0f;

    driveSm.reset (24.0f);
    toneSm.reset (12.0f);
    tapeSm.reset (35.0f);
    tubeSm.reset (28.0f);
    transformerSm.reset (30.0f);
    evenSm.reset (25.0f);
    oddSm.reset (18.0f);
    biasSm.reset (0.0f);
    presenceSm.reset (18.0f);
    airSm.reset (20.0f);
    lowThickSm.reset (12.0f);
    widthSm.reset (100.0f);
    crossSm.reset (3.0f);
    autoGainSm.reset (70.0f);
    mixSm.reset (100.0f);
    outputSm.reset (0.0f);

    bypassFade = 1.0f;
    noiseState = 0x12345678u;
}

float SaturationModule::softLimit (float x) noexcept
{
    return 0.985f * fastTanh (x * 1.18f);
}

float SaturationModule::shapeTape (float x, float drive, float bias) noexcept
{
    const auto d = 1.0f + drive * 3.8f;
    const auto pre = x * d + bias * 0.04f;
    const auto rounded = fastTanh (pre) * (1.0f - 0.08f * drive);
    const auto compressed = rounded / (1.0f + std::abs (rounded) * 0.16f * drive);
    return compressed;
}

float SaturationModule::shapeTube (float x, float drive, float bias) noexcept
{
    const auto d = 1.0f + drive * 4.8f;
    const auto pre = x * d;
    const auto even = pre * pre * (0.08f + 0.22f * drive) * (bias >= 0.0f ? 1.0f : -1.0f);
    const auto odd = pre * pre * pre * (0.015f + 0.055f * drive);
    return fastTanh (pre + even - odd + bias * 0.025f);
}

float SaturationModule::shapeTransformer (float x, float drive) noexcept
{
    const auto d = 1.0f + drive * 2.6f;
    const auto iron = x * d + (x * x * x) * (0.04f + 0.16f * drive);
    return fastTanh (iron) * (1.0f / (1.0f + drive * 0.25f));
}

float SaturationModule::highpassFromLowpass (float x, float& low, float coeff) noexcept
{
    low = (1.0f - coeff) * x + coeff * low;
    return x - low;
}

void SaturationModule::process (juce::AudioBuffer<float>& buffer,
                                juce::AudioProcessorValueTreeState& apvts)
{
    const auto targetOn = getOn (apvts, "saturationOn");
    const auto fadeStep = 1.0f / juce::jmax (1.0f, (float) sampleRate * 0.004f);

    bypassFade += targetOn ? fadeStep : -fadeStep;
    bypassFade = juce::jlimit (0.0f, 1.0f, bypassFade);

    if (! targetOn && bypassFade <= 0.0001f)
        return;

    driveSm.setTarget (getParam (apvts, "satDrive", 24.0f));
    toneSm.setTarget (getParam (apvts, "satTone", 12.0f));
    tapeSm.setTarget (getParam (apvts, "satTape", 35.0f));
    tubeSm.setTarget (getParam (apvts, "satTube", 28.0f));
    transformerSm.setTarget (getParam (apvts, "satTransformer", 30.0f));
    evenSm.setTarget (getParam (apvts, "satEven", 25.0f));
    oddSm.setTarget (getParam (apvts, "satOdd", 18.0f));
    biasSm.setTarget (getParam (apvts, "satBias", 0.0f));
    presenceSm.setTarget (getParam (apvts, "satPresence", 18.0f));
    airSm.setTarget (getParam (apvts, "satAir", 20.0f));
    lowThickSm.setTarget (getParam (apvts, "satLowThick", 12.0f));
    widthSm.setTarget (getParam (apvts, "satWidth", 100.0f));
    crossSm.setTarget (getParam (apvts, "satCrosstalk", 3.0f));
    autoGainSm.setTarget (getParam (apvts, "satAutoGain", 70.0f));
    mixSm.setTarget (getParam (apvts, "satMix", 100.0f));
    outputSm.setTarget (getParam (apvts, "satOutput", 0.0f));

    const auto smoothCoeff = smoothAmountForBlock (sampleRate);
    const auto presenceCoeff = freqToOnePoleCoeff (2400.0f);
    const auto airCoeff = freqToOnePoleCoeff (9000.0f);
    const auto lowCoeff = freqToOnePoleCoeff (180.0f);
    const auto envAttack = msToCoeff (4.0f);
    const auto envRelease = msToCoeff (90.0f);

    const bool stereo = buffer.getNumChannels() > 1;

    for (int n = 0; n < buffer.getNumSamples(); ++n)
    {
        const auto drive = driveSm.next (smoothCoeff) / 100.0f;
        const auto tone = toneSm.next (smoothCoeff) / 100.0f;
        const auto tape = tapeSm.next (smoothCoeff) / 100.0f;
        const auto tube = tubeSm.next (smoothCoeff) / 100.0f;
        const auto transformer = transformerSm.next (smoothCoeff) / 100.0f;
        const auto even = evenSm.next (smoothCoeff) / 100.0f;
        const auto odd = oddSm.next (smoothCoeff) / 100.0f;
        const auto bias = biasSm.next (smoothCoeff) / 100.0f;
        const auto presence = presenceSm.next (smoothCoeff) / 100.0f;
        const auto air = airSm.next (smoothCoeff) / 100.0f;
        const auto lowThick = lowThickSm.next (smoothCoeff) / 100.0f;
        const auto width = widthSm.next (smoothCoeff) / 100.0f;
        const auto crosstalk = crossSm.next (smoothCoeff) / 100.0f;
        const auto autoGain = autoGainSm.next (smoothCoeff) / 100.0f;
        const auto mix = mixSm.next (smoothCoeff) / 100.0f;
        const auto outputGain = dbToGainSafe (outputSm.next (smoothCoeff));

        float dryL = buffer.getSample (0, n);
        float dryR = stereo ? buffer.getSample (1, n) : dryL;

        auto processOne = [&] (float dry, int idx) -> float
        {
            const auto absIn = std::abs (dry);
            const auto envCoeff = absIn > dynamicEnv[idx] ? envAttack : envRelease;
            dynamicEnv[idx] = envCoeff * dynamicEnv[idx] + (1.0f - envCoeff) * absIn;

            const auto dynamicDrive = drive * (1.0f + juce::jlimit (0.0f, 1.0f, dynamicEnv[idx] * 1.8f));
            const auto inputGain = 1.0f + dynamicDrive * 3.5f;
            auto x = dry * inputGain;

            const auto iron = shapeTransformer (x, transformer * dynamicDrive);
            x = x + (iron - x) * transformer;

            const auto tapeOut = shapeTape (x, tape * dynamicDrive, bias);
            x = x + (tapeOut - x) * tape;

            const auto tubeOut = shapeTube (x, tube * dynamicDrive, bias);
            x = x + (tubeOut - x) * tube;

            const auto harmonicEven = x * x * (0.02f + 0.16f * even) * (bias >= 0.0f ? 1.0f : -1.0f);
            const auto harmonicOdd = x * x * x * (0.015f + 0.13f * odd);
            x = x + harmonicEven - harmonicOdd;

            const auto p = highpassFromLowpass (x, presenceLp[idx], presenceCoeff);
            const auto a = highpassFromLowpass (x, airLp[idx], airCoeff);
            const auto low = (1.0f - lowCoeff) * x + lowCoeff * lowLp[idx];
            lowLp[idx] = low;

            x += p * presence * 0.28f;
            x += a * air * 0.22f;
            x += low * lowThick * 0.20f;

            const auto toneCoeff = freqToOnePoleCoeff (juce::jmap (tone, 900.0f, 12000.0f));
            toneLp[idx] = (1.0f - toneCoeff) * x + toneCoeff * toneLp[idx];
            x = toneLp[idx] + (x - toneLp[idx]) * (0.55f + tone * 0.85f);

            const auto compensation = 1.0f / (1.0f + autoGain * (drive * 1.5f + tape * 0.30f + tube * 0.35f + transformer * 0.25f));
            x = softLimit (x * compensation * outputGain);

            previousOutput[idx] = x;
            return dry + (x - dry) * mix * bypassFade;
        };

        auto wetL = processOne (dryL, 0);
        auto wetR = processOne (dryR, 1);

        if (stereo)
        {
            const auto mid = (wetL + wetR) * 0.5f;
            const auto side = (wetL - wetR) * 0.5f * width;
            wetL = mid + side;
            wetR = mid - side;

            const auto cross = juce::jlimit (0.0f, 0.18f, crosstalk * 0.18f);
            const auto l = wetL;
            const auto r = wetR;
            wetL = l * (1.0f - cross) + r * cross;
            wetR = r * (1.0f - cross) + l * cross;
        }

        buffer.setSample (0, n, softLimit (wetL));
        if (stereo)
            buffer.setSample (1, n, softLimit (wetR));
    }
}
