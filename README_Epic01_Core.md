# Epic 01 - Audio + Tone Core

This patch adds the first backend foundation for AutoMenuAIPro.

## Added modules

- Audio/AudioEngine
- Audio/AudioRingBuffer
- Analysis/FFTProcessor
- Analysis/PitchDetector
- Analysis/KeyDetector
- Analysis/BpmDetector
- Analysis/AnalysisEngine
- Core/AnalysisTypes
- Core/CoreController
- UI/Bridge/ToneAnalysisBridge

## Current state

- GUI layout is unchanged.
- Backend is added but not forced into the GUI yet.
- Audio input uses JUCE AudioDeviceManager.
- Analysis runs from a timer and reads recent audio from a ring buffer.
- Key detection provides key, mode, confidence, Camelot and basic BPM.

## Build

Run:

```powershell
cmake -S . -B Build
cmake --build Build --config Release
```

## Suggested commit

```powershell
git add .
git commit -m "Epic 01 - Add audio and tone analysis core"
git push
```
