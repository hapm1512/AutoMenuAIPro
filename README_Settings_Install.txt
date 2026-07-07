COPY FILES:
UI/Settings/SettingsPanel.h
UI/Settings/SettingsPanel.cpp

CMakeLists.txt:
Add source:
UI/Settings/SettingsPanel.cpp

Add include directory:
UI/Settings

Dashboard.h:
#include "../Settings/SettingsPanel.h"

private:
SettingsPanel settingsPanel;
bool settingsVisible = false;

Dashboard.cpp constructor:
addAndMakeVisible (settingsPanel);
settingsPanel.setVisible (false);
settingsPanel.onClose = [this]
{
    settingsVisible = false;
    settingsPanel.setVisible (false);
    repaint();
};

Dashboard.cpp resized():
auto settingsWidth = 320;
settingsPanel.setBounds (getLocalBounds().removeFromRight (settingsWidth).reduced (8));

To open settings from current Settings button:
settingsVisible = !settingsVisible;
settingsPanel.setVisible (settingsVisible);
settingsPanel.toFront (false);
