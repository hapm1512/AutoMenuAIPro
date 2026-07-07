#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <vector>
#include <atomic>

namespace automenu::audio
{
    class AudioRingBuffer final
    {
    public:
        explicit AudioRingBuffer (int capacitySamples = 48000 * 10);

        void reset();
        void pushMono (const float* samples, int numSamples);
        int readLatest (std::vector<float>& destination, int numSamples) const;
        int getCapacity() const noexcept { return capacity; }

    private:
        std::vector<float> buffer;
        int capacity = 0;
        std::atomic<int> writeIndex { 0 };
        mutable juce::SpinLock lock;
    };
}
