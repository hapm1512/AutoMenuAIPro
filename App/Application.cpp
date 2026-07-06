#include "Application.h"

const juce::String AutoMenuAIProApplication::getApplicationName()
{
    return "AutoMenuAIPro";
}

const juce::String AutoMenuAIProApplication::getApplicationVersion()
{
    return "0.3.0";
}

bool AutoMenuAIProApplication::moreThanOneInstanceAllowed()
{
    return true;
}

void AutoMenuAIProApplication::initialise (const juce::String&)
{
    mainWindow = std::make_unique<MainWindow>();
}

void AutoMenuAIProApplication::shutdown()
{
    mainWindow = nullptr;
}

void AutoMenuAIProApplication::systemRequestedQuit()
{
    quit();
}

void AutoMenuAIProApplication::anotherInstanceStarted (const juce::String&)
{
    // Sprint 03A: no multi-instance action yet.
}
