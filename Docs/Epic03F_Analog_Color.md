# Epic 3F - Analog Color

Status: Source implemented.

## Scope

Epic 3F upgrades the original Saturation module into a commercial analog color stage for vocal processing.

## Implemented DSP

- Transformer drive
- Tape saturation
- Tube saturation
- Even harmonic generator
- Odd harmonic generator
- Dynamic saturation
- Bias control
- Presence exciter
- Air exciter
- Low-end thickener
- Stereo width control
- Crosstalk control
- Auto gain compensation
- Dry/Wet mix
- Output trim
- Parameter smoothing
- Bypass crossfade
- Soft output protection

## Processing chain

Input -> Transformer -> Tape -> Tube -> Harmonics -> Presence -> Air -> Low Thickener -> Tone -> Auto Gain -> Width -> Crosstalk -> Output

## Files changed

- Source/DSP/SaturationModule.h
- Source/DSP/SaturationModule.cpp
- Source/Parameters/ParameterLayout.cpp
- Source/GUI/ChannelStripEditor.cpp
- Docs/Epic03F_Analog_Color.md

## Build note

No CMake file change required. Existing `SaturationModule.cpp` remains in target sources.
