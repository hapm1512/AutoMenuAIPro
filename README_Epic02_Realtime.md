# Epic 02 - Realtime Audio Capture + GUI Binding

This update connects the audio capture core to the compact GUI.

## Included
- Auto starts JUCE AudioEngine on Dashboard creation.
- AnalysisManager receives live input blocks.
- ToneDetectorComponent reads live AnalysisResult.
- Key, scale, confidence, BPM, Camelot and meter graphics update from analysis data.
- Settings panel behavior is preserved.

## Build
```powershell
cmake -S . -B Build
cmake --build Build --config Release
```

## Notes
- Uses the default system input device for now.
- Backend only; GUI layout remains locked.
- Device selection will be added later in Settings.
