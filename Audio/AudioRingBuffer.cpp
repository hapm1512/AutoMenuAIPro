#include "AudioRingBuffer.h"
#include <algorithm>

namespace automenu::audio
{
    AudioRingBuffer::AudioRingBuffer (int capacitySamples)
        : buffer ((size_t) juce::jmax (capacitySamples, 1024), 0.0f),
          capacity ((int) buffer.size())
    {
    }

    void AudioRingBuffer::reset()
    {
        const juce::SpinLock::ScopedLockType sl (lock);
        std::fill (buffer.begin(), buffer.end(), 0.0f);
        writeIndex.store (0);
    }

    void AudioRingBuffer::pushMono (const float* samples, int numSamples)
    {
        if (samples == nullptr || numSamples <= 0)
            return;

        const juce::SpinLock::ScopedLockType sl (lock);
        auto index = writeIndex.load();

        for (int i = 0; i < numSamples; ++i)
        {
            buffer[(size_t) index] = samples[i];
            index = (index + 1) % capacity;
        }

        writeIndex.store (index);
    }

    int AudioRingBuffer::readLatest (std::vector<float>& destination, int numSamples) const
    {
        numSamples = juce::jlimit (0, capacity, numSamples);
        destination.assign ((size_t) numSamples, 0.0f);

        if (numSamples <= 0)
            return 0;

        const juce::SpinLock::ScopedLockType sl (lock);
        const int currentWrite = writeIndex.load();
        int start = currentWrite - numSamples;

        while (start < 0)
            start += capacity;

        for (int i = 0; i < numSamples; ++i)
            destination[(size_t) i] = buffer[(size_t) ((start + i) % capacity)];

        return numSamples;
    }
}
