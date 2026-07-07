cmake_minimum_required(VERSION 3.22)

project(AutoMenuAIPro VERSION 0.4.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(JUCE_DIR "D:/JUCE8")
add_subdirectory("${JUCE_DIR}" "${CMAKE_BINARY_DIR}/JUCE")

juce_add_gui_app(AutoMenuAIPro
    PRODUCT_NAME "AutoMenuAIPro"
    COMPANY_NAME "Hai Pham"
    BUNDLE_ID "com.haipham.automenuaipro"
)

target_sources(AutoMenuAIPro PRIVATE
    App/main.cpp
    App/Application.cpp
    App/MainWindow.cpp

    Core/AutoMenuCore.cpp

    Audio/AudioEngine.cpp
    Audio/AudioRingBuffer.cpp

    Analysis/AnalysisManager.cpp
    Analysis/FFTAnalyzer.cpp
    Analysis/PitchDetector.cpp
    Analysis/KeyDetector.cpp
    Analysis/BPMDetector.cpp
    Analysis/NoiseGate.cpp
    Analysis/ToneStabilizer.cpp
    Analysis/ToneEngine.cpp

    Core/MacroManager.cpp
    Core/AutomationEngine.cpp
    Core/SuggestionEngine.cpp
    
    Workspace/TonePresetMapping.cpp

    Cubase/MidiDeviceManager.cpp
    Cubase/MidiCommandQueue.cpp
    Cubase/CubaseManager.cpp

    UI/Dashboard/Dashboard.cpp
    UI/Theme/AppTheme.cpp
    UI/Settings/SettingsPanel.cpp

    UI/Header/HeaderComponent.cpp
    UI/Panels/MacroPanel/MacroPanelComponent.cpp
    UI/Panels/MixerPanel/MixerPanelComponent.cpp
    UI/Panels/TonePanel/ToneDetectorComponent.cpp
    UI/Panels/OutputMeterPanel/OutputMeterComponent.cpp
    UI/Footer/FooterComponent.cpp
)

target_include_directories(AutoMenuAIPro PRIVATE
    App
    Core
    Audio
    Analysis
    Utils
    Workspace
    UI
    Cubase
    UI/Dashboard
    UI/Theme
    UI/Header
    UI/Panels/MacroPanel
    UI/Panels/MixerPanel
    UI/Panels/TonePanel
    UI/Panels/OutputMeterPanel
    UI/Footer
    UI/Settings
)

target_link_libraries(AutoMenuAIPro PRIVATE
    juce::juce_gui_extra
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_events
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_audio_devices
)

target_compile_definitions(AutoMenuAIPro PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
)
