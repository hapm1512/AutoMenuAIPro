#include "AudioRingBuffer.h"

namespace AutoMenu
{
    AudioRingBuffer::AudioRingBuffer() = default;

    void AudioRingBuffer::prepare (int channels, int capacitySamples)
    {
        const juce::ScopedLock sl (lock);
        numChannels = juce::jmax (1, channels);
        capacity = juce::jmax (1024, capacitySamples);
        data.assign ((size_t) numChannels, std::vector<float> ((size_t) capacity, 0.0f));
        writePosition.store (0);
        writtenSamples.store (0);
    }

    void AudioRingBuffer::clear()
    {
        const juce::ScopedLock sl (lock);
        for (auto& channel : data)
            std::fill (channel.begin(), channel.end(), 0.0f);
        writePosition.store (0);
        writtenSamples.store (0);
    }

    void AudioRingBuffer::push (const float* const* input, int channels, int samples)
    {
        if (capacity <= 0 || samples <= 0 || input == nullptr)
            return;

        const juce::ScopedLock sl (lock);
        int wp = writePosition.load();

        for (int i = 0; i < samples; ++i)
        {
            for (int ch = 0; ch < numChannels; ++ch)
            {
                const int src = juce::jmin (ch, channels - 1);
                data[(size_t) ch][(size_t) wp] = input[src] != nullptr ? input[src][i] : 0.0f;
            }

            wp = (wp + 1) % capacity;
        }

        writePosition.store (wp);
        writtenSamples.store (juce::jmin (capacity, writtenSamples.load() + samples));
    }

    int AudioRingBuffer::readLatestMono (juce::AudioBuffer<float>& destination, int samples) const
    {
        if (capacity <= 0 || samples <= 0)
            return 0;

        const juce::ScopedLock sl (lock);
        const int available = writtenSamples.load();
        const int toRead = juce::jmin (samples, available);

        destination.setSize (1, toRead, false, false, true);
        destination.clear();

        int start = writePosition.load() - toRead;
        while (start < 0)
            start += capacity;

        auto* out = destination.getWritePointer (0);
        for (int i = 0; i < toRead; ++i)
        {
            const int idx = (start + i) % capacity;
            float mixed = 0.0f;

            for (int ch = 0; ch < numChannels; ++ch)
                mixed += data[(size_t) ch][(size_t) idx];

            out[i] = mixed / (float) juce::jmax (1, numChannels);
        }

        return toRead;
    }
}
