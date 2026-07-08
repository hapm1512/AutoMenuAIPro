#include "PreampModule.h"

void PreampModule::prepare (double sr, int bs, int ch)
{
    DspModuleBase::prepare (sr, bs, ch);

    const auto smoothTime = 0.025;

    inputTrimGain.reset (sampleRate, smoothTime);
    driveGain.reset (sampleRate, smoothTime);
    outputGain.reset (sampleRate, smoothTime);
    headroomDb.reset (sampleRate, smoothTime);
    transformerAmount.reset (sampleRate, smoothTime);
    tubeAmount.reset (sampleRate, smoothTime);
    harmonicAmount.reset (sampleRate, smoothTime);
    biasAmount.reset (sampleRate, smoothTime);
    memoryAmount.reset (sampleRate, smoothTime);
    stereoVarianceAmount.reset (sampleRate, smoothTime);
    crosstalkAmount.reset (sampleRate, smoothTime);
    noiseAmount.reset (sampleRate, smoothTime);
    autoGainAmount.reset (sampleRate, smoothTime);
    peakProtectAmount.reset (sampleRate, smoothTime);
    mixAmount.reset (sampleRate, smoothTime);

    states.resize ((size_t) juce::jmax (1, ch));
    dryBuffer.setSize (juce::jmax (1, ch), juce::jmax (1, bs), false, false, true);

    reset();
}

void PreampModule::reset()
{
    for (auto& s : states)
        s = {};

    inputTrimGain.setCurrentAndTargetValue (1.0f);
    driveGain.setCurrentAndTargetValue (1.0f);
    outputGain.setCurrentAndTargetValue (1.0f);
    headroomDb.setCurrentAndTargetValue (12.0f);
    transformerAmount.setCurrentAndTargetValue (0.35f);
    tubeAmount.setCurrentAndTargetValue (0.30f);
    harmonicAmount.setCurrentAndTargetValue (0.35f);
    biasAmount.setCurrentAndTargetValue (0.12f);
    memoryAmount.setCurrentAndTargetValue (0.20f);
    stereoVarianceAmount.setCurrentAndTargetValue (0.10f);
    crosstalkAmount.setCurrentAndTargetValue (0.04f);
    noiseAmount.setCurrentAndTargetValue (0.0f);
    autoGainAmount.setCurrentAndTargetValue (0.70f);
    peakProtectAmount.setCurrentAndTargetValue (1.0f);
    mixAmount.setCurrentAndTargetValue (1.0f);
}

void PreampModule::process (juce::AudioBuffer<float>& buffer,
                            juce::AudioProcessorValueTreeState& apvts)
{
    juce::ScopedNoDenormals noDenormals;

    if (buffer.getNumSamples() <= 0 || buffer.getNumChannels() <= 0)
        return;

    const bool enabled = getOn (apvts, "preampOn");

    dryBuffer.setSize (buffer.getNumChannels(), buffer.getNumSamples(), false, false, true);
    dryBuffer.makeCopyOf (buffer, true);

    if (! enabled)
        return;

    const auto trimDb = getParam (apvts, "preampInputTrim", 0.0f);
    const auto drive = getParam (apvts, "preampDrive", 23.0f);
    const auto outDb = getParam (apvts, "preampOutput", 0.0f);
    const auto headroom = getParam (apvts, "preampHeadroom", 12.0f);
    const auto transformer = getParam (apvts, "preampTransformer", 35.0f);
    const auto tube = getParam (apvts, "preampTube", 30.0f);
    const auto harm = getParam (apvts, "preampHarmonic", 35.0f);
    const auto bias = getParam (apvts, "preampBias", 12.0f);
    const auto memory = getParam (apvts, "preampMemory", 20.0f);
    const auto variance = getParam (apvts, "preampStereo", 10.0f);
    const auto crosstalk = getParam (apvts, "preampCrosstalk", 4.0f);
    const auto noise = getParam (apvts, "preampNoise", 0.0f);
    const auto autoGain = getParam (apvts, "preampAutoGain", 70.0f);
    const auto peakProtect = getParam (apvts, "preampPeakProtect", 100.0f);
    const auto mix = getParam (apvts, "preampMix", 100.0f);

    inputTrimGain.setTargetValue (dbToGain (trimDb));
    driveGain.setTargetValue (dbToGain (drive * 0.13f));
    outputGain.setTargetValue (dbToGain (outDb - drive * 0.035f));
    headroomDb.setTargetValue (juce::jlimit (3.0f, 24.0f, headroom));
    transformerAmount.setTargetValue (juce::jlimit (0.0f, 1.0f, transformer / 100.0f));
    tubeAmount.setTargetValue (juce::jlimit (0.0f, 1.0f, tube / 100.0f));
    harmonicAmount.setTargetValue (juce::jlimit (0.0f, 1.0f, harm / 100.0f));
    biasAmount.setTargetValue (juce::jlimit (-1.0f, 1.0f, bias / 100.0f));
    memoryAmount.setTargetValue (juce::jlimit (0.0f, 1.0f, memory / 100.0f));
    stereoVarianceAmount.setTargetValue (juce::jlimit (0.0f, 1.0f, variance / 100.0f));
    crosstalkAmount.setTargetValue (juce::jlimit (0.0f, 1.0f, crosstalk / 100.0f));
    noiseAmount.setTargetValue (juce::jlimit (0.0f, 1.0f, noise / 100.0f));
    autoGainAmount.setTargetValue (juce::jlimit (0.0f, 1.0f, autoGain / 100.0f));
    peakProtectAmount.setTargetValue (juce::jlimit (0.0f, 1.0f, peakProtect / 100.0f));
    mixAmount.setTargetValue (juce::jlimit (0.0f, 1.0f, mix / 100.0f));

    for (int n = 0; n < buffer.getNumSamples(); ++n)
    {
        const auto inTrim = inputTrimGain.getNextValue();
        const auto dg = driveGain.getNextValue();
        const auto og = outputGain.getNextValue();
        const auto hr = headroomDb.getNextValue();
        const auto tr = transformerAmount.getNextValue();
        const auto tb = tubeAmount.getNextValue();
        const auto ha = harmonicAmount.getNextValue();
        const auto bi = biasAmount.getNextValue();
        const auto me = memoryAmount.getNextValue();
        const auto sv = stereoVarianceAmount.getNextValue();
        const auto ct = crosstalkAmount.getNextValue();
        const auto nf = noiseAmount.getNextValue();
        const auto ag = autoGainAmount.getNextValue();
        const auto pp = peakProtectAmount.getNextValue();
        const auto mx = mixAmount.getNextValue();

        float previousProcessed = 0.0f;

        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        {
            auto* x = buffer.getWritePointer (ch);
            const auto dry = dryBuffer.getSample (ch, n);

            auto y = processOneSample (dry, ch, previousProcessed,
                                       inTrim, dg, og, hr, tr, tb, ha, bi, me,
                                       sv, ct, nf, ag, pp);

            previousProcessed = y;
            x[n] = dry + (y - dry) * mx;
        }
    }
}

