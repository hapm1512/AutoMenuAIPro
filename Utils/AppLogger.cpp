#include "AppLogger.h"

namespace AutoMenu
{
    juce::CriticalSection AppLogger::lock;
    std::unique_ptr<juce::FileLogger> AppLogger::logger;

    void AppLogger::initialise()
    {
        const juce::ScopedLock sl (lock);

        if (logger != nullptr)
            return;

        auto dir = juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
            .getChildFile ("AutoMenuAIPro")
            .getChildFile ("Logs");

        dir.createDirectory();

        const auto fileName = "AutoMenuAIPro_" + juce::Time::getCurrentTime().formatted ("%Y%m%d") + ".log";
        logger = std::make_unique<juce::FileLogger> (dir.getChildFile (fileName), "AutoMenuAIPro Log", 512 * 1024);
        juce::Logger::setCurrentLogger (logger.get());

        logger->logMessage ("Logger started");
    }

    void AppLogger::shutdown()
    {
        const juce::ScopedLock sl (lock);

        if (logger != nullptr)
            logger->logMessage ("Logger stopped");

        juce::Logger::setCurrentLogger (nullptr);
        logger.reset();
    }

    void AppLogger::write (const juce::String& message)
    {
        const juce::ScopedLock sl (lock);

        if (logger != nullptr)
            logger->logMessage (message);
        else
            juce::Logger::writeToLog (message);
    }

    juce::File AppLogger::getLogFile()
    {
        const juce::ScopedLock sl (lock);

        if (logger != nullptr)
            return logger->getLogFile();

        return {};
    }
}
