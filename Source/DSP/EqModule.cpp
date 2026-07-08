#include "EqModule.h"

namespace
{
    constexpr float sqrtHalf = 0.70710678118f;

    float smoothTowards (float current, float target, float coeff) noexcept
    {
        return current + (target - current) * coeff;
    }

    float safeQ (float q) noexcept
    {
        return juce::jlimit (0.15f, 8.0f, q);
    }
}

void EqModule::Biquad::reset() noexcept
{
    z1 = 0.0f;
    z2 = 0.0f;
}

float EqModule::Biquad::process (float x) noexcept
{
    const auto y = b0 * x + z1;
    z1 = b1 * x - a1 * y + z2;
    z2 = b2 * x - a2 * y;
    return juce::jlimit (-8.0f, 8.0f, y);
}

void EqModule::Biquad::setBypass() noexcept
{
    b0 = 1.0f; b1 = 0.0f; b2 = 0.0f;
    a1 = 0.0f; a2 = 0.0f;
}

void EqModule::Biquad::setHighPass (float sr, float freq, float q) noexcept
{
    const auto f = juce::jlimit (15.0f, sr * 0.45f, freq);
    const auto omega = 2.0f * juce::MathConstants<float>::pi * f / sr;
    const auto sn = std::sin (omega);
    const auto cs = std::cos (omega);
    const auto alpha = sn / (2.0f * safeQ (q));

    const auto bb0 = (1.0f + cs) * 0.5f;
    const auto bb1 = -(1.0f + cs);
    const auto bb2 = (1.0f + cs) * 0.5f;
    const auto aa0 = 1.0f + alpha;
    const auto aa1 = -2.0f * cs;
    const auto aa2 = 1.0f - alpha;

    b0 = bb0 / aa0; b1 = bb1 / aa0; b2 = bb2 / aa0;
    a1 = aa1 / aa0; a2 = aa2 / aa0;
}

void EqModule::Biquad::setBell (float sr, float freq, float gainDb, float q) noexcept
{
    if (std::abs (gainDb) < 0.005f)
    {
        setBypass();
        return;
    }

    const auto f = juce::jlimit (20.0f, sr * 0.45f, freq);
    const auto omega = 2.0f * juce::MathConstants<float>::pi * f / sr;
    const auto sn = std::sin (omega);
    const auto cs = std::cos (omega);
    const auto A = std::pow (10.0f, gainDb / 40.0f);
    const auto alpha = sn / (2.0f * safeQ (q));

    const auto bb0 = 1.0f + alpha * A;
    const auto bb1 = -2.0f * cs;
    const auto bb2 = 1.0f - alpha * A;
    const auto aa0 = 1.0f + alpha / A;
    const auto aa1 = -2.0f * cs;
    const auto aa2 = 1.0f - alpha / A;

    b0 = bb0 / aa0; b1 = bb1 / aa0; b2 = bb2 / aa0;
    a1 = aa1 / aa0; a2 = aa2 / aa0;
}

void EqModule::Biquad::setLowShelf (float sr, float freq, float gainDb, float slope) noexcept
{
    if (std::abs (gainDb) < 0.005f)
    {
        setBypass();
        return;
    }

    const auto f = juce::jlimit (20.0f, sr * 0.45f, freq);
    const auto omega = 2.0f * juce::MathConstants<float>::pi * f / sr;
    const auto sn = std::sin (omega);
    const auto cs = std::cos (omega);
    const auto A = std::pow (10.0f, gainDb / 40.0f);
    const auto beta = std::sqrt (A) / juce::jmax (0.1f, slope);

    const auto bb0 = A * ((A + 1.0f) - (A - 1.0f) * cs + 2.0f * beta * sn);
    const auto bb1 = 2.0f * A * ((A - 1.0f) - (A + 1.0f) * cs);
    const auto bb2 = A * ((A + 1.0f) - (A - 1.0f) * cs - 2.0f * beta * sn);
    const auto aa0 = (A + 1.0f) + (A - 1.0f) * cs + 2.0f * beta * sn;
    const auto aa1 = -2.0f * ((A - 1.0f) + (A + 1.0f) * cs);
    const auto aa2 = (A + 1.0f) + (A - 1.0f) * cs - 2.0f * beta * sn;

    b0 = bb0 / aa0; b1 = bb1 / aa0; b2 = bb2 / aa0;
    a1 = aa1 / aa0; a2 = aa2 / aa0;
}