float PreampModule::processOneSample (float input, int channel, float previousChannelSample,
                                      float inTrim, float drive, float outGain, float headroom,
                                      float transformer, float tube, float harmonic, float bias,
                                      float memory, float variance, float crosstalk, float noise,
                                      float autoGain, float peakProtect) noexcept
{
    auto& s = states[(size_t) juce::jlimit (0, (int) states.size() - 1, channel)];

    const auto dcR = 0.995f;
    auto x = input - s.dcX1 + dcR * s.dcY1;
    s.dcX1 = input;
    s.dcY1 = x;

    const auto varianceGain = (channel & 1) == 0 ? (1.0f + 0.018f * variance)
                                                 : (1.0f - 0.014f * variance);
    x *= inTrim * varianceGain;

    const auto headroomGain = dbToGain (-headroom);
    const auto preDrive = x * drive * headroomGain;

    s.memory = 0.9992f * s.memory + 0.0008f * preDrive;
    auto y = preDrive + s.memory * memory * 0.30f;

    y = saturateTransformer (y, transformer);
    y = saturateTube (y, tube, harmonic, bias);

    const auto even = y * y * (0.055f * harmonic);
    const auto odd = y * y * y * (0.030f + 0.045f * harmonic);
    y += even - odd;

    y = softCompressor (y, 0.38f + 0.30f * (1.0f - transformer), 2.0f + tube * 3.5f);
    y = softClip (y);

    const auto aaCoeff = 0.20f + 0.28f * transformer;
    s.lowpass += aaCoeff * (y - s.lowpass);
    y = s.lowpass;

    if (crosstalk > 0.0f && channel > 0)
        y = y * (1.0f - crosstalk * 0.22f) + previousChannelSample * (crosstalk * 0.22f);

    if (noise > 0.0f)
        y += (noiseRandom.nextFloat() * 2.0f - 1.0f) * noise * 0.000035f;

    const auto absY = std::abs (y);
    s.autoGainEnv = 0.9994f * s.autoGainEnv + 0.0006f * juce::jmax (absY, 0.0001f);
    const auto gainTarget = juce::jlimit (0.35f, 1.35f, 0.30f / s.autoGainEnv);
    y *= 1.0f + (gainTarget - 1.0f) * autoGain;

    s.peakEnv = juce::jmax (absY, s.peakEnv * 0.998f);
    const auto ceiling = 0.98f;
    if (peakProtect > 0.0f && std::abs (y) > ceiling)
    {
        const auto protectedSample = std::copysign (ceiling + (1.0f - ceiling) * std::tanh ((std::abs (y) - ceiling) / (1.0f - ceiling)), y);
        y += (protectedSample - y) * peakProtect;
    }

    y *= dbToGain (headroom) * outGain;
    y = juce::jlimit (-1.0f, 1.0f, y);
    s.lastOut = y;
    return y;
}

float PreampModule::saturateTransformer (float x, float amount) noexcept
{
    const auto k = 1.0f + amount * 2.4f;
    const auto lowWeight = 1.0f + 0.08f * amount;
    return std::tanh (x * k) / std::tanh (k) * lowWeight;
}

float PreampModule::saturateTube (float x, float amount, float harmonic, float bias) noexcept
{
    const auto shifted = x + bias * amount * 0.11f;
    const auto k = 1.0f + amount * 3.2f;
    const auto tube = std::tanh (shifted * k) / std::tanh (k);
    const auto asym = tube - bias * amount * 0.045f;
    return x + (asym - x) * (0.18f + 0.62f * amount) * (0.65f + harmonic * 0.35f);
}

float PreampModule::softCompressor (float x, float threshold, float ratio) noexcept
{
    const auto a = std::abs (x);
    if (a <= threshold)
        return x;

    const auto over = a - threshold;
    const auto compressed = threshold + over / juce::jmax (1.0f, ratio);
    return std::copysign (compressed, x);
}

float PreampModule::softClip (float x) noexcept
{
    constexpr float limit = 0.94f;
    const auto a = std::abs (x);
    if (a <= limit)
        return x;

    const auto over = a - limit;
    return std::copysign (limit + (1.0f - limit) * std::tanh (over / (1.0f - limit)), x);
}
