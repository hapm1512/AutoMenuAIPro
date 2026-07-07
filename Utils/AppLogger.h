#pragma once

#include <juce_core/juce_core.h>

namespace AutoMenu
{
    class AppLogger final
    {
    public:
        static void initialise();
        static void shutdown();
        static void write (const juce::String& message);
        static juce::File getLogFile();

    private:
        static juce::CriticalSection lock;
        static std::unique_ptr<juce::FileLogger> logger;
    };
}
