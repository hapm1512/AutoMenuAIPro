#include "MidiCommandQueue.h"

namespace AutoMenu
{
    void MidiCommandQueue::push(const MidiCommand& command)
    {
        const juce::ScopedLock sl(lock);
        queue.push_back(command);
    }

    std::vector<MidiCommand> MidiCommandQueue::popAll()
    {
        const juce::ScopedLock sl(lock);

        auto result = queue;
        queue.clear();

        return result;
    }

    void MidiCommandQueue::clear()
    {
        const juce::ScopedLock sl(lock);
        queue.clear();
    }

    bool MidiCommandQueue::isEmpty() const
    {
        const juce::ScopedLock sl(lock);
        return queue.empty();
    }
}