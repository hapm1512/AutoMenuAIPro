# Vocal Suite Ultra Pro - Validation Test Matrix

## Build Matrix

| Test | Debug | Release | Pass |
|---|---:|---:|---:|
| CMake Configure | [ ] | [ ] | [ ] |
| VST3 Build | [ ] | [ ] | [ ] |
| Standalone Build | [ ] | [ ] | [ ] |
| Plugin Scan | [ ] | [ ] | [ ] |

## Audio Matrix

| Sample Rate | 32 | 64 | 128 | 256 | 512 | 1024 | 2048 |
|---|---|---|---|---|---|---|---|
| 44.1 kHz | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |
| 48 kHz | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |
| 88.2 kHz | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |
| 96 kHz | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |
| 176.4 kHz | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |
| 192 kHz | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |

## DAW Matrix

| Host | Scan | Insert | Preset | Automation | Bypass | Render | Pass |
|---|---|---|---|---|---|---|---|
| Cubase | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |
| Reaper | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |
| Studio One | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |
| Ableton Live | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |
| FL Studio | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] | [ ] |

## Functional Matrix

| Area | Test | Pass |
|---|---|---:|
| Input | Peak meter responds | [ ] |
| Gate | No chatter on vocal tail | [ ] |
| EQ | Automation is smooth | [ ] |
| De-Esser | Sibilance reduction stable | [ ] |
| Compressor | Gain reduction meter stable | [ ] |
| Saturation | No clipping burst | [ ] |
| Output | Ceiling respected | [ ] |
| Preset | Save / load retains state | [ ] |
| GUI | No repaint lag | [ ] |
