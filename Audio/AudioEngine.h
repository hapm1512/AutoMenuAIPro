#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "AudioRingBuffer.h"
#include "../Core/AnalysisTypes.h"

namespace AutoMenu
{
    class AnalysisManager;

    class AudioEngine final : private juce::AudioIODeviceCallback
    {
    public:
        AudioEngine();
        ~AudioEngine() override;

        bool initialise (int inputChannels = 2, int outputChannels = 0);
        void shutdown();

        void setAnalysisManager (AnalysisManager* manager) noexcept;

        juce::String getCurrentDeviceName() const;
        double getSampleRate() const noexcept { return sampleRate; }
        int getBufferSize() const noexcept { return bufferSize; }
        bool isRunning() const noexcept { return running; }

        juce::AudioDeviceManager& getDeviceManager() noexcept { return deviceManager; }
        const AudioRingBuffer& getRingBuffer() const noexcept { return ringBuffer; }

    private:
        void audioDeviceIOCallbackWithContext (const float* const* inputChannelData,
                                               int numInputChannels,
                                               float* const* outputChannelData,
                                               int numOutputChannels,
                                               int numSamples,
                                               const juce::AudioIODeviceCallbackContext& context) override;
        void audioDeviceAboutToStart (juce::AudioIODevice* device) override;
        void audioDeviceStopped() override;

        juce::AudioDeviceManager deviceManager;
        AudioRingBuffer ringBuffer;
        AnalysisManager* analysisManager = nullptr;
        double sampleRate = 48000.0;
        int bufferSize = 0;
        bool running = false;
    };
}
