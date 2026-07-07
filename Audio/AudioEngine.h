#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include "AudioRingBuffer.h"

namespace automenu::audio
{
    class AudioEngine final : private juce::AudioIODeviceCallback
    {
    public:
        AudioEngine();
        ~AudioEngine() override;

        bool start();
        void stop();

        bool isRunning() const noexcept { return running; }
        double getSampleRate() const noexcept { return sampleRate; }
        int getBlockSize() const noexcept { return blockSize; }
        juce::String getCurrentDeviceName() const;

        AudioRingBuffer& getRingBuffer() noexcept { return ringBuffer; }
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
        std::vector<float> monoScratch;
        std::atomic<bool> running { false };
        double sampleRate = 48000.0;
        int blockSize = 512;
    };
}
