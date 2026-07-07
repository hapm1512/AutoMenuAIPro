# Epic 03 - Tone Detection Engine

This package upgrades the realtime analysis layer without changing the locked GUI.

Changes:
- FFT size upgraded to 4096 samples.
- NoiseGate added for silence/noise rejection.
- DC removal added before analysis.
- Pitch detector upgraded to YIN-style CMND detection.
- ToneStabilizer added for anti-jitter key voting.
- BPM output smoothed before GUI display.
- AnalysisManager now outputs more stable ToneResult data.

Build:
```powershell
cmake -S . -B Build
cmake --build Build --config Release
```

Commit:
```powershell
git add .
git commit -m "Epic 03 - Improve realtime tone detection engine"
git push
git tag core-epic03-v1
git push origin core-epic03-v1
```
