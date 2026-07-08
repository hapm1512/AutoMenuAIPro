cmake_minimum_required(VERSION 3.24)

project(VocalSuiteUltraPro VERSION 4.5.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT DEFINED JUCE_DIR)
    set(JUCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../JUCE")
endif()

add_subdirectory(${JUCE_DIR} JUCE)

juce_add_plugin(VocalSuiteUltraPro
    COMPANY_NAME "Hai Audio"
    PRODUCT_NAME "Vocal Suite Ultra Pro"
    PLUGIN_MANUFACTURER_CODE HaiA
    PLUGIN_CODE Vsup
    FORMATS VST3 Standalone
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
)

juce_generate_juce_header(VocalSuiteUltraPro)

target_sources(VocalSuiteUltraPro
PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/PluginEntry.cpp

    Source/Parameters/ParameterLayout.cpp
    Source/Core/PluginState.cpp
    Source/Core/PresetManager.cpp

    Source/GUI/Theme.cpp
    Source/GUI/ChannelStripEditor.cpp
    Source/GUI/Components/Knob.cpp
    Source/GUI/Components/ModulePanel.cpp
    Source/GUI/Components/LevelMeter.cpp
    Source/GUI/Components/TopBar.cpp
    Source/GUI/Components/BottomMeterBar.cpp
    Source/GUI/Controls/PopupValue.cpp
    Source/GUI/Controls/TooltipManager.cpp
    Source/GUI/LookAndFeel/MainLookAndFeel.cpp

    Source/DSP/Engine/DspEngine.cpp
    Source/DSP/DspUtilities.h
    Source/DSP/ProfessionalMetering.h
    Source/DSP/PreampModule.cpp
    Source/DSP/EqModule.cpp
    Source/DSP/GateModule.cpp
    Source/DSP/DeEsserModule.cpp
    Source/DSP/CompressorModule.cpp
    Source/DSP/SaturationModule.cpp
    Source/DSP/HiEndModule.cpp
    Source/DSP/OutputModule.cpp
)

target_include_directories(VocalSuiteUltraPro
PRIVATE
    Source
    Source/Core
    Source/DSP
    Source/DSP/Engine
    Source/GUI
    Source/GUI/Components
    Source/GUI/Controls
    Source/GUI/Layout
    Source/GUI/LookAndFeel
    Source/Parameters
)

target_compile_definitions(VocalSuiteUltraPro
PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(VocalSuiteUltraPro
PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_dsp
PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
