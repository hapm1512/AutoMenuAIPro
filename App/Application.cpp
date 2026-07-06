#include "Application.h"

const juce::String AutoMenuAIProApplication::getApplicationName()
{
    return "AutoMenuAIPro";
}

const juce::String AutoMenuAIProApplication::getApplicationVersion()
{
    return "0.4.0";
}

void AutoMenuAIProApplication::initialise (const juce::String&)
{
    mainWindow = std::make_unique<MainWindow>();
}

void AutoMenuAIProApplication::shutdown()
{
    mainWindow = nullptr;
}
