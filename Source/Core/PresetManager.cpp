#include "PresetManager.h"

namespace
{
    constexpr const char* kAuthor = "Hai Pham Audio";
    constexpr const char* kVersion = "1.0";

    using ValueList = std::vector<std::pair<juce::String, float>>;

    ValueList baseVocal()
    {
        return {
            { "preampOn", 1.0f }, { "eqOn", 1.0f }, { "gateOn", 1.0f }, { "deesserOn", 1.0f },
            { "compressorOn", 1.0f }, { "saturationOn", 1.0f }, { "hiendOn", 1.0f }, { "outputOn", 1.0f },
            { "preampInputTrim", 0.0f }, { "preampDrive", 18.0f }, { "preampHeadroom", 14.0f },
            { "preampTransformer", 24.0f }, { "preampTube", 16.0f }, { "preampHarmonic", 22.0f },
            { "preampBias", 0.0f }, { "preampMemory", 12.0f }, { "preampStereo", 8.0f },
            { "preampCrosstalk", 3.0f }, { "preampNoise", 0.0f }, { "preampAutoGain", 70.0f },
            { "preampPeakProtect", 100.0f }, { "preampMix", 100.0f }, { "preampOutput", 0.0f },
            { "gateThreshold", -48.0f }, { "gateRange", -18.0f }, { "gateHyst", 6.0f },
            { "gateAttack", 1.2f }, { "gateHold", 55.0f }, { "gateRelease", 130.0f },
            { "gateLookAhead", 2.0f }, { "gateKnee", 8.0f }, { "gateDepth", 80.0f },
            { "gateRmsMix", 65.0f }, { "gateAdaptive", 35.0f }, { "gateBreath", 45.0f },
            { "gateStereoLink", 80.0f }, { "gateAutoMakeup", 10.0f },
            { "eqHpfFreq", 80.0f }, { "eqLowGain", 0.0f }, { "eqLowMidGain", -1.0f },
            { "eqMidGain", 0.5f }, { "eqHighGain", 1.2f }, { "eqAirGain", 1.8f },
            { "eqDynAmount", 20.0f }, { "eqAutoGain", 70.0f }, { "eqOutput", 0.0f },
            { "deessFreq", 7200.0f }, { "deessThreshold", -30.0f }, { "deessRange", -7.0f },
            { "deessRatio", 4.0f }, { "deessAttack", 0.7f }, { "deessRelease", 75.0f },
            { "deessFocus", 65.0f }, { "deessSplit", 80.0f }, { "deessStereoLink", 70.0f },
            { "compInput", 0.0f }, { "comp1176Amount", 30.0f }, { "comp1176Ratio", 4.0f },
            { "compAttack", 2.2f }, { "compRelease", 85.0f }, { "compLA2APeak", 25.0f },
            { "compLA2AGain", 0.0f }, { "compParallel", 85.0f }, { "compSideHp", 90.0f },
            { "compAnalog", 25.0f }, { "compAutoGain", 55.0f }, { "compOutput", 0.0f },
            { "satDrive", 12.0f }, { "satTone", 12.0f }, { "satTape", 18.0f }, { "satTube", 14.0f },
            { "satTransformer", 18.0f }, { "satEven", 15.0f }, { "satOdd", 10.0f }, { "satBias", 0.0f },
            { "satPresence", 12.0f }, { "satAir", 14.0f }, { "satLowThick", 8.0f },
            { "satWidth", 105.0f }, { "satCrosstalk", 3.0f }, { "satAutoGain", 70.0f },
            { "satMix", 65.0f }, { "satOutput", 0.0f },
            { "hiWarm", 25.0f }, { "hiWidth", 105.0f }, { "hiTame", 20.0f }, { "hiGlue", 15.0f },
            { "outTrim", 0.0f }, { "outCeiling", -1.0f }, { "outThreshold", -3.0f },
            { "outSoftClip", 16.0f }, { "outISP", 100.0f }, { "outLufsComp", 0.0f },
            { "outDither", 0.0f }, { "outRelease", 60.0f }, { "outMix", 100.0f }
        };
    }

    ValueList with (ValueList values, std::initializer_list<std::pair<juce::String, float>> edits)
    {
        for (const auto& edit : edits)
        {
            bool found = false;
            for (auto& v : values)
            {
                if (v.first == edit.first)
                {
                    v.second = edit.second;
                    found = true;
                    break;
                }
            }
            if (! found)
                values.push_back (edit);
        }
        return values;
    }
}

