#include "PluginEditor.h"

VocalSuiteUltraProAudioProcessorEditor::VocalSuiteUltraProAudioProcessorEditor
    (VocalSuiteUltraProAudioProcessor& processor)
    : AudioProcessorEditor (&processor),
      editor (processor)
{
    addAndMakeVisible (editor);
    setResizable (true, true);
    setWantsKeyboardFocus (true);
    setResizeLimits (1180, 720, 1720, 980);
    setSize (1540, 900);
}

void VocalSuiteUltraProAudioProcessorEditor::resized()
{
    editor.setBounds (getLocalBounds());
}


bool VocalSuiteUltraProAudioProcessorEditor::keyPressed (const juce::KeyPress& key)
{
    return editor.keyPressed (key);
}
