#pragma once
#include <JuceHeader.h>

#include "PreampModule.h"
#include "EqModule.h"
#include "GateModule.h"
#include "DeEsserModule.h"
#include "CompressorModule.h"
#include "SaturationModule.h"
#include "HiEndModule.h"
#include "OutputModule.h"
#include "DspUtilities.h"
#include "ProfessionalMetering.h"

class DspEngine final
{
public:
    void prepare (double sampleRate, int blockSize, int channels);
    void reset();

    void process (juce::AudioBuffer<float>& buffer,
                  juce::AudioProcessorValueTreeState& apvts);

    float getInputPeak() const noexcept;
    float getOutputPeak() const noexcept;
    float getGainReduction() const noexcept;
    float getInputRms() const noexcept;
    float getOutputRms() const noexcept;
    float getTruePeak() const noexcept;
    float getLufsMomentary() const noexcept;
    float getLufsShortTerm() const noexcept;
    float getLufsIntegrated() const noexcept;
    float getStereoCorrelation() const noexcept;

private:
    PreampModule preamp;
    GateModule gate;
    EqModule eq;
    DeEsserModule deesser;
    CompressorModule compressor;
    SaturationModule saturation;
    HiEndModule hiend;
    OutputModule output;
    VocalDSP::ProfessionalMetering professionalMetering;

    std::atomic<float> inputPeak { 0.0f };
    std::atomic<float> outputPeak { 0.0f };
    std::atomic<float> inputRms { 0.0f };
    std::atomic<float> outputRms { 0.0f };
    std::atomic<float> gainReduction { 0.0f };
    std::atomic<float> truePeak { 0.0f };
    std::atomic<float> lufsMomentary { -70.0f };
    std::atomic<float> lufsShortTerm { -70.0f };
    std::atomic<float> lufsIntegrated { -70.0f };
    std::atomic<float> stereoCorrelation { 1.0f };
};
