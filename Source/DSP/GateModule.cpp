#include "GateModule.h"

void GateModule::prepare (double sr, int bs, int ch)
{
    DspModuleBase::prepare (sr, bs, ch);
    lookAheadBuffer.setSize (juce::jmax (1, channels), maxLookAheadSamples + juce::jmax (1, bs) + 8);
    reset();
}

void GateModule::reset()
{
    for (auto& v : rmsEnv) v = 0.0f;
    for (auto& v : peakEnv) v = 0.0f;
    for (auto& v : noiseFloorDb) v = -80.0f;
    for (auto& v : gain) v = 1.0f;
    for (auto& v : makeupGain) v = 1.0f;
    for (auto& v : hpState) v = 0.0f;
    for (auto& v : hpPrevIn) v = 0.0f;
    for (auto& v : lpState) v = 0.0f;
    for (auto& v : holdCounter) v = 0;

    smoothedThresholdDb = -42.0f;
    lookAheadWrite = 0;
    lookAheadBuffer.clear();
}

float GateModule::processSideChain (float x, int ch, float hpHz, float lpHz) noexcept
{
    const auto hpCoeff = freqToOnePoleCoeff (hpHz);
    const auto hp = hpCoeff * (hpState[ch] + x - hpPrevIn[ch]);
    hpState[ch] = hp;
    hpPrevIn[ch] = x;

    const auto lpCoeff = freqToOnePoleCoeff (lpHz);
    lpState[ch] = (1.0f - lpCoeff) * hp + lpCoeff * lpState[ch];
    return lpState[ch];
}

float GateModule::computeTargetGain (float detectorDb,
                                     float thresholdDb,
                                     float rangeDb,
                                     float hystDb,
                                     float kneeDb,
                                     float depthPercent,
                                     int channel,
                                     int holdSamples) noexcept
{
    const bool strongOpen = detectorDb >= thresholdDb;
    const bool keepOpen = detectorDb >= (thresholdDb - hystDb);

    if (strongOpen)
        holdCounter[channel] = holdSamples;
    else if (holdCounter[channel] > 0 && keepOpen)
        --holdCounter[channel];
    else if (! keepOpen)
        holdCounter[channel] = 0;

    const bool isOpen = strongOpen || holdCounter[channel] > 0;
    const auto safeKnee = juce::jmax (0.1f, kneeDb);
    const auto kneeStart = thresholdDb - safeKnee;

    float closedAmount = 1.0f;

    if (detectorDb > kneeStart && detectorDb < thresholdDb)
        closedAmount = (thresholdDb - detectorDb) / safeKnee;
    else if (detectorDb >= thresholdDb || isOpen)
        closedAmount = 0.0f;

    const auto depth = juce::jlimit (0.0f, 1.0f, depthPercent * 0.01f);
    const auto closedGain = dbToGain (rangeDb * depth);
    return 1.0f + closedAmount * (closedGain - 1.0f);
}

