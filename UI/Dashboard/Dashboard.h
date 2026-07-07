#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../Header/HeaderComponent.h"
#include "../Panels/MacroPanel/MacroPanelComponent.h"
#include "../Panels/MixerPanel/MixerPanelComponent.h"
#include "../Panels/TonePanel/ToneDetectorComponent.h"
#include "../Footer/FooterComponent.h"
#include "../Settings/SettingsPanel.h"
#include "../../Core/AutoMenuCore.h"

class Dashboard final : public juce::Component,
                        private juce::Timer
{
public:
    Dashboard();
    ~Dashboard() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    void timerCallback() override;
    void toggleSettings();
    void showSettings (bool shouldShow);
    void updateToneDisplay();

    AutoMenu::AutoMenuCore core;

    HeaderComponent header;
    MacroPanelComponent macroPanel;
    MixerPanelComponent mixerPanel;
    ToneDetectorComponent toneDetector;
    FooterComponent footer;
    SettingsPanel settingsPanel;

    bool settingsVisible = false;

    juce::Rectangle<int> headerArea;
    juce::Rectangle<int> macroArea;
    juce::Rectangle<int> mixerArea;
    juce::Rectangle<int> statusArea;
    juce::Rectangle<int> toneArea;
    juce::Rectangle<int> settingsArea;
};
