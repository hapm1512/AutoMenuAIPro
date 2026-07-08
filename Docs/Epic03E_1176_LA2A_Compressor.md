# Epic 3E - 1176 + LA-2A Compressor

Status: source integrated.

## Scope
- Replace basic compressor core.
- Add serial 1176 FET stage.
- Add serial LA-2A opto stage.
- Add program-dependent opto memory.
- Add soft-knee gain reduction.
- Add sidechain HPF.
- Add parallel compression mix.
- Add analog harmonic color.
- Add auto gain compensation.
- Add parameter smoothing.
- Preserve existing DSP chain.

## Processing Order
Input
-> Input Trim
-> 1176 FET detector
-> 1176 fast gain reduction
-> 1176 analog color
-> LA-2A opto detector
-> LA-2A memory release
-> LA-2A gain reduction
-> Auto gain
-> Parallel mix
-> Output trim

## Main Parameters
- compInput
- compOutput
- compAttack
- compRelease
- comp1176Ratio
- comp1176Amount
- compLA2APeak
- compLA2AGain
- compLA2ASpeed
- compParallel
- compSideHp
- compAnalog
- compAutoGain
