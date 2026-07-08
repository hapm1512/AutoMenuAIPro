# Epic 4A - DSP Optimization

Status: Source updated, pending user build.

## Scope
- Denormal protection.
- NaN/Inf sample guard.
- Multi-channel peak metering.
- RMS metering hook.
- Zero allocation in processBlock path.
- Early return for empty buffers.
- Safer output after nonlinear stages.
- Audio-thread friendly helper utilities.

## Files changed
- Source/DSP/DspUtilities.h
- Source/DSP/Engine/DspEngine.h
- Source/DSP/Engine/DspEngine.cpp

## Notes
- No DSP tone module was added.
- Existing Epic 3 chain is preserved.
- Optimization is intentionally conservative.
- Next step: build in Visual Studio/CMake.
