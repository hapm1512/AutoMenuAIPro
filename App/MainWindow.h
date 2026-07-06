#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../UI/Dashboard/Dashboard.h"

class MainWindow final : public juce::DocumentWindow
{
public:
    MainWindow();
    void closeButtonPressed() override;
};
