#include "PresetManager.h"

PresetManager::PresetManager (juce::AudioProcessorValueTreeState& stateToUse)
    : apvts (stateToUse)
{
    buildFactoryPresets();
    slotA = copyState();
    slotB = copyState();
}

int PresetManager::getNumFactoryPresets() const noexcept
{
    return (int) factoryPresets.size();
}

int PresetManager::getCurrentFactoryIndex() const noexcept
{
    return currentFactoryIndex;
}

juce::String PresetManager::getCurrentPresetName() const
{
    return currentPresetName;
}

juce::StringArray PresetManager::getFactoryPresetNames() const
{
    juce::StringArray names;
    for (const auto& p : factoryPresets)
        names.add (p.name);
    return names;
}

void PresetManager::loadFactoryPreset (int index)
{
    if (factoryPresets.empty())
        return;

    index = juce::jlimit (0, (int) factoryPresets.size() - 1, index);
    pushUndoState();

    const auto& preset = factoryPresets[(size_t) index];
    for (const auto& value : preset.values)
        applyValue (value.first, value.second);

    currentFactoryIndex = index;
    currentPresetName = preset.name;
}

void PresetManager::loadNextFactoryPreset()
{
    loadFactoryPreset ((currentFactoryIndex + 1) % juce::jmax (1, getNumFactoryPresets()));
}

void PresetManager::loadPreviousFactoryPreset()
{
    const int count = juce::jmax (1, getNumFactoryPresets());
    loadFactoryPreset ((currentFactoryIndex + count - 1) % count);
}

void PresetManager::captureA()
{
    slotA = copyState();
}

void PresetManager::captureB()
{
    slotB = copyState();
}

void PresetManager::copyAToB()
{
    slotB = slotA.createCopy();
}

void PresetManager::swapAB()
{
    auto temp = slotA.createCopy();
    slotA = slotB.createCopy();
    slotB = temp;
    replaceState (slotA);
    currentPresetName = "A/B Swapped";
}

void PresetManager::recallA()
{
    pushUndoState();
    replaceState (slotA);
    currentPresetName = "A Slot";
}

void PresetManager::recallB()
{
    pushUndoState();
    replaceState (slotB);
    currentPresetName = "B Slot";
}

bool PresetManager::saveUserPreset (const juce::String& presetName)
{
    auto dir = getPresetDirectory();
    if (! dir.exists())
        dir.createDirectory();

    auto safeName = presetName.isNotEmpty() ? presetName : currentPresetName;
    safeName = safeName.retainCharacters ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 _-.").trim();
    if (safeName.isEmpty())
        safeName = "User Preset";

    auto file = dir.getChildFile (safeName).withFileExtension ("vspreset");
    if (auto xml = copyState().createXml())
        return file.replaceWithText (xml->toString(), false, false, "UTF-8");

    return false;
}

bool PresetManager::loadUserPreset (const juce::File& presetFile)
{
    if (! presetFile.existsAsFile())
        return false;

    auto xml = juce::XmlDocument::parse (presetFile);
    if (xml == nullptr)
        return false;

    auto tree = juce::ValueTree::fromXml (*xml);
    if (! tree.isValid())
        return false;

    pushUndoState();
    replaceState (tree);
    currentPresetName = presetFile.getFileNameWithoutExtension();
    return true;
}

juce::File PresetManager::getPresetDirectory() const
{
    return juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
        .getChildFile ("Hai Audio")
        .getChildFile ("Vocal Suite Ultra Pro")
        .getChildFile ("Presets");
}

void PresetManager::pushUndoState()
{
    undoStack.add (copyState());
    redoStack.clear();

    while (undoStack.size() > 24)
        undoStack.remove (0);
}

bool PresetManager::undo()
{
    if (undoStack.isEmpty())
        return false;

    redoStack.add (copyState());
    auto state = undoStack.getLast();
    undoStack.remove (undoStack.size() - 1);
    replaceState (state);
    currentPresetName = "Undo";
    return true;
}

bool PresetManager::redo()
{
    if (redoStack.isEmpty())
        return false;

    undoStack.add (copyState());
    auto state = redoStack.getLast();
    redoStack.remove (redoStack.size() - 1);
    replaceState (state);
    currentPresetName = "Redo";
    return true;
}

juce::ValueTree PresetManager::copyState() const
{
    return apvts.copyState();
}

void PresetManager::replaceState (const juce::ValueTree& tree)
{
    if (tree.isValid())
        apvts.replaceState (tree.createCopy());
}