PresetManager::PresetManager (juce::AudioProcessorValueTreeState& stateToUse)
    : apvts (stateToUse)
{
    buildFactoryPresets();
    slotA = copyState();
    slotB = copyState();
}

int PresetManager::getNumFactoryPresets() const noexcept { return (int) factoryPresets.size(); }
int PresetManager::getCurrentFactoryIndex() const noexcept { return currentFactoryIndex; }
juce::String PresetManager::getCurrentPresetName() const { return currentPresetName; }
juce::String PresetManager::getCurrentPresetCategory() const { return currentCategory; }
juce::String PresetManager::getCurrentPresetAuthor() const { return kAuthor; }
juce::String PresetManager::getCurrentPresetDescription() const { return currentDescription; }

juce::StringArray PresetManager::getFactoryPresetNames() const
{
    juce::StringArray names;
    for (const auto& p : factoryPresets)
        names.add (p.name);
    return names;
}

juce::StringArray PresetManager::getFactoryPresetCategories() const
{
    juce::StringArray categories;
    for (const auto& p : factoryPresets)
        categories.addIfNotAlreadyThere (p.category);
    return categories;
}

juce::StringArray PresetManager::getFactoryPresetNamesForCategory (const juce::String& categoryName) const
{
    juce::StringArray names;
    for (const auto& p : factoryPresets)
        if (p.category.equalsIgnoreCase (categoryName))
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
    currentCategory = preset.category;
    currentDescription = preset.description;
    addRecentPreset (preset.name);
}

