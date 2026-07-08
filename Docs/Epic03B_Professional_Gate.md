# Epic 3B Professional Gate

Status: source integrated.

## Scope

- Adaptive noise floor
- Auto threshold blend
- RMS detector
- Peak detector
- Attack / Hold / Release
- Hysteresis
- LookAhead delay
- Downward expander
- Soft knee
- Range and depth control
- Sidechain HPF
- Sidechain LPF
- Breath preserve
- Stereo link
- Parameter smoothing
- Auto makeup

## DSP Pipeline

Input
-> Sidechain HPF/LPF
-> RMS + Peak Detector
-> Adaptive Threshold
-> Hysteresis / Hold
-> LookAhead Delay
-> Soft Knee Downward Expander
-> Breath Preserve
-> Stereo Link
-> Auto Makeup
-> Output

## Files changed

- Source/DSP/GateModule.h
- Source/DSP/GateModule.cpp
- Source/Parameters/ParameterLayout.cpp
- Source/GUI/ChannelStripEditor.cpp
- Docs/Epic03B_Professional_Gate.md