void PresetManager::applyValue (const juce::String& paramID, float plainValue)
{
    if (auto* p = apvts.getParameter (paramID))
    {
        const float norm = p->convertTo0to1 (plainValue);
        p->beginChangeGesture();
        p->setValueNotifyingHost (juce::jlimit (0.0f, 1.0f, norm));
        p->endChangeGesture();
    }
}

void PresetManager::buildFactoryPresets()
{
    factoryPresets.clear();

    factoryPresets.push_back ({ "Modern Vocal Pop", "VOCAL", {
        { "preampInputTrim", 1.5f }, { "preampDrive", 24.0f }, { "preampTransformer", 34.0f }, { "preampTube", 18.0f },
        { "gateThreshold", -46.0f }, { "gateRange", 18.0f }, { "gateRelease", 135.0f },
        { "eqHpfFreq", 82.0f }, { "eqLowGain", -1.2f }, { "eqMidGain", 1.0f }, { "eqHighGain", 2.2f }, { "eqAirGain", 3.0f },
        { "deessFreq", 7200.0f }, { "deessThreshold", -31.0f }, { "deessRange", 5.5f },
        { "comp1176Amount", 42.0f }, { "comp1176Ratio", 4.0f }, { "compLA2APeak", 37.0f }, { "compParallel", 78.0f },
        { "satDrive", 18.0f }, { "satTape", 20.0f }, { "satTube", 14.0f }, { "satAir", 28.0f }, { "satMix", 64.0f },
        { "outCeiling", -1.0f }, { "outThreshold", -5.5f }, { "outSoftClip", 18.0f }
    }});

    factoryPresets.push_back ({ "Live Clean Vocal", "LIVE", {
        { "preampInputTrim", 0.0f }, { "preampDrive", 12.0f }, { "preampTransformer", 18.0f }, { "preampTube", 8.0f },
        { "gateThreshold", -42.0f }, { "gateRange", 22.0f }, { "gateLookAhead", 2.0f }, { "gateBreath", 65.0f },
        { "eqHpfFreq", 105.0f }, { "eqLowGain", -2.0f }, { "eqMidGain", 0.6f }, { "eqHighGain", 1.4f }, { "eqAirGain", 1.8f },
        { "deessFreq", 6800.0f }, { "deessThreshold", -29.0f }, { "deessRange", 4.0f },
        { "comp1176Amount", 28.0f }, { "compLA2APeak", 25.0f }, { "compParallel", 86.0f },
        { "satDrive", 8.0f }, { "satMix", 42.0f }, { "outCeiling", -1.2f }, { "outThreshold", -4.0f }
    }});

    factoryPresets.push_back ({ "Warm Ballad Lead", "VOCAL", {
        { "preampDrive", 30.0f }, { "preampTransformer", 44.0f }, { "preampTube", 32.0f }, { "preampMemory", 30.0f },
        { "gateThreshold", -50.0f }, { "gateRange", 12.0f }, { "gateRelease", 185.0f },
        { "eqHpfFreq", 70.0f }, { "eqLowGain", 1.0f }, { "eqLowMidGain", -1.4f }, { "eqHighGain", 1.1f }, { "eqAirGain", 2.0f },
        { "deessFreq", 7600.0f }, { "deessRange", 3.8f },
        { "comp1176Amount", 22.0f }, { "compLA2APeak", 48.0f }, { "compLA2AGain", 1.5f }, { "compParallel", 72.0f },
        { "satTape", 34.0f }, { "satTube", 30.0f }, { "satLowThick", 28.0f }, { "satMix", 70.0f },
        { "hiWarm", 52.0f }, { "hiWidth", 20.0f }, { "outCeiling", -1.0f }
    }});

    factoryPresets.push_back ({ "Bright Air Vocal", "AIR", {
        { "preampDrive", 16.0f }, { "preampTransformer", 20.0f }, { "preampTube", 10.0f },
        { "eqHpfFreq", 90.0f }, { "eqLowGain", -1.6f }, { "eqHighGain", 2.8f }, { "eqAirGain", 5.2f }, { "eqAirFreq", 12500.0f },
        { "deessFreq", 7900.0f }, { "deessThreshold", -28.0f }, { "deessRange", 6.5f },
        { "comp1176Amount", 34.0f }, { "compLA2APeak", 30.0f }, { "compParallel", 82.0f },
        { "satPresence", 22.0f }, { "satAir", 46.0f }, { "satMix", 56.0f },
        { "hiWarm", 68.0f }, { "hiWidth", 26.0f }, { "outCeiling", -1.0f }, { "outSoftClip", 12.0f }
    }});
}
