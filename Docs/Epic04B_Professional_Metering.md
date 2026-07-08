# Epic 4B - Professional Metering

Status: Source integrated. Build pending user validation.

## Scope

Epic 4B adds a professional metering layer without changing the Epic 3 DSP chain.

## Added

- ProfessionalMetering utility
- Input peak / RMS
- Output peak / RMS
- Estimated true peak
- Gain reduction meter feed
- LUFS momentary approximation
- LUFS short-term smoothing
- LUFS integrated approximation
- Stereo correlation estimator
- GUI label updates
- Correlation bar rendering
- RMS overlay support in LevelMeter

## Files changed

- Source/DSP/ProfessionalMetering.h
- Source/DSP/Engine/DspEngine.h
- Source/DSP/Engine/DspEngine.cpp
- Source/PluginProcessor.h
- Source/PluginProcessor.cpp
- Source/GUI/ChannelStripEditor.cpp
- Source/GUI/Components/BottomMeterBar.h
- Source/GUI/Components/BottomMeterBar.cpp
- Source/GUI/Components/LevelMeter.h
- Source/GUI/Components/LevelMeter.cpp
- CMakeLists.txt

## Notes

This implementation is lightweight and realtime-safe for the audio thread. LUFS values are display-grade approximations intended for monitoring, not a certified offline loudness analyzer.
