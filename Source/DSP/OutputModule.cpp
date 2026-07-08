#include "OutputModule.h"

void OutputModule::prepare (double sr, int bs, int ch)
{
    DspModuleBase::prepare (sr, bs, ch);
    reset();
}

void OutputModule::reset()
{
    for (auto& s : state)
    {
        s = {};
        s.env = 1.0f;
        s.gain = 1.0f;
    }

    outputTrimSmoothed = 1.0f;
    ceilingSmoothed = dbToGain (-1.0f);
    thresholdSmoothed = dbToGain (-2.0f);
    lufsGainSmoothed = 1.0f;
    mixSmoothed = 1.0f;
    lastGainReductionDb = 0.0f;
    lastRms = 0.0f;
    lastCorrelation = 0.0f;
    noiseSeed = 0x12345678u;
}

float OutputModule::processDcBlock (float x, ChannelState& s) const noexcept
{
    constexpr float r = 0.995f;
    const float y = x - s.dcX1 + r * s.dcY1;
    s.dcX1 = x;
    s.dcY1 = y;
    return y;
}

float OutputModule::softClip (float x, float amount) const noexcept
{
    const float drive = 1.0f + juce::jlimit (0.0f, 1.0f, amount) * 2.5f;
    const float y = std::tanh (x * drive) / std::tanh (drive);
    return juce::jlimit (-1.25f, 1.25f, y);
}

float OutputModule::nextNoise() noexcept
{
    noiseSeed = noiseSeed * 1664525u + 1013904223u;
    const float a = (float) ((noiseSeed >> 8) & 0x00ffffff) / 16777216.0f;
    noiseSeed = noiseSeed * 1664525u + 1013904223u;
    const float b = (float) ((noiseSeed >> 8) & 0x00ffffff) / 16777216.0f;
    return a - b;
}

void OutputModule::process (juce::AudioBuffer<float>& buffer,
                            juce::AudioProcessorValueTreeState& apvts)
{
    if (! getOn (apvts, "outputOn"))
        return;

    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();
    if (numChannels <= 0 || numSamples <= 0)
        return;

    const float outputTrimDb = getParam (apvts, "outTrim", 0.0f);
    const float ceilingDb = getParam (apvts, "outCeiling", -1.0f);
    const float thresholdDb = getParam (apvts, "outThreshold", -2.0f);
    const float softClipAmt = getParam (apvts, "outSoftClip", 20.0f) * 0.01f;
    const float ispProtect = getParam (apvts, "outISP", 100.0f) * 0.01f;
    const float lufsCompDb = getParam (apvts, "outLufsComp", 0.0f);
    const float ditherAmt = getParam (apvts, "outDither", 0.0f) * 0.01f;
    const float releaseMs = getParam (apvts, "outRelease", 60.0f);
    const float mixTarget = getParam (apvts, "outMix", 100.0f) * 0.01f;

    const float smoothCoeff = msToCoeff (8.0f);
    const float releaseCoeff = msToCoeff (releaseMs);
    const float attackCoeff = msToCoeff (0.12f);

    const float trimTarget = dbToGain (outputTrimDb);
    const float ceilingTarget = dbToGain (ceilingDb);
    const float thresholdTarget = dbToGain (thresholdDb);
    const float lufsTarget = dbToGain (lufsCompDb * 0.50f);

    float maxReduction = 1.0f;
    double sumSq = 0.0;
    double sumL = 0.0;
    double sumR = 0.0;
    double sumLR = 0.0;

    for (int n = 0; n < numSamples; ++n)
    {
        outputTrimSmoothed = trimTarget + smoothCoeff * (outputTrimSmoothed - trimTarget);
        ceilingSmoothed = ceilingTarget + smoothCoeff * (ceilingSmoothed - ceilingTarget);
        thresholdSmoothed = thresholdTarget + smoothCoeff * (thresholdSmoothed - thresholdTarget);
        lufsGainSmoothed = lufsTarget + smoothCoeff * (lufsGainSmoothed - lufsTarget);
        mixSmoothed = mixTarget + smoothCoeff * (mixSmoothed - mixTarget);

        float peak = 0.0f;
        float dry[2] { 0.0f, 0.0f };
        float wet[2] { 0.0f, 0.0f };

        for (int ch = 0; ch < numChannels; ++ch)
        {
            auto* data = buffer.getWritePointer (ch);
            const int idx = ch % 2;
            dry[idx] = data[n];

            float v = dry[idx] * outputTrimSmoothed * lufsGainSmoothed;
            v = processDcBlock (v, state[idx]);
            v = softClip (v, softClipAmt);

            wet[idx] = v;
            peak = juce::jmax (peak, std::abs (v));
        }

        const float ispMargin = juce::jmap (ispProtect, 0.0f, 1.0f, 1.0f, 0.92f);
        const float limiterCeiling = ceilingSmoothed * ispMargin;
        float targetGain = 1.0f;

        if (peak > thresholdSmoothed && peak > 1.0e-6f)
            targetGain = juce::jmin (1.0f, limiterCeiling / peak);

        for (int ch = 0; ch < numChannels; ++ch)
        {
            auto* data = buffer.getWritePointer (ch);
            const int idx = ch % 2;

            state[idx].env = targetGain < state[idx].env || state[idx].env <= 0.0f
                           ? targetGain + attackCoeff * (state[idx].env - targetGain)
                           : targetGain + releaseCoeff * (state[idx].env - targetGain);

            state[idx].gain = juce::jlimit (0.001f, 1.0f, state[idx].env);
            maxReduction = juce::jmin (maxReduction, state[idx].gain);

            float v = wet[idx] * state[idx].gain;
            v = juce::jlimit (-limiterCeiling, limiterCeiling, v);

            if (ditherAmt > 0.0f)
                v += nextNoise() * (1.0f / 8388608.0f) * ditherAmt;

            v = dry[idx] + (v - dry[idx]) * mixSmoothed;
            data[n] = juce::jlimit (-1.0f, 1.0f, v);

            sumSq += (double) data[n] * (double) data[n];
        }

        if (numChannels >= 2)
        {
            const float l = buffer.getSample (0, n);
            const float r = buffer.getSample (1, n);
            sumL += (double) l * (double) l;
            sumR += (double) r * (double) r;
            sumLR += (double) l * (double) r;
        }
    }

    const double denom = juce::jmax (1, numSamples * numChannels);
    lastRms = (float) std::sqrt (sumSq / denom);

    if (numChannels >= 2 && sumL > 1.0e-12 && sumR > 1.0e-12)
        lastCorrelation = (float) juce::jlimit (-1.0, 1.0, sumLR / std::sqrt (sumL * sumR));
    else
        lastCorrelation = 1.0f;

    lastGainReductionDb = -gainToDb (maxReduction);
}
