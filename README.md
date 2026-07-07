# Vocal Suite Ultra Pro - CMake Sprint 01

## Mục tiêu
- Dựng project sạch bằng CMake.
- GUI Channel Strip.
- DSP shell.
- Build được trước.
- Dễ mở rộng Sprint sau.

## Build

```bat
cmake -B build -S . -DJUCE_DIR=C:/JUCE
cmake --build build --config Release
```

## Cấu trúc

```txt
Source/
  PluginProcessor.h/.cpp
  PluginEditor.h/.cpp

  Parameters/
    ParameterLayout.h/.cpp

  Core/
    PluginState.h/.cpp

  GUI/
    Theme.h/.cpp
    ChannelStripEditor.h/.cpp
    Components/
      Knob
      ModulePanel
      LevelMeter
      TopBar
      BottomMeterBar

  DSP/
    DspEngine
    PreampModule
    EqModule
    GateModule
    DeEsserModule
    CompressorModule
    SaturationModule
    HiEndModule
    OutputModule
```
