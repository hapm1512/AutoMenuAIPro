cmake_minimum_required(VERSION 3.24)

project(VocalSuiteUltraPro VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT DEFINED JUCE_DIR)
    set(JUCE_DIR "D:/JUCE8")
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

set(VSUP_SOURCES
    Source/PluginEntry.cpp
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp

    Source/Parameters/ParameterLayout.cpp

    Source/Core/PluginState.cpp

    Source/GUI/Theme.cpp
    Source/GUI/ChannelStripEditor.cpp
    Source/GUI/Components/Knob.cpp
    Source/GUI/Components/ModulePanel.cpp
    Source/GUI/Components/LevelMeter.cpp
    Source/GUI/Components/TopBar.cpp
    Source/GUI/Components/BottomMeterBar.cpp

    Source/DSP/Engine/DspEngine.cpp
    Source/DSP/PreampModule.cpp
    Source/DSP/EqModule.cpp
    Source/DSP/GateModule.cpp
    Source/DSP/DeEsserModule.cpp
    Source/DSP/CompressorModule.cpp
    Source/DSP/SaturationModule.cpp
    Source/DSP/HiEndModule.cpp
    Source/DSP/OutputModule.cpp
)

target_sources(VocalSuiteUltraPro PRIVATE ${VSUP_SOURCES})

juce_generate_juce_header(VocalSuiteUltraPro)

target_include_directories(VocalSuiteUltraPro
PRIVATE
    Source
    Source/Core
    Source/GUI
    Source/GUI/Components
    Source/DSP
    Source/DSP/Engine
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
