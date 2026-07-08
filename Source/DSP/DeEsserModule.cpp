#include "DeEsserModule.h"

void DeEsserModule::prepare (double sr, int bs, int ch)
{
    DspModuleBase::prepare (sr, bs, ch);
    maxDelaySamples = juce::jmax (1, (int) std::ceil (0.020 * sampleRate));
    ensureDelaySize();
    reset();
}

void DeEsserModule::ensureDelaySize()
{
    for (auto& s : state)
    {
        if ((int) s.delay.size() != maxDelaySamples + 1)
            s.delay.assign ((size_t) maxDelaySamples + 1, 0.0f);

        s.delayWrite = 0;
    }
}

void DeEsserModule::reset()
{
    for (auto& s : state)
    {
        s.sideHp = 0.0f;
        s.sideLp = 0.0f;
        s.splitLp = 0.0f;
        s.peakEnv = 0.0f;
        s.rmsEnv = 0.0f;
        s.noiseFloor = 0.0001f;
        s.gain = 1.0f;
        s.holdSamples = 0.0f;
        s.delayWrite = 0;
        std::fill (s.delay.begin(), s.delay.end(), 0.0f);
    }
}

float DeEsserModule::safeMix (float value) noexcept
{
    return juce::jlimit (0.0f, 1.0f, value * 0.01f);
}

float DeEsserModule::softKneeOverDb (float inputDb, float thresholdDb, float kneeDb) noexcept
{
    const auto knee = juce::jmax (0.1f, kneeDb);
    const auto x = inputDb - thresholdDb;

    if (x <= -0.5f * knee)
        return 0.0f;

    if (x >= 0.5f * knee)
        return x;

    const auto y = x + 0.5f * knee;
    return (y * y) / (2.0f * knee);
}