void EqModule::Biquad::setHighShelf (float sr, float freq, float gainDb, float slope) noexcept
{
    if (std::abs (gainDb) < 0.005f)
    {
        setBypass();
        return;
    }

    const auto f = juce::jlimit (20.0f, sr * 0.45f, freq);
    const auto omega = 2.0f * juce::MathConstants<float>::pi * f / sr;
    const auto sn = std::sin (omega);
    const auto cs = std::cos (omega);
    const auto A = std::pow (10.0f, gainDb / 40.0f);
    const auto beta = std::sqrt (A) / juce::jmax (0.1f, slope);

    const auto bb0 = A * ((A + 1.0f) + (A - 1.0f) * cs + 2.0f * beta * sn);
    const auto bb1 = -2.0f * A * ((A - 1.0f) + (A + 1.0f) * cs);
    const auto bb2 = A * ((A + 1.0f) + (A - 1.0f) * cs - 2.0f * beta * sn);
    const auto aa0 = (A + 1.0f) - (A - 1.0f) * cs + 2.0f * beta * sn;
    const auto aa1 = 2.0f * ((A - 1.0f) - (A + 1.0f) * cs);
    const auto aa2 = (A + 1.0f) - (A - 1.0f) * cs - 2.0f * beta * sn;

    b0 = bb0 / aa0; b1 = bb1 / aa0; b2 = bb2 / aa0;
    a1 = aa1 / aa0; a2 = aa2 / aa0;
}

void EqModule::ChannelEq::reset() noexcept
{
    hpf.reset(); lowShelf.reset(); lowMid.reset(); mid.reset(); highMid.reset(); highShelf.reset(); air.reset();
    dynEnv = 0.0f;
    makeupEnv = 1.0f;
}

void EqModule::prepare (double sr, int bs, int ch)
{
    DspModuleBase::prepare (sr, bs, ch);
    reset();
}

void EqModule::reset()
{
    for (auto& c : lr) c.reset();
    for (auto& c : ms) c.reset();
}

void EqModule::updateFilters (ChannelEq& c,
                              float hpfFreq,
                              float lowFreq, float lowGain,
                              float lowMidFreq, float lowMidGain, float lowMidQ,
                              float midFreq, float midGain, float midQ,
                              float highMidFreq, float highMidGain, float highMidQ,
                              float highFreq, float highGain,
                              float airFreq, float airGain) noexcept
{
    const auto sr = (float) sampleRate;
    c.hpf.setHighPass (sr, hpfFreq, sqrtHalf);
    c.lowShelf.setLowShelf (sr, lowFreq, lowGain, 0.80f);
    c.lowMid.setBell (sr, lowMidFreq, lowMidGain, lowMidQ);
    c.mid.setBell (sr, midFreq, midGain, midQ);
    c.highMid.setBell (sr, highMidFreq, highMidGain, highMidQ);
    c.highShelf.setHighShelf (sr, highFreq, highGain, 0.80f);
    c.air.setHighShelf (sr, airFreq, airGain, 0.65f);
}

float EqModule::processEqSample (ChannelEq& c, float x, float dynAmount, float dynThresholdDb) noexcept
{
    auto y = c.hpf.process (x);
    y = c.lowShelf.process (y);
    y = c.lowMid.process (y);
    y = c.mid.process (y);

    const auto detector = std::abs (y);
    const auto attack = 1.0f - std::exp (-1.0f / (0.006f * (float) sampleRate));
    const auto release = 1.0f - std::exp (-1.0f / (0.090f * (float) sampleRate));
    c.dynEnv += ((detector > c.dynEnv ? attack : release) * (detector - c.dynEnv));

    const auto dynDb = gainToDb (c.dynEnv);
    const auto over = juce::jmax (0.0f, dynDb - dynThresholdDb);
    const auto dynReduce = juce::Decibels::decibelsToGain (-over * dynAmount * 0.06f);

    y = c.highMid.process (y) * dynReduce;
    y = c.highShelf.process (y);
    y = c.air.process (y);
    return juce::jlimit (-4.0f, 4.0f, y);
}

