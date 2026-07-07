#include "Dashboard.h"
#include "../Theme/AppTheme.h"

Dashboard::Dashboard()
{
    setSize (1080, 520);

    addAndMakeVisible (header);
    addAndMakeVisible (macroPanel);
    addAndMakeVisible (mixerPanel);
    addAndMakeVisible (toneDetector);
    addAndMakeVisible (footer);
    addAndMakeVisible (settingsPanel);

    settingsPanel.setVisible (false);

    footer.onSettings = [this]
    {
        toggleSettings();
    };

    settingsPanel.onClose = [this]
    {
        showSettings (false);
    };

    settingsPanel.onSave = [this]
    {
        showSettings (false);
    };

    macroPanel.onMacroPressed = [this] (int macroIndex)
    {
        core.triggerMacro (macroIndex);
    };

    toneDetector.onApplySuggestion = [this]
    {
        core.applyCurrentSuggestion();
    };

    core.startAudio();
    core.connectCubaseMidiAuto();

    startTimerHz (12);
}

Dashboard::~Dashboard()
{
    stopTimer();
    core.stopAudio();
}

void Dashboard::paint (juce::Graphics& g)
{
    g.fillAll (AppTheme::background());

    if (settingsVisible)
    {
        g.setColour (juce::Colours::black.withAlpha (0.22f));
        g.fillRect (getLocalBounds());
    }
}

void Dashboard::resized()
{
    auto r = getLocalBounds().reduced (8);

    headerArea = r.removeFromTop (38);
    header.setBounds (headerArea);

    r.removeFromTop (6);

    auto left = r;
    auto right = left.removeFromRight (280);
    left.removeFromRight (6);

    macroArea = left.removeFromTop (176);
    macroPanel.setBounds (macroArea);

    left.removeFromTop (6);

    statusArea = left.removeFromBottom (24);
    footer.setBounds (statusArea);

    left.removeFromBottom (6);

    mixerArea = left;
    mixerPanel.setBounds (mixerArea);

    toneArea = right;
    toneDetector.setBounds (toneArea);

    settingsArea = getLocalBounds()
        .removeFromRight (360)
        .reduced (8, 50)
        .withTrimmedBottom (2);

    settingsPanel.setBounds (settingsArea);
    settingsPanel.toFront (false);
}

void Dashboard::timerCallback()
{
    updateToneDisplay();
}

void Dashboard::updateToneDisplay()
{
    toneDetector.setRealtimeToneState (core.getRealtimeToneState());
    toneDetector.setSuggestionState (core.updateSuggestion());
}

void Dashboard::toggleSettings()
{
    showSettings (! settingsVisible);
}

void Dashboard::showSettings (bool shouldShow)
{
    settingsVisible = shouldShow;
    settingsPanel.setVisible (settingsVisible);
    settingsPanel.toFront (true);
    repaint();
}
