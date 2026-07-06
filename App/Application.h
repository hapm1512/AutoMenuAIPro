#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "MainWindow.h"

class AutoMenuAIProApplication final : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override;
    const juce::String getApplicationVersion() override;

    void initialise (const juce::String& commandLine) override;
    void shutdown() override;

private:
    std::unique_ptr<MainWindow> mainWindow;
};
