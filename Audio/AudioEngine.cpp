#include "AudioEngine.h"

namespace automenu::audio
{
    AudioEngine::AudioEngine()
        : ringBuffer (48000 * 12)
    {
    }

    AudioEngine::~AudioEngine()
    {
        stop();
    }

    bool AudioEngine::start()
    {
        auto error = deviceManager.initialiseWithDefaultDevices (2, 0);

        if (error.isNotEmpty())
            return false;

        deviceManager.addAudioCallback (this);
        running.store (true);
        return true;
    }

    void AudioEngine::stop()
    {
        if (! running.exchange (false))
            return;

        deviceManager.removeAudioCallback (this);
        deviceManager.closeAudioDevice();
        ringBuffer.reset();
    }

    juce::String AudioEngine::getCurrentDeviceName() const
    {
        if (auto* device = deviceManager.getCurrentAudioDevice())
            return device->getName();

        return "No Device";
    }

    void AudioEngine::audioDeviceAboutToStart (juce::AudioIODevice* device)
    {
        if (device != nullptr)
        {
            sampleRate = device->getCurrentSampleRate();
            blockSize = device->getCurrentBufferSizeSamples();
        }

        monoScratch.resize ((size_t) juce::jmax (blockSize, 512));
        ringBuffer.reset();
    }

    void AudioEngine::audioDeviceStopped()
    {
        sampleRate = 48000.0;
        blockSize = 512;
    }

    void AudioEngine::audioDeviceIOCallbackWithContext (const float* const* inputChannelData,
                                                        int numInputChannels,
                                                        float* const* outputChannelData,
                                                        int numOutputChannels,
                                                        int numSamples,
                                                        const juce::AudioIODeviceCallbackContext&)
    {
        for (int ch = 0; ch < numOutputChannels; ++ch)
            if (outputChannelData[ch] != nullptr)
                juce::FloatVectorOperations::clear (outputChannelData[ch], numSamples);

        if (numInputChannels <= 0 || inputChannelData == nullptr || inputChannelData[0] == nullptr)
            return;

        if ((int) monoScratch.size() < numSamples)
            monoScratch.resize ((size_t) numSamples);

        juce::FloatVectorOperations::copy (monoScratch.data(), inputChannelData[0], numSamples);

        if (numInputChannels > 1 && inputChannelData[1] != nullptr)
        {
            juce::FloatVectorOperations::add (monoScratch.data(), inputChannelData[1], numSamples);
            juce::FloatVectorOperations::multiply (monoScratch.data(), 0.5f, numSamples);
        }

        ringBuffer.pushMono (monoScratch.data(), numSamples);
    }
}
