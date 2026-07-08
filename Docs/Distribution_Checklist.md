# Distribution Checklist

## Build

- [ ] Clean configure CMake.
- [ ] Build Release VST3.
- [ ] Build Standalone if required.
- [ ] Run smoke test.
- [ ] Run validation matrix.

## Package

- [ ] Run `Scripts/Package_Commercial.ps1`.
- [ ] Run `Scripts/Verify_Package.ps1`.
- [ ] Confirm SHA256SUMS.txt exists.
- [ ] Confirm installer scripts exist.
- [ ] Confirm factory presets exist.
- [ ] Confirm user manual exists.

## DAW Validation

- [ ] Cubase.
- [ ] Reaper.
- [ ] Studio One.
- [ ] Ableton Live.
- [ ] FL Studio.

## Release

- [ ] Tag release.
- [ ] Upload ZIP package.
- [ ] Save build log.
- [ ] Archive source package.
