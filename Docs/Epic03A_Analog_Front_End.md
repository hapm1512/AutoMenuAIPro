# Epic 3A Analog Front-End

Status: Implemented in source.

## Files changed

- `Source/DSP/PreampModule.h`
- `Source/DSP/PreampModule.cpp`
- `Source/Parameters/ParameterLayout.cpp`
- `Source/GUI/ChannelStripEditor.cpp`
- `CMakeLists.txt`

## DSP chain

Input → DC Block → Input Trim → Headroom → Transformer → Memory Effect → Tube → Even/Odd Harmonics → Soft Compression → Soft Clip → Anti Alias → Stereo Variance → Crosstalk → Noise Floor → Auto Gain → Peak Protect → Output

## Added parameters

- `preampInputTrim`
- `preampHeadroom`
- `preampTransformer`
- `preampTube`
- `preampBias`
- `preampMemory`
- `preampStereo`
- `preampCrosstalk`
- `preampNoise`
- `preampAutoGain`
- `preampPeakProtect`
- `preampMix`

Existing parameters preserved:

- `preampOn`
- `preampDrive`
- `preampHarmonic`
- `preampOutput`

## Notes

- Epic 3A is integrated inside `PreampModule` to preserve the current architecture.
- CMake now includes `Source/PluginEntry.cpp` to fix the missing `createPluginFilter()` linker error shown in `buildlog.txt`.
- The implementation avoids external dependencies beyond JUCE 8.
