#include "WorkspaceState.h"

namespace AutoMenu
{
    WorkspaceState::WorkspaceState()
    {
        resetToDefaults();
    }

    void WorkspaceState::resetToDefaults()
    {
        workspaceName = "Live Vocal";
        lastMidiOutput.clear();
        lastAudioDevice.clear();

        performanceMode = false;
        autoStartAudio = true;
        autoConnectMidi = true;
        restoreOnStartup = true;

        guiScale = 0.85f;
    }

    juce::File WorkspaceState::getConfigFile()
    {
        auto dir = juce::File::getSpecialLocation(
                       juce::File::userApplicationDataDirectory)
                       .getChildFile("AutoMenuAIPro");

        dir.createDirectory();

        return dir.getChildFile("workspace_state.json");
    }

    bool WorkspaceState::load()
    {
        auto file = getConfigFile();

        if (!file.existsAsFile())
            return false;

        auto parsed = juce::JSON::parse(file);
        auto* obj = parsed.getDynamicObject();

        if (obj == nullptr)
            return false;

        auto getString = [obj](const juce::Identifier& key,
                               const juce::String& fallback)
        {
            auto v = obj->getProperty(key);
            return v.isVoid() ? fallback : v.toString();
        };

        auto getBool = [obj](const juce::Identifier& key,
                             bool fallback)
        {
            auto v = obj->getProperty(key);
            return v.isVoid() ? fallback : static_cast<bool>(v);
        };

        auto getFloat = [obj](const juce::Identifier& key,
                              float fallback)
        {
            auto v = obj->getProperty(key);
            return v.isVoid() ? fallback : static_cast<float>((double)v);
        };

        workspaceName = getString("workspaceName", workspaceName);
        lastMidiOutput = getString("lastMidiOutput", lastMidiOutput);
        lastAudioDevice = getString("lastAudioDevice", lastAudioDevice);

        performanceMode = getBool("performanceMode", performanceMode);
        autoStartAudio = getBool("autoStartAudio", autoStartAudio);
        autoConnectMidi = getBool("autoConnectMidi", autoConnectMidi);
        restoreOnStartup = getBool("restoreOnStartup", restoreOnStartup);

        guiScale = getFloat("guiScale", guiScale);
        guiScale = juce::jlimit(0.70f, 1.20f, guiScale);

        return true;
    }

    bool WorkspaceState::save() const
    {
        auto obj = std::make_unique<juce::DynamicObject>();

        obj->setProperty("workspaceName", workspaceName);
        obj->setProperty("lastMidiOutput", lastMidiOutput);
        obj->setProperty("lastAudioDevice", lastAudioDevice);

        obj->setProperty("performanceMode", performanceMode);
        obj->setProperty("autoStartAudio", autoStartAudio);
        obj->setProperty("autoConnectMidi", autoConnectMidi);
        obj->setProperty("restoreOnStartup", restoreOnStartup);

        obj->setProperty("guiScale", guiScale);

        return getConfigFile().replaceWithText(
            juce::JSON::toString(juce::var(obj.release()), true));
    }
}