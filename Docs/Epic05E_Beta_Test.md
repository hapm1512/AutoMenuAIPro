# Epic 5E - Beta Test

Status: Source prepared for external beta validation.

## Goal

Validate Vocal Suite Ultra Pro with real users before commercial release.

## Scope

- Beta build distribution
- Tester onboarding
- Feedback form
- Crash report collection
- Host compatibility tracking
- Preset compatibility tracking
- License/trial validation
- Installer validation
- Performance validation
- Release blocker tracking

## Beta Gates

A beta build passes when:

- VST3 loads in all target DAWs.
- Standalone opens without crash.
- Factory presets load correctly.
- License/trial state persists.
- Installer and uninstaller work.
- No audio-thread allocation warnings are observed.
- No NaN/Inf output is observed.
- CPU remains stable across buffer sizes.

## Target DAWs

- Cubase
- Reaper
- Studio One
- Ableton Live
- FL Studio

## Target Sample Rates

- 44.1 kHz
- 48 kHz
- 88.2 kHz
- 96 kHz
- 192 kHz

## Target Buffers

- 32
- 64
- 128
- 256
- 512
- 1024
- 2048

## Beta Result

Epic 5E does not change DSP tone.
It adds beta testing infrastructure and release readiness tracking.
