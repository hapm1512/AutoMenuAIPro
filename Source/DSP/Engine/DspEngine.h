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

private:
    PreampModule preamp;
    EqModule eq;
    GateModule gate;
    DeEsserModule deesser;
    CompressorModule compressor;
    SaturationModule saturation;
    HiEndModule hiend;
    OutputModule output;

    std::atomic<float> inputPeak { 0.0f };
    std::atomic<float> outputPeak { 0.0f };
    std::atomic<float> gainReduction { 0.0f };
};
