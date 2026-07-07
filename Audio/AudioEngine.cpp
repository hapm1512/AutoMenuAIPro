#include "AudioEngine.h"
#include "../Analysis/AnalysisManager.h"

namespace AutoMenu
{
    AudioEngine::AudioEngine() = default;

    AudioEngine::~AudioEngine()
    {
        shutdown();
    }

    bool AudioEngine::initialise (int inputChannels, int outputChannels)
    {
        shutdown();

        const auto error = deviceManager.initialise (inputChannels, outputChannels, nullptr, true);
        if (error.isNotEmpty())
            return false;

        deviceManager.addAudioCallback (this);
        running = true;
        return true;
    }

    void AudioEngine::shutdown()
    {
        if (running)
            deviceManager.removeAudioCallback (this);

        running = false;
        analysisManager = nullptr;
    }

    void AudioEngine::setAnalysisManager (AnalysisManager* manager) noexcept
    {
        analysisManager = manager;
    }

    juce::String AudioEngine::getCurrentDeviceName() const
    {
        if (auto* device = deviceManager.getCurrentAudioDevice())
            return device->getName();
        return "No Audio Device";
    }

    void AudioEngine::audioDeviceAboutToStart (juce::AudioIODevice* device)
    {
        if (device != nullptr)
        {
            sampleRate = device->getCurrentSampleRate();
            bufferSize = device->getCurrentBufferSizeSamples();
            ringBuffer.prepare (device->getActiveInputChannels().countNumberOfSetBits(), (int) sampleRate * 8);
        }
    }

    void AudioEngine::audioDeviceStopped()
    {
        ringBuffer.clear();
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

        ringBuffer.push (inputChannelData, numInputChannels, numSamples);

        if (analysisManager != nullptr)
            analysisManager->pushAudioBlock (inputChannelData, numInputChannels, numSamples, sampleRate);
    }
}
