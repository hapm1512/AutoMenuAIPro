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

        addFloat ("preampInputTrim", "Input Trim", -18.0f, 18.0f, 0.0f, " dB");
        addFloat ("preampDrive", "Drive", 0.0f, 100.0f, 23.0f, "%");
        addFloat ("preampHeadroom", "Headroom", 3.0f, 24.0f, 12.0f, " dB");
        addFloat ("preampTransformer", "Transformer", 0.0f, 100.0f, 35.0f, "%");
        addFloat ("preampTube", "Tube", 0.0f, 100.0f, 30.0f, "%");
        addFloat ("preampHarmonic", "Harmonic", 0.0f, 100.0f, 35.0f, "%");
        addFloat ("preampBias", "Dynamic Bias", -100.0f, 100.0f, 12.0f, "%");
        addFloat ("preampMemory", "Memory", 0.0f, 100.0f, 20.0f, "%");
        addFloat ("preampStereo", "Stereo Var", 0.0f, 100.0f, 10.0f, "%");
        addFloat ("preampCrosstalk", "Crosstalk", 0.0f, 100.0f, 4.0f, "%");
        addFloat ("preampNoise", "Noise Floor", 0.0f, 100.0f, 0.0f, "%");
        addFloat ("preampAutoGain", "Auto Gain", 0.0f, 100.0f, 70.0f, "%");
        addFloat ("preampPeakProtect", "Peak Protect", 0.0f, 100.0f, 100.0f, "%");
        addFloat ("preampMix", "Preamp Mix", 0.0f, 100.0f, 100.0f, "%");
        addFloat ("preampOutput", "Preamp Output", -24.0f, 24.0f, 0.0f, " dB");

        addFloat ("eqLow", "Low", -12.0f, 12.0f, 0.0f, " dB");
        addFloat ("eqMid", "Mid", -12.0f, 12.0f, 0.0f, " dB");
        addFloat ("eqHigh", "High", -12.0f, 12.0f, 0.0f, " dB");
        addFloat ("eqAir", "Air", -12.0f, 12.0f, 0.0f, " dB");

        addFloat ("eqHpfFreq", "EQ HPF", 20.0f, 240.0f, 70.0f, " Hz");
        addFloat ("eqLowFreq", "EQ Low Freq", 40.0f, 300.0f, 120.0f, " Hz");
        addFloat ("eqLowGain", "EQ Low Gain", -15.0f, 15.0f, 0.0f, " dB");
        addFloat ("eqLowMidFreq", "EQ Low Mid Freq", 150.0f, 900.0f, 320.0f, " Hz");
        addFloat ("eqLowMidGain", "EQ Low Mid Gain", -15.0f, 15.0f, 0.0f, " dB");
        addFloat ("eqLowMidQ", "EQ Low Mid Q", 0.20f, 5.0f, 1.0f);
        addFloat ("eqMidFreq", "EQ Mid Freq", 500.0f, 3200.0f, 1200.0f, " Hz");
        addFloat ("eqMidGain", "EQ Mid Gain", -15.0f, 15.0f, 0.0f, " dB");
        addFloat ("eqMidQ", "EQ Mid Q", 0.20f, 5.0f, 1.0f);
        addFloat ("eqHighMidFreq", "EQ High Mid Freq", 1800.0f, 7000.0f, 3800.0f, " Hz");
        addFloat ("eqHighMidGain", "EQ High Mid Gain", -15.0f, 15.0f, 0.0f, " dB");
        addFloat ("eqHighMidQ", "EQ High Mid Q", 0.20f, 5.0f, 1.1f);
        addFloat ("eqHighFreq", "EQ High Freq", 4000.0f, 14000.0f, 8500.0f, " Hz");
        addFloat ("eqHighGain", "EQ High Gain", -15.0f, 15.0f, 0.0f, " dB");
        addFloat ("eqAirFreq", "EQ Air Freq", 8000.0f, 20000.0f, 14000.0f, " Hz");
        addFloat ("eqAirGain", "EQ Air Gain", -15.0f, 15.0f, 0.0f, " dB");
        addFloat ("eqDynAmount", "EQ Dynamic", 0.0f, 100.0f, 20.0f, "%");
        addFloat ("eqDynThreshold", "EQ Dynamic Threshold", -60.0f, 0.0f, -20.0f, " dB");
        addFloat ("eqMS", "EQ M/S", 0.0f, 100.0f, 0.0f, "%");
        addFloat ("eqAutoGain", "EQ Auto Gain", 0.0f, 100.0f, 70.0f, "%");
        addFloat ("eqOutput", "EQ Output", -18.0f, 18.0f, 0.0f, " dB");

        addFloat ("gateThreshold", "Gate Threshold", -80.0f, 0.0f, -42.0f, " dB");
        addFloat ("gateRange", "Gate Range", -80.0f, 0.0f, -24.0f, " dB");
        addFloat ("gateHyst", "Gate Hysteresis", 0.0f, 24.0f, 6.0f, " dB");
        addFloat ("gateAttack", "Gate Attack", 0.1f, 50.0f, 1.0f, " ms");
        addFloat ("gateHold", "Gate Hold", 0.0f, 300.0f, 60.0f, " ms");
        addFloat ("gateRelease", "Gate Release", 5.0f, 500.0f, 120.0f, " ms");
        addFloat ("gateLookAhead", "Gate LookAhead", 0.0f, 10.0f, 2.0f, " ms");
        addFloat ("gateKnee", "Gate Knee", 0.1f, 24.0f, 8.0f, " dB");
        addFloat ("gateDepth", "Gate Depth", 0.0f, 100.0f, 100.0f, "%");
        addFloat ("gateSideHp", "Gate Side HPF", 20.0f, 500.0f, 90.0f, " Hz");
        addFloat ("gateSideLp", "Gate Side LPF", 1000.0f, 18000.0f, 9000.0f, " Hz");
        addFloat ("gateRmsMix", "Gate RMS Mix", 0.0f, 100.0f, 65.0f, "%");
        addFloat ("gateAdaptive", "Gate Adaptive", 0.0f, 100.0f, 35.0f, "%");
        addFloat ("gateBreath", "Gate Breath", 0.0f, 100.0f, 30.0f, "%");
        addFloat ("gateStereoLink", "Gate Stereo Link", 0.0f, 100.0f, 75.0f, "%");
        addFloat ("gateAutoMakeup", "Gate Auto Makeup", 0.0f, 100.0f, 15.0f, "%");

        addFloat ("deessFreq", "Sibilance Freq", 3000.0f, 12000.0f, 6200.0f, " Hz");
        addFloat ("deessThreshold", "DeEss Threshold", -60.0f, 0.0f, -30.0f, " dB");
        addFloat ("deessRange", "DeEss Range", -30.0f, 0.0f, -10.0f, " dB");
        addFloat ("deessRatio", "DeEss Ratio", 1.0f, 12.0f, 4.0f, ":1");
        addFloat ("deessAttack", "DeEss Attack", 0.1f, 15.0f, 0.7f, " ms");
        addFloat ("deessHold", "DeEss Hold", 0.0f, 80.0f, 12.0f, " ms");
        addFloat ("deessRelease", "DeEss Release", 5.0f, 350.0f, 75.0f, " ms");
        addFloat ("deessLookahead", "DeEss LookAhead", 0.0f, 10.0f, 2.0f, " ms");
        addFloat ("deessKnee", "DeEss Knee", 0.1f, 24.0f, 7.0f, " dB");
        addFloat ("deessFocus", "DeEss Focus", 0.0f, 100.0f, 65.0f, "%");
        addFloat ("deessPeakMix", "DeEss Peak/RMS", 0.0f, 100.0f, 55.0f, "%");
        addFloat ("deessAdaptive", "DeEss Adaptive", 0.0f, 100.0f, 35.0f, "%");
        addFloat ("deessSplit", "DeEss Split", 0.0f, 100.0f, 80.0f, "%");
        addFloat ("deessStereoLink", "DeEss Stereo Link", 0.0f, 100.0f, 70.0f, "%");
        addFloat ("deessAutoMakeup", "DeEss Auto Makeup", 0.0f, 100.0f, 10.0f, "%");
        addFloat ("deessOutput", "DeEss Output", -12.0f, 12.0f, 0.0f, " dB");

        addFloat ("compInput", "Comp Input", -24.0f, 24.0f, 0.0f, " dB");
        addFloat ("compOutput", "Comp Output", -24.0f, 24.0f, 0.0f, " dB");
        addFloat ("compAttack", "1176 Attack", 0.1f, 30.0f, 2.2f, " ms");
        addFloat ("compRelease", "1176 Release", 10.0f, 600.0f, 85.0f, " ms");
        addFloat ("comp1176Ratio", "1176 Ratio", 2.0f, 20.0f, 4.0f, ":1");
        addFloat ("comp1176Amount", "1176 Input", 0.0f, 100.0f, 35.0f, "%");
        addFloat ("compLA2APeak", "LA-2A Peak Reduction", 0.0f, 100.0f, 25.0f, "%");
        addFloat ("compLA2AGain", "LA-2A Gain", -12.0f, 12.0f, 0.0f, " dB");
        addFloat ("compLA2ASpeed", "LA-2A Speed", 0.0f, 100.0f, 45.0f, "%");
        addFloat ("compParallel", "Compressor Mix", 0.0f, 100.0f, 100.0f, "%");
        addFloat ("compSideHp", "Compressor Side HPF", 20.0f, 300.0f, 90.0f, " Hz");
        addFloat ("compAnalog", "Compressor Analog", 0.0f, 100.0f, 30.0f, "%");
        addFloat ("compAutoGain", "Compressor Auto Gain", 0.0f, 100.0f, 55.0f, "%");

        addFloat ("satDrive", "Sat Drive", 0.0f, 100.0f, 24.0f, "%");
        addFloat ("satTone", "Sat Tone", 0.0f, 100.0f, 12.0f, "%");
        addFloat ("satTape", "Tape", 0.0f, 100.0f, 35.0f, "%");
        addFloat ("satTube", "Tube", 0.0f, 100.0f, 28.0f, "%");
        addFloat ("satTransformer", "Transformer", 0.0f, 100.0f, 30.0f, "%");
        addFloat ("satEven", "Even Harmonics", 0.0f, 100.0f, 25.0f, "%");
        addFloat ("satOdd", "Odd Harmonics", 0.0f, 100.0f, 18.0f, "%");
        addFloat ("satBias", "Bias", -100.0f, 100.0f, 0.0f, "%");
        addFloat ("satPresence", "Presence", 0.0f, 100.0f, 18.0f, "%");
        addFloat ("satAir", "Air", 0.0f, 100.0f, 20.0f, "%");
        addFloat ("satLowThick", "Low Thick", 0.0f, 100.0f, 12.0f, "%");
        addFloat ("satWidth", "Sat Width", 0.0f, 200.0f, 100.0f, "%");
        addFloat ("satCrosstalk", "Sat Crosstalk", 0.0f, 100.0f, 3.0f, "%");
        addFloat ("satAutoGain", "Sat Auto Gain", 0.0f, 100.0f, 70.0f, "%");
        addFloat ("satMix", "Sat Mix", 0.0f, 100.0f, 100.0f, "%");
        addFloat ("satOutput", "Sat Output", -18.0f, 18.0f, 0.0f, " dB");

        addFloat ("hiWarm", "Warm", 0.0f, 100.0f, 25.0f, "%");
        addFloat ("hiWidth", "Width", 0.0f, 200.0f, 100.0f, "%");
        addFloat ("hiTame", "Tame", 0.0f, 100.0f, 20.0f, "%");
        addFloat ("hiGlue", "Glue", 0.0f, 100.0f, 15.0f, "%");

        addFloat ("outTrim", "Output Trim", -24.0f, 24.0f, 0.0f, " dB");
        addFloat ("outCeiling", "Ceiling", -12.0f, 0.0f, -1.0f, " dBTP");
        addFloat ("outThreshold", "Threshold", -24.0f, 0.0f, -2.0f, " dB");
        addFloat ("outSoftClip", "Output Soft Clip", 0.0f, 100.0f, 20.0f, "%");
        addFloat ("outISP", "ISP Protection", 0.0f, 100.0f, 100.0f, "%");
        addFloat ("outLufsComp", "LUFS Compensation", -12.0f, 12.0f, 0.0f, " dB");
        addFloat ("outDither", "Dither", 0.0f, 100.0f, 0.0f, "%");
        addFloat ("outRelease", "Limiter Release", 10.0f, 500.0f, 60.0f, " ms");
        addFloat ("outMix", "Output Mix", 0.0f, 100.0f, 100.0f, "%");

        return { p.begin(), p.end() };
    }
}
