#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <vector>
#include <atomic>

namespace AutoMenu
{
    class AudioRingBuffer final
    {
    public:
        AudioRingBuffer();

        void prepare (int channels, int capacitySamples);
        void clear();

        void push (const float* const* input, int channels, int samples);
        int readLatestMono (juce::AudioBuffer<float>& destination, int samples) const;

        int getCapacity() const noexcept { return capacity; }
        int getNumChannels() const noexcept { return numChannels; }

    private:
        int numChannels = 0;
        int capacity = 0;
        mutable juce::CriticalSection lock;
        std::vector<std::vector<float>> data;
        std::atomic<int> writePosition { 0 };
        std::atomic<int> writtenSamples { 0 };
    };
}
