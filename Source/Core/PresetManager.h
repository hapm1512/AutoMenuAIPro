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
        juce::String author;
        juce::String version;
        juce::String description;
        juce::StringArray tags;
        std::vector<std::pair<juce::String, float>> values;
    };

    int getNumFactoryPresets() const noexcept;
    int getCurrentFactoryIndex() const noexcept;
    juce::String getCurrentPresetName() const;
    juce::String getCurrentPresetCategory() const;
    juce::String getCurrentPresetAuthor() const;
    juce::String getCurrentPresetDescription() const;
    juce::StringArray getFactoryPresetNames() const;
    juce::StringArray getFactoryPresetCategories() const;
    juce::StringArray getFactoryPresetNamesForCategory (const juce::String& categoryName) const;

    void loadFactoryPreset (int index);
    bool loadFactoryPresetByName (const juce::String& presetName);
    void loadNextFactoryPreset();
    void loadPreviousFactoryPreset();

    void setFavorite (const juce::String& presetName, bool shouldBeFavorite);
    bool isFavorite (const juce::String& presetName) const;
    juce::StringArray getFavoritePresetNames() const;
    juce::StringArray getRecentPresetNames() const;
    juce::StringArray searchFactoryPresets (const juce::String& searchText) const;

    void captureA();
    void captureB();
    void copyAToB();
    void swapAB();
    void recallA();
    void recallB();

    bool saveUserPreset (const juce::String& presetName);
    bool loadUserPreset (const juce::File& presetFile);
    bool exportCurrentPreset (const juce::File& destinationFile) const;
    juce::File getPresetDirectory() const;

    void pushUndoState();
    bool undo();
    bool redo();

private:
    juce::AudioProcessorValueTreeState& apvts;
    std::vector<FactoryPreset> factoryPresets;

    int currentFactoryIndex = 0;
    juce::String currentPresetName { "Clean Vocal" };
    juce::String currentCategory { "Studio" };
    juce::String currentDescription { "Transparent vocal chain." };

    juce::StringArray favorites;
    juce::StringArray recentPresets;

    juce::ValueTree slotA;
    juce::ValueTree slotB;
    juce::Array<juce::ValueTree> undoStack;
    juce::Array<juce::ValueTree> redoStack;

    juce::ValueTree copyState() const;
    void replaceState (const juce::ValueTree& tree);
    void applyValue (const juce::String& paramID, float plainValue);
    void buildFactoryPresets();
    void addRecentPreset (const juce::String& presetName);
    juce::ValueTree createPresetTree (const FactoryPreset& preset) const;
};
