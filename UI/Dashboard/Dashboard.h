#pragma once

#include <juce_gui_extra/juce_gui_extra.h>
#include "../Header/HeaderComponent.h"
#include "../Panels/MacroPanel/MacroPanelComponent.h"
#include "../Panels/MixerPanel/MixerPanelComponent.h"
#include "../Panels/TonePanel/ToneDetectorComponent.h"
#include "../Panels/OutputMeterPanel/OutputMeterComponent.h"
#include "../Footer/FooterComponent.h"

class Dashboard final : public juce::Component
{
public:
    Dashboard();
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    HeaderComponent header;
    MacroPanelComponent macroPanel;
    MixerPanelComponent mixerPanel;
    ToneDetectorComponent toneDetector;
    FooterComponent footer;
    OutputMeterComponent outputMeter;
};