void EqModule::process (juce::AudioBuffer<float>& buffer,
                        juce::AudioProcessorValueTreeState& apvts)
{
    if (! getOn (apvts, "eqOn"))
        return;

    const auto smooth = juce::jlimit (0.001f, 0.25f, 64.0f / (float) juce::jmax (1.0, sampleRate));

    const auto targetHpf = getParam (apvts, "eqHpfFreq", 70.0f);
    const auto targetLow = getParam (apvts, "eqLowGain", getParam (apvts, "eqLow", 0.0f));
    const auto targetLowMid = getParam (apvts, "eqLowMidGain", 0.0f);
    const auto targetMid = getParam (apvts, "eqMidGain", getParam (apvts, "eqMid", 0.0f));
    const auto targetHighMid = getParam (apvts, "eqHighMidGain", 0.0f);
    const auto targetHigh = getParam (apvts, "eqHighGain", getParam (apvts, "eqHigh", 0.0f));
    const auto targetAir = getParam (apvts, "eqAirGain", getParam (apvts, "eqAir", 0.0f));
    const auto targetOutput = dbToGain (getParam (apvts, "eqOutput", 0.0f));

    smoothHpf = smoothTowards (smoothHpf, targetHpf, smooth);
    smoothLowGain = smoothTowards (smoothLowGain, targetLow, smooth);
    smoothLowMidGain = smoothTowards (smoothLowMidGain, targetLowMid, smooth);
    smoothMidGain = smoothTowards (smoothMidGain, targetMid, smooth);
    smoothHighMidGain = smoothTowards (smoothHighMidGain, targetHighMid, smooth);
    smoothHighGain = smoothTowards (smoothHighGain, targetHigh, smooth);
    smoothAirGain = smoothTowards (smoothAirGain, targetAir, smooth);
    smoothOutput = smoothTowards (smoothOutput, targetOutput, smooth);

    const auto lowFreq = getParam (apvts, "eqLowFreq", 120.0f);
    const auto lowMidFreq = getParam (apvts, "eqLowMidFreq", 320.0f);
    const auto lowMidQ = getParam (apvts, "eqLowMidQ", 1.0f);
    const auto midFreq = getParam (apvts, "eqMidFreq", 1200.0f);
    const auto midQ = getParam (apvts, "eqMidQ", 1.0f);
    const auto highMidFreq = getParam (apvts, "eqHighMidFreq", 3800.0f);
    const auto highMidQ = getParam (apvts, "eqHighMidQ", 1.1f);
    const auto highFreq = getParam (apvts, "eqHighFreq", 8500.0f);
    const auto airFreq = getParam (apvts, "eqAirFreq", 14000.0f);
    const auto dynAmount = getParam (apvts, "eqDynAmount", 20.0f) * 0.01f;
    const auto dynThreshold = getParam (apvts, "eqDynThreshold", -20.0f);
    const auto msAmount = getParam (apvts, "eqMS", 0.0f) * 0.01f;
    const auto autoGain = getParam (apvts, "eqAutoGain", 70.0f) * 0.01f;

    const auto gainSum = std::abs (smoothLowGain) + std::abs (smoothLowMidGain) + std::abs (smoothMidGain)
                       + std::abs (smoothHighMidGain) + std::abs (smoothHighGain) + std::abs (smoothAirGain);
    const auto autoComp = juce::Decibels::decibelsToGain (-gainSum * 0.035f * autoGain);

    for (auto& c : lr)
        updateFilters (c, smoothHpf, lowFreq, smoothLowGain, lowMidFreq, smoothLowMidGain, lowMidQ,
                       midFreq, smoothMidGain, midQ, highMidFreq, smoothHighMidGain, highMidQ,
                       highFreq, smoothHighGain, airFreq, smoothAirGain);

    for (auto& c : ms)
        updateFilters (c, smoothHpf, lowFreq, smoothLowGain, lowMidFreq, smoothLowMidGain, lowMidQ,
                       midFreq, smoothMidGain, midQ, highMidFreq, smoothHighMidGain, highMidQ,
                       highFreq, smoothHighGain, airFreq, smoothAirGain * (1.0f + msAmount * 0.35f));

    const auto numSamples = buffer.getNumSamples();
    const auto numChannels = buffer.getNumChannels();

    if (numChannels >= 2 && msAmount > 0.001f)
    {
        auto* left = buffer.getWritePointer (0);
        auto* right = buffer.getWritePointer (1);

        for (int n = 0; n < numSamples; ++n)
        {
            const auto l = left[n];
            const auto r = right[n];
            auto mid = (l + r) * 0.5f;
            auto side = (l - r) * 0.5f;

            mid = processEqSample (ms[0], mid, dynAmount, dynThreshold);
            side = processEqSample (ms[1], side, dynAmount * 0.75f, dynThreshold + 3.0f);
            side *= (1.0f + msAmount * 0.12f);

            const auto msL = mid + side;
            const auto msR = mid - side;
            const auto lrL = processEqSample (lr[0], l, dynAmount, dynThreshold);
            const auto lrR = processEqSample (lr[1], r, dynAmount, dynThreshold);

            left[n] = ((lrL * (1.0f - msAmount)) + (msL * msAmount)) * smoothOutput * autoComp;
            right[n] = ((lrR * (1.0f - msAmount)) + (msR * msAmount)) * smoothOutput * autoComp;
        }
    }
    else
    {
        for (int ch = 0; ch < numChannels; ++ch)
        {
            auto* x = buffer.getWritePointer (ch);
            auto& c = lr[(size_t) (ch % 2)];

            for (int n = 0; n < numSamples; ++n)
                x[n] = processEqSample (c, x[n], dynAmount, dynThreshold) * smoothOutput * autoComp;
        }
    }
}