bool PresetManager::loadFactoryPresetByName (const juce::String& presetName)
{
    for (int i = 0; i < (int) factoryPresets.size(); ++i)
    {
        if (factoryPresets[(size_t) i].name.equalsIgnoreCase (presetName))
        {
            loadFactoryPreset (i);
            return true;
        }
    }
    return false;
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

void PresetManager::setFavorite (const juce::String& presetName, bool shouldBeFavorite)
{
    if (presetName.isEmpty())
        return;

    if (shouldBeFavorite)
        favorites.addIfNotAlreadyThere (presetName);
    else
        favorites.removeString (presetName);
}

bool PresetManager::isFavorite (const juce::String& presetName) const
{
    return favorites.contains (presetName);
}

juce::StringArray PresetManager::getFavoritePresetNames() const { return favorites; }
juce::StringArray PresetManager::getRecentPresetNames() const { return recentPresets; }

juce::StringArray PresetManager::searchFactoryPresets (const juce::String& searchText) const
{
    juce::StringArray results;
    const auto q = searchText.trim().toLowerCase();
    if (q.isEmpty())
        return getFactoryPresetNames();

    for (const auto& p : factoryPresets)
    {
        const auto haystack = (p.name + " " + p.category + " " + p.description + " " + p.tags.joinIntoString (" ")).toLowerCase();
        if (haystack.contains (q))
            results.add (p.name);
    }
    return results;
}

void PresetManager::captureA() { slotA = copyState(); }
void PresetManager::captureB() { slotB = copyState(); }
void PresetManager::copyAToB() { slotB = slotA.createCopy(); }

void PresetManager::swapAB()
{
    auto temp = slotA.createCopy();
    slotA = slotB.createCopy();
    slotB = temp;
    replaceState (slotA);
    currentPresetName = "A/B Swapped";
    currentCategory = "Compare";
}

void PresetManager::recallA()
{
    pushUndoState();
    replaceState (slotA);
    currentPresetName = "A Slot";
    currentCategory = "Compare";
}

void PresetManager::recallB()
{
    pushUndoState();
    replaceState (slotB);
    currentPresetName = "B Slot";
    currentCategory = "Compare";
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
    return exportCurrentPreset (file);
}

bool PresetManager::loadUserPreset (const juce::File& presetFile)
{
    if (! presetFile.existsAsFile())
        return false;

    auto xml = juce::XmlDocument::parse (presetFile);
    if (xml == nullptr)
        return false;

    if (xml->hasTagName ("VocalSuitePreset"))
    {
        auto* stateXml = xml->getChildByName ("PARAMETERS");
        if (stateXml == nullptr)
            stateXml = xml.get();

        auto tree = juce::ValueTree::fromXml (*stateXml);
        if (! tree.isValid())
            return false;

        pushUndoState();
        replaceState (tree);
        currentPresetName = xml->getStringAttribute ("name", presetFile.getFileNameWithoutExtension());
        currentCategory = xml->getStringAttribute ("category", "User");
        currentDescription = xml->getStringAttribute ("description", "User preset.");
        addRecentPreset (currentPresetName);
        return true;
    }

    auto tree = juce::ValueTree::fromXml (*xml);
    if (! tree.isValid())
        return false;

    pushUndoState();
    replaceState (tree);
    currentPresetName = presetFile.getFileNameWithoutExtension();
    currentCategory = "User";
    currentDescription = "User preset.";
    addRecentPreset (currentPresetName);
    return true;
}

bool PresetManager::exportCurrentPreset (const juce::File& destinationFile) const
{
    if (auto xml = copyState().createXml())
    {
        juce::XmlElement root ("VocalSuitePreset");
        root.setAttribute ("name", currentPresetName);
        root.setAttribute ("category", currentCategory);
        root.setAttribute ("author", kAuthor);
        root.setAttribute ("version", kVersion);
        root.setAttribute ("pluginVersion", "4.5.0");
        root.setAttribute ("description", currentDescription);
        root.addChildElement (xml.release());
        return destinationFile.replaceWithText (root.toString(), false, false, "UTF-8");
    }
    return false;
}

juce::File PresetManager::getPresetDirectory() const
{
    return juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
        .getChildFile ("Hai Pham Audio")
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
    currentCategory = "History";
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
    currentCategory = "History";
    return true;
}

juce::ValueTree PresetManager::copyState() const { return apvts.copyState(); }

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

void PresetManager::addRecentPreset (const juce::String& presetName)
{
    recentPresets.removeString (presetName);
    recentPresets.insert (0, presetName);
    while (recentPresets.size() > 12)
        recentPresets.remove (recentPresets.size() - 1);
}

juce::ValueTree PresetManager::createPresetTree (const FactoryPreset& preset) const
{
    juce::ValueTree tree ("VocalSuiteFactoryPreset");
    tree.setProperty ("name", preset.name, nullptr);
    tree.setProperty ("category", preset.category, nullptr);
    tree.setProperty ("author", preset.author, nullptr);
    tree.setProperty ("version", preset.version, nullptr);
    tree.setProperty ("description", preset.description, nullptr);

    for (const auto& value : preset.values)
    {
        juce::ValueTree param ("Parameter");
        param.setProperty ("id", value.first, nullptr);
        param.setProperty ("value", value.second, nullptr);
        tree.addChild (param, -1, nullptr);
    }
    return tree;
}

void PresetManager::buildFactoryPresets()
{
    factoryPresets.clear();

    auto add = [this] (juce::String name, juce::String category, juce::String description,
                       std::initializer_list<const char*> tags, ValueList values)
    {
        juce::StringArray tagArray;
        for (auto* tag : tags)
            tagArray.add (tag);
        factoryPresets.push_back ({ name, category, kAuthor, kVersion, description, tagArray, std::move (values) });
    };

    const auto b = baseVocal();

    add ("Clean Vocal", "Studio", "Transparent vocal chain for natural lead vocals.", { "clean", "studio", "natural" },
         with (b, { { "preampDrive", 10.0f }, { "eqHighGain", 0.8f }, { "eqAirGain", 1.2f }, { "comp1176Amount", 22.0f }, { "compLA2APeak", 18.0f }, { "satMix", 35.0f }, { "outThreshold", -2.5f } }));

    add ("Modern Pop", "Studio", "Bright modern pop vocal with polished compression and air.", { "pop", "bright", "radio" },
         with (b, { { "preampDrive", 22.0f }, { "eqHpfFreq", 90.0f }, { "eqLowMidGain", -1.8f }, { "eqHighGain", 2.2f }, { "eqAirGain", 3.4f }, { "deessRange", -8.5f }, { "comp1176Amount", 42.0f }, { "compLA2APeak", 34.0f }, { "satAir", 30.0f }, { "satPresence", 24.0f }, { "outThreshold", -5.0f } }));

    add ("Warm Ballad", "Studio", "Warm smooth lead vocal for ballad and soft music.", { "warm", "ballad", "smooth" },
         with (b, { { "preampTransformer", 42.0f }, { "preampTube", 34.0f }, { "eqHpfFreq", 70.0f }, { "eqLowGain", 1.2f }, { "eqHighGain", 0.7f }, { "comp1176Amount", 20.0f }, { "compLA2APeak", 48.0f }, { "satTape", 38.0f }, { "satTube", 32.0f }, { "satLowThick", 30.0f }, { "hiWarm", 55.0f } }));

    add ("Bright Female", "Studio", "Clear bright female vocal with controlled sibilance.", { "female", "bright", "air" },
         with (b, { { "eqHpfFreq", 105.0f }, { "eqLowGain", -1.3f }, { "eqHighGain", 2.8f }, { "eqAirGain", 4.6f }, { "eqAirFreq", 14500.0f }, { "deessFreq", 7900.0f }, { "deessRange", -10.0f }, { "satAir", 42.0f }, { "satPresence", 22.0f } }));

    add ("Male Rock", "Studio", "Forward male rock vocal with grit and stable level.", { "male", "rock", "grit" },
         with (b, { { "preampDrive", 30.0f }, { "preampTransformer", 46.0f }, { "eqLowMidGain", -1.2f }, { "eqMidGain", 1.5f }, { "eqHighMidGain", 2.2f }, { "comp1176Amount", 55.0f }, { "comp1176Ratio", 8.0f }, { "satDrive", 30.0f }, { "satOdd", 25.0f }, { "satMix", 78.0f }, { "outSoftClip", 25.0f } }));

    add ("Acoustic Vocal", "Studio", "Gentle acoustic vocal with preserved dynamics.", { "acoustic", "natural", "soft" },
         with (b, { { "gateRange", -12.0f }, { "gateBreath", 70.0f }, { "eqHpfFreq", 75.0f }, { "eqAirGain", 1.8f }, { "comp1176Amount", 18.0f }, { "compLA2APeak", 26.0f }, { "satTape", 18.0f }, { "satMix", 45.0f }, { "outThreshold", -2.5f } }));

    add ("Podcast Voice", "Podcast", "Clean spoken voice with controlled low end and consistent loudness.", { "podcast", "speech", "voice" },
         with (b, { { "eqHpfFreq", 95.0f }, { "eqLowMidGain", -2.2f }, { "eqMidGain", 1.0f }, { "eqHighGain", 1.0f }, { "deesserOn", 0.0f }, { "comp1176Amount", 30.0f }, { "compLA2APeak", 35.0f }, { "satTape", 8.0f }, { "outLufsComp", 2.0f }, { "outThreshold", -6.0f } }));

    add ("Broadcast FM", "Broadcast", "Dense broadcast-style voice with strong level control.", { "broadcast", "fm", "voice" },
         with (b, { { "preampDrive", 24.0f }, { "eqHpfFreq", 100.0f }, { "eqLowMidGain", -2.8f }, { "eqMidGain", 1.8f }, { "eqHighGain", 1.4f }, { "comp1176Amount", 48.0f }, { "compLA2APeak", 52.0f }, { "satTube", 22.0f }, { "satLowThick", 16.0f }, { "outLufsComp", 3.5f }, { "outThreshold", -7.0f } }));

    add ("Live Clean", "Live", "Safe live vocal preset with natural noise control.", { "live", "clean", "stage" },
         with (b, { { "gateThreshold", -42.0f }, { "gateRange", -24.0f }, { "gateRelease", 155.0f }, { "gateBreath", 65.0f }, { "eqHpfFreq", 110.0f }, { "eqLowGain", -1.5f }, { "eqAirGain", 1.2f }, { "comp1176Amount", 28.0f }, { "compLA2APeak", 22.0f }, { "satMix", 35.0f }, { "outCeiling", -1.2f } }));

    add ("Live Powerful", "Live", "Stronger live lead vocal with compression and presence.", { "live", "power", "lead" },
         with (b, { { "gateThreshold", -40.0f }, { "gateRange", -24.0f }, { "eqHpfFreq", 115.0f }, { "eqMidGain", 1.1f }, { "eqHighGain", 2.0f }, { "comp1176Amount", 46.0f }, { "compLA2APeak", 32.0f }, { "satPresence", 26.0f }, { "satMix", 58.0f }, { "outThreshold", -5.5f } }));

    add ("Live Karaoke", "Live", "Karaoke vocal chain with bright top and limiter safety.", { "live", "karaoke", "bright" },
         with (b, { { "preampInputTrim", 1.0f }, { "gateThreshold", -44.0f }, { "eqHpfFreq", 100.0f }, { "eqLowGain", -1.0f }, { "eqHighGain", 2.4f }, { "eqAirGain", 3.0f }, { "deessRange", -8.0f }, { "comp1176Amount", 36.0f }, { "compLA2APeak", 28.0f }, { "satAir", 28.0f }, { "outThreshold", -5.0f } }));

    add ("Live Band", "Live", "Stage preset for band mix with tighter gate and mid focus.", { "live", "band", "stage" },
         with (b, { { "gateThreshold", -38.0f }, { "gateRange", -30.0f }, { "gateHold", 45.0f }, { "gateRelease", 120.0f }, { "eqHpfFreq", 125.0f }, { "eqLowMidGain", -2.4f }, { "eqMidGain", 1.6f }, { "eqHighMidGain", 1.8f }, { "comp1176Amount", 44.0f }, { "outCeiling", -1.5f } }));

    add ("Small Stage", "Live", "Compact stage preset with feedback-safe tone shaping.", { "live", "small stage", "safe" },
         with (b, { { "gateThreshold", -39.0f }, { "gateRange", -26.0f }, { "eqHpfFreq", 130.0f }, { "eqLowMidGain", -3.0f }, { "eqHighMidGain", 1.0f }, { "eqAirGain", 0.8f }, { "comp1176Amount", 34.0f }, { "satMix", 30.0f } }));

    add ("Large Stage", "Live", "Large stage preset with wider image and polished highs.", { "live", "large stage", "wide" },
         with (b, { { "gateThreshold", -43.0f }, { "gateRange", -22.0f }, { "eqHpfFreq", 100.0f }, { "eqHighGain", 1.8f }, { "eqAirGain", 2.6f }, { "hiWidth", 118.0f }, { "satWidth", 115.0f }, { "comp1176Amount", 38.0f }, { "outThreshold", -4.5f } }));

    add ("Speech Clear", "Podcast", "Clear speech preset focused on intelligibility.", { "speech", "clear", "dialog" },
         with (b, { { "eqHpfFreq", 105.0f }, { "eqLowGain", -1.8f }, { "eqLowMidGain", -2.0f }, { "eqMidGain", 1.8f }, { "eqHighGain", 1.4f }, { "deessRange", -5.0f }, { "comp1176Amount", 28.0f }, { "compLA2APeak", 30.0f }, { "satMix", 25.0f }, { "outLufsComp", 2.0f } }));

    add ("Narration", "Podcast", "Warm narration preset for long-form spoken content.", { "narration", "speech", "warm" },
         with (b, { { "preampTransformer", 35.0f }, { "preampTube", 24.0f }, { "eqHpfFreq", 80.0f }, { "eqLowGain", 0.8f }, { "eqLowMidGain", -1.0f }, { "eqAirGain", 1.0f }, { "compLA2APeak", 42.0f }, { "satTape", 28.0f }, { "satLowThick", 18.0f }, { "hiWarm", 45.0f } }));

    add ("Vintage Tube", "Creative", "Vintage tube color with rounded transient response.", { "creative", "vintage", "tube" },
         with (b, { { "preampDrive", 38.0f }, { "preampTube", 50.0f }, { "preampTransformer", 42.0f }, { "eqHighGain", -0.5f }, { "eqAirGain", 0.8f }, { "compLA2APeak", 45.0f }, { "satTube", 58.0f }, { "satEven", 44.0f }, { "satMix", 82.0f }, { "outSoftClip", 26.0f } }));

    add ("Tape Color", "Creative", "Tape-style saturation with thicker low mid character.", { "creative", "tape", "color" },
         with (b, { { "preampTransformer", 36.0f }, { "eqLowGain", 0.6f }, { "eqHighGain", -0.4f }, { "satTape", 70.0f }, { "satDrive", 34.0f }, { "satLowThick", 38.0f }, { "satCrosstalk", 10.0f }, { "satMix", 84.0f }, { "outSoftClip", 28.0f } }));

    add ("Air Vocal", "Creative", "Extra air and sheen for glossy vocal effects.", { "creative", "air", "shine" },
         with (b, { { "eqHighGain", 2.8f }, { "eqAirGain", 6.0f }, { "eqAirFreq", 15500.0f }, { "deessFreq", 8200.0f }, { "deessRange", -11.0f }, { "satPresence", 25.0f }, { "satAir", 65.0f }, { "hiWarm", 65.0f }, { "satMix", 62.0f } }));

    add ("Wide Vocal", "Creative", "Wider vocal image while keeping lead energy centered.", { "creative", "wide", "stereo" },
         with (b, { { "eqMS", 25.0f }, { "hiWidth", 135.0f }, { "satWidth", 140.0f }, { "satCrosstalk", 8.0f }, { "eqAirGain", 2.4f }, { "compParallel", 78.0f }, { "outCeiling", -1.0f } }));

    add ("Radio Voice", "Creative", "Hyped radio voice with compression and saturation.", { "creative", "radio", "effect" },
         with (b, { { "eqHpfFreq", 135.0f }, { "eqLowMidGain", -3.0f }, { "eqMidGain", 2.4f }, { "eqHighGain", 2.0f }, { "comp1176Amount", 60.0f }, { "comp1176Ratio", 10.0f }, { "compLA2APeak", 45.0f }, { "satDrive", 38.0f }, { "satTube", 32.0f }, { "outLufsComp", 4.0f }, { "outThreshold", -8.0f } }));

    add ("Lo-Fi", "Creative", "Lo-fi vocal color with darker top and stronger saturation.", { "creative", "lo-fi", "effect" },
         with (b, { { "eqHpfFreq", 150.0f }, { "eqLowGain", -1.4f }, { "eqHighGain", -3.0f }, { "eqAirGain", -5.0f }, { "comp1176Amount", 48.0f }, { "satDrive", 55.0f }, { "satTape", 62.0f }, { "satOdd", 35.0f }, { "satMix", 88.0f }, { "outSoftClip", 36.0f } }));

    add ("Rap Vocal", "Studio", "Fast aggressive rap vocal with tight dynamics and presence.", { "rap", "fast", "aggressive" },
         with (b, { { "gateThreshold", -44.0f }, { "eqHpfFreq", 95.0f }, { "eqLowMidGain", -1.6f }, { "eqMidGain", 1.6f }, { "eqHighMidGain", 2.4f }, { "eqAirGain", 2.2f }, { "comp1176Amount", 58.0f }, { "comp1176Ratio", 8.0f }, { "compAttack", 1.0f }, { "compRelease", 60.0f }, { "satPresence", 28.0f }, { "outThreshold", -6.0f } }));

    add ("Soft Female", "Studio", "Soft female vocal with controlled highs and smooth opto compression.", { "female", "soft", "smooth" },
         with (b, { { "eqHpfFreq", 95.0f }, { "eqHighGain", 1.8f }, { "eqAirGain", 3.2f }, { "deessFreq", 7600.0f }, { "deessRange", -9.0f }, { "comp1176Amount", 20.0f }, { "compLA2APeak", 42.0f }, { "satTube", 20.0f }, { "satMix", 50.0f } }));

    add ("Deep Male", "Studio", "Deep male vocal with low-end cleanup and warm density.", { "male", "deep", "warm" },
         with (b, { { "eqHpfFreq", 65.0f }, { "eqLowGain", 0.7f }, { "eqLowMidGain", -2.2f }, { "eqMidGain", 0.9f }, { "eqAirGain", 1.2f }, { "compLA2APeak", 40.0f }, { "satLowThick", 24.0f }, { "hiWarm", 42.0f } }));

    add ("Vocal Glue", "Studio", "General-purpose vocal glue preset for final chain polish.", { "studio", "glue", "polish" },
         with (b, { { "preampDrive", 18.0f }, { "eqLowMidGain", -1.3f }, { "eqHighGain", 1.6f }, { "comp1176Amount", 34.0f }, { "compLA2APeak", 34.0f }, { "compParallel", 80.0f }, { "hiGlue", 35.0f }, { "satMix", 55.0f }, { "outThreshold", -4.2f } }));

    currentPresetName = factoryPresets.front().name;
    currentCategory = factoryPresets.front().category;
    currentDescription = factoryPresets.front().description;
}