void DeEsserModule::process (juce::AudioBuffer<float>& buffer,
                             juce::AudioProcessorValueTreeState& apvts)
{
    if (! getOn (apvts, "deesserOn"))
        return;

    const auto freq = getParam (apvts, "deessFreq", 6200.0f);
    const auto thresholdDb = getParam (apvts, "deessThreshold", -30.0f);
    const auto rangeDb = getParam (apvts, "deessRange", -10.0f);
    const auto ratio = getParam (apvts, "deessRatio", 4.0f);
    const auto attackMs = getParam (apvts, "deessAttack", 0.7f);
    const auto holdMs = getParam (apvts, "deessHold", 12.0f);
    const auto releaseMs = getParam (apvts, "deessRelease", 75.0f);
    const auto lookaheadMs = getParam (apvts, "deessLookahead", 2.0f);
    const auto kneeDb = getParam (apvts, "deessKnee", 7.0f);
    const auto focus = safeMix (getParam (apvts, "deessFocus", 65.0f));
    const auto peakMix = safeMix (getParam (apvts, "deessPeakMix", 55.0f));
    const auto adaptive = safeMix (getParam (apvts, "deessAdaptive", 35.0f));
    const auto splitMix = safeMix (getParam (apvts, "deessSplit", 80.0f));
    const auto stereoLink = safeMix (getParam (apvts, "deessStereoLink", 70.0f));
    const auto makeup = safeMix (getParam (apvts, "deessAutoMakeup", 10.0f));
    const auto outputDb = getParam (apvts, "deessOutput", 0.0f);

    const auto sideHpCoeff = freqToOnePoleCoeff (juce::jmax (800.0f, freq * (0.42f + 0.36f * focus)));
    const auto sideLpCoeff = freqToOnePoleCoeff (juce::jmin ((float) sampleRate * 0.45f, freq * (1.55f + 1.10f * focus)));
    const auto splitCoeff = freqToOnePoleCoeff (freq);

    const auto attackCoeff = msToCoeff (attackMs);
    const auto releaseCoeff = msToCoeff (releaseMs);
    const auto peakReleaseCoeff = msToCoeff (35.0f);
    const auto rmsCoeff = msToCoeff (8.0f + releaseMs * 0.10f);
    const auto noiseCoeff = msToCoeff (1200.0f);
    const auto holdSamplesTarget = holdMs * 0.001f * (float) sampleRate;

    const auto minGain = dbToGain (juce::jlimit (-30.0f, 0.0f, rangeDb));
    const auto outputGain = dbToGain (outputDb);
    const int delaySamples = juce::jlimit (0, maxDelaySamples - 1,
                                           (int) std::round (lookaheadMs * 0.001f * sampleRate));

    float linkedGain[2] { 1.0f, 1.0f };

    for (int n = 0; n < buffer.getNumSamples(); ++n)
    {
        float channelTarget[2] { 1.0f, 1.0f };

        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        {
            auto& s = state[(size_t) (ch % 2)];
            const auto input = buffer.getSample (ch, n);

            s.sideHp = (1.0f - sideHpCoeff) * input + sideHpCoeff * s.sideHp;
            const auto highPassed = input - s.sideHp;
            s.sideLp = (1.0f - sideLpCoeff) * highPassed + sideLpCoeff * s.sideLp;
            const auto side = s.sideLp;

            const auto absSide = std::abs (side);
            s.peakEnv = juce::jmax (absSide, peakReleaseCoeff * s.peakEnv);
            s.rmsEnv = (1.0f - rmsCoeff) * (side * side) + rmsCoeff * s.rmsEnv;

            const auto rms = std::sqrt (juce::jmax (0.0f, s.rmsEnv));
            const auto detector = peakMix * s.peakEnv + (1.0f - peakMix) * rms;

            if (gainToDb (detector) < thresholdDb - 8.0f)
                s.noiseFloor = (1.0f - noiseCoeff) * detector + noiseCoeff * s.noiseFloor;

            const auto adaptiveThreshold = thresholdDb + adaptive * juce::jlimit (-10.0f, 8.0f, gainToDb (s.noiseFloor) + 42.0f);
            const auto detectorDb = gainToDb (detector);
            const auto overDb = softKneeOverDb (detectorDb, adaptiveThreshold, kneeDb);
            const auto reductionDb = juce::jlimit (rangeDb, 0.0f, -overDb * (1.0f - 1.0f / juce::jmax (1.01f, ratio)));
            channelTarget[ch % 2] = juce::jmax (minGain, dbToGain (reductionDb));
        }

        if (buffer.getNumChannels() > 1)
        {
            const auto minTarget = juce::jmin (channelTarget[0], channelTarget[1]);
            linkedGain[0] = channelTarget[0] * (1.0f - stereoLink) + minTarget * stereoLink;
            linkedGain[1] = channelTarget[1] * (1.0f - stereoLink) + minTarget * stereoLink;
        }
        else
        {
            linkedGain[0] = channelTarget[0];
            linkedGain[1] = channelTarget[0];
        }

        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        {
            auto& s = state[(size_t) (ch % 2)];
            const auto input = buffer.getSample (ch, n);
            auto target = linkedGain[ch % 2];

            if (target < s.gain)
            {
                s.gain = target + attackCoeff * (s.gain - target);
                s.holdSamples = holdSamplesTarget;
            }
            else if (s.holdSamples > 0.0f)
            {
                s.holdSamples -= 1.0f;
            }
            else
            {
                s.gain = target + releaseCoeff * (s.gain - target);
            }

            s.splitLp = (1.0f - splitCoeff) * input + splitCoeff * s.splitLp;
            const auto high = input - s.splitLp;
            const auto splitProcessed = s.splitLp + high * s.gain;
            const auto wideProcessed = input * s.gain;
            const auto deessed = splitProcessed * splitMix + wideProcessed * (1.0f - splitMix);

            const auto compensated = deessed * (1.0f + (1.0f - s.gain) * 0.35f * makeup);

            s.delay[(size_t) s.delayWrite] = compensated;
            int read = s.delayWrite - delaySamples;
            if (read < 0)
                read += (int) s.delay.size();

            const auto delayed = s.delay[(size_t) read];
            s.delayWrite = (s.delayWrite + 1) % (int) s.delay.size();

            buffer.setSample (ch, n, juce::jlimit (-1.25f, 1.25f, delayed * outputGain));
        }
    }
}
