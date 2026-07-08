# Epic 4E - Validation & QA

Status: Source delivered for build validation.

## Scope

Epic 4E adds a production validation layer without changing the audio DSP chain.
The goal is to verify stability, compatibility, automation safety, preset safety,
and runtime behavior before Release Candidate.

## Validation Areas

- CMake configure validation
- Debug build validation
- Release build validation
- VST3 target validation
- Standalone target validation
- Sample-rate matrix
- Buffer-size matrix
- Mono / stereo bus validation
- Parameter automation safety
- Preset save / load safety
- Bypass behavior
- Meter stability
- Denormal / NaN / Inf guard
- DAW compatibility checklist
- Release readiness checklist

## Test Matrix

### Sample Rates

- 44100 Hz
- 48000 Hz
- 88200 Hz
- 96000 Hz
- 176400 Hz
- 192000 Hz

### Buffer Sizes

- 32 samples
- 64 samples
- 128 samples
- 256 samples
- 512 samples
- 1024 samples
- 2048 samples

### Hosts

- Cubase
- Reaper
- Studio One
- Ableton Live
- FL Studio
- Standalone

## Pass Criteria

- No build error
- No plugin scan crash
- No DAW load crash
- No audio thread allocation spike during normal use
- No NaN / Inf output
- No denormal CPU spike
- No stuck meter
- No broken preset recall
- No automation crash
- No bypass pop beyond acceptable level
- No output above ceiling under normal limiter use

## Result

Epic 4E prepares the project for Release Candidate testing.
