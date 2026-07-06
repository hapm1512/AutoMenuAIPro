#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../UI/Dashboard/Dashboard.h"

class MainWindow final : public juce::DocumentWindow
{
public:
    MainWindow();
    ~MainWindow() override = default;

    void closeButtonPressed() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};
