#pragma once
#include <JuceHeader.h>
#include <vector>

class PresetManager final
{
public:
    explicit PresetManager (juce::AudioProcessorValueTreeState& stateToUse);

    struct FactoryPreset
    {
        juce::String name;
        juce::String category;
        std::vector<std::pair<juce::String, float>> values;
    };

    int getNumFactoryPresets() const noexcept;
    int getCurrentFactoryIndex() const noexcept;
    juce::String getCurrentPresetName() const;
    juce::StringArray getFactoryPresetNames() const;

    void loadFactoryPreset (int index);
    void loadNextFactoryPreset();
    void loadPreviousFactoryPreset();

    void captureA();
    void captureB();
    void copyAToB();
    void swapAB();
    void recallA();
    void recallB();

    bool saveUserPreset (const juce::String& presetName);
    bool loadUserPreset (const juce::File& presetFile);
    juce::File getPresetDirectory() const;

    void pushUndoState();
    bool undo();
    bool redo();

private:
    juce::AudioProcessorValueTreeState& apvts;
    std::vector<FactoryPreset> factoryPresets;

    int currentFactoryIndex = 0;
    juce::String currentPresetName { "Modern Vocal Pop" };

    juce::ValueTree slotA;
    juce::ValueTree slotB;
    juce::Array<juce::ValueTree> undoStack;
    juce::Array<juce::ValueTree> redoStack;

    juce::ValueTree copyState() const;
    void replaceState (const juce::ValueTree& tree);
    void applyValue (const juce::String& paramID, float plainValue);
    void buildFactoryPresets();
};
