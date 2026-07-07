#include "ParameterLayout.h"

namespace Parameters
{
    juce::AudioProcessorValueTreeState::ParameterLayout createLayout()
    {
        std::vector<std::unique_ptr<juce::RangedAudioParameter>> p;

        auto addBool = [&] (const juce::String& id, const juce::String& name, bool def)
        {
            p.push_back (std::make_unique<juce::AudioParameterBool>
                (juce::ParameterID { id, 1 }, name, def));
        };

        auto addFloat = [&] (const juce::String& id, const juce::String& name,
                             float min, float max, float def, const juce::String& unit = {})
        {
            p.push_back (std::make_unique<juce::AudioParameterFloat>
                (juce::ParameterID { id, 1 }, name,
                 juce::NormalisableRange<float> (min, max, 0.01f, 1.0f), def, unit));
        };

        addBool ("preampOn", "Preamp On", true);
        addBool ("eqOn", "EQ On", true);
        addBool ("gateOn", "Gate On", true);
        addBool ("deesserOn", "De-Esser On", true);
        addBool ("compressorOn", "Compressor On", true);
        addBool ("saturationOn", "Saturation On", true);
        addBool ("hiendOn", "Hi-End On", true);
        addBool ("outputOn", "Output On", true);

        addFloat ("preampDrive", "Drive", 0.0f, 100.0f, 23.0f, "%");
        addFloat ("preampHarmonic", "Harmonic", 0.0f, 100.0f, 35.0f, "%");
        addFloat ("preampOutput", "Preamp Output", -24.0f, 24.0f, 0.0f, " dB");

        addFloat ("eqLow", "Low", -12.0f, 12.0f, 0.0f, " dB");
        addFloat ("eqMid", "Mid", -12.0f, 12.0f, 0.0f, " dB");
        addFloat ("eqHigh", "High", -12.0f, 12.0f, 0.0f, " dB");
        addFloat ("eqAir", "Air", -12.0f, 12.0f, 0.0f, " dB");

        addFloat ("gateThreshold", "Gate Threshold", -80.0f, 0.0f, -42.0f, " dB");
        addFloat ("gateRange", "Gate Range", -80.0f, 0.0f, -24.0f, " dB");
        addFloat ("gateHyst", "Gate Hysteresis", 0.0f, 24.0f, 6.0f, " dB");
        addFloat ("gateAttack", "Gate Attack", 0.1f, 50.0f, 1.0f, " ms");
        addFloat ("gateHold", "Gate Hold", 0.0f, 300.0f, 60.0f, " ms");
        addFloat ("gateRelease", "Gate Release", 5.0f, 500.0f, 120.0f, " ms");

        addFloat ("deessFreq", "S Freq", 3000.0f, 12000.0f, 6000.0f, " Hz");
        addFloat ("deessThreshold", "DeEss Threshold", -60.0f, 0.0f, -28.0f, " dB");
        addFloat ("deessRange", "DeEss Range", -24.0f, 0.0f, -8.0f, " dB");
        addFloat ("deessLookahead", "Lookahead", 0.0f, 10.0f, 2.0f, " ms");
        addFloat ("deessRelease", "DeEss Release", 5.0f, 300.0f, 80.0f, " ms");

        addFloat ("compInput", "Comp Input", -24.0f, 24.0f, 0.0f, " dB");
        addFloat ("compOutput", "Comp Output", -24.0f, 24.0f, 0.0f, " dB");
        addFloat ("compAttack", "Attack", 0.1f, 30.0f, 3.0f, " ms");
        addFloat ("compRelease", "Release", 10.0f, 500.0f, 80.0f, " ms");
        addFloat ("compPeakReduction", "Peak Reduction", 0.0f, 100.0f, 25.0f, "%");
        addFloat ("compGain", "Gain", -24.0f, 24.0f, 0.0f, " dB");

        addFloat ("satDrive", "Sat Drive", 0.0f, 100.0f, 24.0f, "%");
        addFloat ("satTone", "Sat Tone", 0.0f, 100.0f, 12.0f, "%");
        addFloat ("satMix", "Sat Mix", 0.0f, 100.0f, 100.0f, "%");

        addFloat ("hiWarm", "Warm", 0.0f, 100.0f, 25.0f, "%");
        addFloat ("hiWidth", "Width", 0.0f, 200.0f, 100.0f, "%");
        addFloat ("hiTame", "Tame", 0.0f, 100.0f, 20.0f, "%");
        addFloat ("hiGlue", "Glue", 0.0f, 100.0f, 15.0f, "%");

        addFloat ("outCeiling", "Ceiling", -12.0f, 0.0f, -1.0f, " dBTP");
        addFloat ("outThreshold", "Threshold", -24.0f, 0.0f, -2.0f, " dB");

        return { p.begin(), p.end() };
    }
}
