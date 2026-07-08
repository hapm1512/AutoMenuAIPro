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


## Epic 4C Preset System

Added factory presets, user preset save/load foundation, A/B slots, undo/redo stack, and TopBar preset navigation.


## Epic 4D Commercial GUI

- HiDPI scale support
- Commercial theme manager
- Dark/Light theme toggle
- Keyboard command hub
- Buffered GUI painting hooks
- Smooth status and meter update

## Epic 4E - Validation & QA

This build adds validation documents, build validation scripts, host QA checklist,
and a lightweight runtime validation report header.

Main files:

- Docs/Epic04E_Validation_QA.md
- Docs/Validation_Test_Matrix.md
- Scripts/Validate_Epic4E.ps1
- Scripts/Host_QA_Checklist.md
- Source/Core/ValidationReport.h


## Epic 4F - Release Candidate

This build adds release candidate preparation assets.

Main files:

- Docs/Epic04F_Release_Candidate.md
- Docs/Release_Checklist.md
- Docs/CHANGELOG.md
- Scripts/Package_RC.ps1
- Scripts/SmokeTest_RC.ps1
- Source/Core/ReleaseInfo.h

No intentional DSP tone changes were made in this epic.

## Documentation

User documentation is available in `Docs/`:

- `UserManual.md`
- `QuickStart.md`
- `PresetGuide.md`
- `Troubleshooting.md`
- `HostSetup.md`


## Epic 5C — Installer & Packaging

Commercial packaging workflow is included.

Build Release first, then run:

```powershell
powershell -ExecutionPolicy Bypass -File .\Scripts\Package_Commercial.ps1
powershell -ExecutionPolicy Bypass -File .\Scripts\Verify_Package.ps1
```

Installer scripts are in `Installer/`.

Recommended user install:

```powershell
powershell -ExecutionPolicy Bypass -File .\Installer\Install_VST3_User.ps1
```


## Epic 5D Licensing / Trial

Licensing and trial infrastructure added for commercial release preparation.

## Epic 5E - Beta Test

This milestone adds beta testing infrastructure for pre-release validation.
It includes tester documentation, beta feedback templates, issue tracking, and smoke-test scripts.
DSP tone is unchanged from the previous milestone.


## Commercial Release

Current commercial release preparation: v1.0 / internal 5.5.0.

Company / author: Hai Pham Audio.

See:

- Docs/Epic05F_Commercial_Release.md
- Docs/Commercial_Release_Checklist.md
- Docs/UserManual.md
- Scripts/Package_Commercial_v1.ps1
- Scripts/Verify_Commercial_Release.ps1