void GateModule::process (juce::AudioBuffer<float>& buffer,
                          juce::AudioProcessorValueTreeState& apvts)
{
    if (! getOn (apvts, "gateOn"))
        return;

    const auto manualThreshold = getParam (apvts, "gateThreshold", -42.0f);
    const auto rangeDb = getParam (apvts, "gateRange", -24.0f);
    const auto hyst = getParam (apvts, "gateHyst", 6.0f);
    const auto attackMs = getParam (apvts, "gateAttack", 1.0f);
    const auto holdMs = getParam (apvts, "gateHold", 60.0f);
    const auto releaseMs = getParam (apvts, "gateRelease", 120.0f);
    const auto lookAheadMs = getParam (apvts, "gateLookAhead", 2.0f);
    const auto kneeDb = getParam (apvts, "gateKnee", 8.0f);
    const auto depthPercent = getParam (apvts, "gateDepth", 100.0f);
    const auto sideHpHz = getParam (apvts, "gateSideHp", 90.0f);
    const auto sideLpHz = getParam (apvts, "gateSideLp", 9000.0f);
    const auto rmsMix = getParam (apvts, "gateRmsMix", 65.0f) * 0.01f;
    const auto adapt = getParam (apvts, "gateAdaptive", 35.0f) * 0.01f;
    const auto breath = getParam (apvts, "gateBreath", 30.0f) * 0.01f;
    const auto stereoLink = getParam (apvts, "gateStereoLink", 75.0f) * 0.01f;
    const auto autoMakeup = getParam (apvts, "gateAutoMakeup", 15.0f) * 0.01f;

    const auto attackCoeff = msToCoeff (attackMs);
    const auto releaseCoeff = msToCoeff (releaseMs);
    const auto rmsCoeff = msToCoeff (18.0f);
    const auto peakCoeff = msToCoeff (3.0f);
    const auto noiseCoeff = msToCoeff (850.0f);
    const auto thresholdCoeff = msToCoeff (35.0f);
    const auto makeupCoeff = msToCoeff (90.0f);

    const int holdSamples = juce::roundToInt ((holdMs * 0.001f) * (float) sampleRate);
    const int lookAheadSamples = juce::jlimit (0, maxLookAheadSamples - 1,
        juce::roundToInt ((lookAheadMs * 0.001f) * (float) sampleRate));
    const int delaySize = lookAheadBuffer.getNumSamples();

    const int numCh = juce::jmin (buffer.getNumChannels(), channels, maxChannels);
    const int numSamples = buffer.getNumSamples();

    for (int n = 0; n < numSamples; ++n)
    {
        float detectorDb[maxChannels] { -120.0f, -120.0f };

        for (int ch = 0; ch < numCh; ++ch)
        {
            const auto input = buffer.getSample (ch, n);
            lookAheadBuffer.setSample (ch, lookAheadWrite, input);

            const auto sc = processSideChain (input, ch, sideHpHz, sideLpHz);
            const auto absx = std::abs (sc) + 1.0e-9f;

            rmsEnv[ch] = (1.0f - rmsCoeff) * (absx * absx) + rmsCoeff * rmsEnv[ch];
            peakEnv[ch] = absx > peakEnv[ch]
                ? absx
                : (peakCoeff * peakEnv[ch] + (1.0f - peakCoeff) * absx);

            const auto rmsDb = gainToDb (std::sqrt (rmsEnv[ch] + 1.0e-12f));
            const auto peakDb = gainToDb (peakEnv[ch]);
            detectorDb[ch] = peakDb + rmsMix * (rmsDb - peakDb);

            if (detectorDb[ch] < manualThreshold - 3.0f)
                noiseFloorDb[ch] = (1.0f - noiseCoeff) * detectorDb[ch] + noiseCoeff * noiseFloorDb[ch];
        }

        if (numCh > 1)
        {
            const auto linked = juce::jmax (detectorDb[0], detectorDb[1]);
            detectorDb[0] = detectorDb[0] + stereoLink * (linked - detectorDb[0]);
            detectorDb[1] = detectorDb[1] + stereoLink * (linked - detectorDb[1]);
        }

        const auto floorDb = numCh > 1
            ? 0.5f * (noiseFloorDb[0] + noiseFloorDb[1])
            : noiseFloorDb[0];
        const auto autoThreshold = floorDb + 9.0f;
        const auto targetThreshold = juce::jlimit (-80.0f, 0.0f,
            manualThreshold * (1.0f - adapt) + autoThreshold * adapt);
        smoothedThresholdDb = targetThreshold + thresholdCoeff * (smoothedThresholdDb - targetThreshold);

        for (int ch = 0; ch < numCh; ++ch)
        {
            auto targetGain = computeTargetGain (detectorDb[ch], smoothedThresholdDb,
                                                 rangeDb, hyst, kneeDb, depthPercent,
                                                 ch, holdSamples);

            const auto preserve = breath * 0.18f;
            targetGain = juce::jmax (targetGain, preserve);

            const auto coeff = targetGain > gain[ch] ? attackCoeff : releaseCoeff;
            gain[ch] = targetGain + coeff * (gain[ch] - targetGain);

            const auto desiredMakeup = juce::jlimit (1.0f, dbToGain (6.0f),
                1.0f + (1.0f - gain[ch]) * autoMakeup * 0.5f);
            makeupGain[ch] = desiredMakeup + makeupCoeff * (makeupGain[ch] - desiredMakeup);

            const int read = (lookAheadWrite - lookAheadSamples + delaySize) % delaySize;
            auto y = lookAheadBuffer.getSample (ch, read);
            y *= gain[ch] * makeupGain[ch];
            buffer.setSample (ch, n, y);
        }

        lookAheadWrite = (lookAheadWrite + 1) % delaySize;
    }
}
