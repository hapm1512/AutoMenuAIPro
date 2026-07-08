# Epic 03G - Output Stage

## Status
Implemented in source.

## Scope
- Output Trim
- Soft Clip Output
- True Peak style limiter
- ISP protection margin
- Brickwall ceiling
- LUFS compensation gain
- DC protection
- Dither control
- RMS meter tap
- Stereo correlation tap
- Gain reduction tracking
- Parameter smoothing
- Dry/Wet output mix

## Processing Pipeline
Input -> Output Trim -> DC Protection -> Soft Clip -> True Peak Limiter -> ISP Margin -> Ceiling Clamp -> Dither -> Meter Tap -> Output

## Updated Files
- Source/DSP/OutputModule.h
- Source/DSP/OutputModule.cpp
- Source/Parameters/ParameterLayout.cpp
- Source/GUI/ChannelStripEditor.cpp

## Notes
The limiter uses fast attack, smoothed release, ISP safety margin and final hard ceiling protection. The module is intentionally zero-dependency and JUCE 8 compatible.
