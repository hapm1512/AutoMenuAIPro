# Epic 3D - Commercial De-Esser

Status: Source integrated.

## Goal
Replace the simple high-band attenuation de-esser with a commercial vocal de-esser suitable for recorded and live vocals.

## Implemented DSP

- Split-band de-essing
- Wide-band de-essing blend
- Dynamic sidechain focus
- Adaptive sibilance threshold
- Peak detector
- RMS detector
- Peak/RMS blend
- LookAhead delay path
- Attack / Hold / Release envelope
- Soft knee reduction curve
- Ratio control
- Range control
- Frequency focus
- Adaptive noise floor tracking
- Stereo link
- Auto makeup
- Output trim
- Parameter smoothing style envelope
- Safe peak limiting guard

## Signal Flow

Input
-> Sidechain HPF/LPF focus
-> Peak + RMS detector
-> Adaptive threshold
-> Soft knee ratio curve
-> Split-band reduction
-> Wide-band blend
-> Stereo link
-> Auto makeup
-> Output trim

## Main Parameters

- deessFreq
- deessThreshold
- deessRange
- deessRatio
- deessAttack
- deessHold
- deessRelease
- deessLookahead
- deessKnee
- deessFocus
- deessPeakMix
- deessAdaptive
- deessSplit
- deessStereoLink
- deessAutoMakeup
- deessOutput

## Edited Files

- Source/DSP/DeEsserModule.h
- Source/DSP/DeEsserModule.cpp
- Source/Parameters/ParameterLayout.cpp
- Source/GUI/ChannelStripEditor.cpp

## Build Note

Designed for JUCE 8, CMake, Visual Studio, C++20.
