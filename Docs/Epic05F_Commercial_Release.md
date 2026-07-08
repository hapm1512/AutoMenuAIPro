# Epic 5F — Commercial Release v1.0

Status: Source prepared. Build verification must be performed in Visual Studio/CMake on the target machine.

## Goal

Prepare Vocal Suite Ultra Pro for commercial v1.0 packaging and distribution.

## Included

- Final release metadata.
- Commercial release checklist.
- Final package script.
- Release verification script.
- Store page copy draft.
- EULA draft.
- Support policy draft.
- Version updated to 5.5.0.

## Release Identity

- Product: Vocal Suite Ultra Pro
- Author / Company: Hai Pham Audio
- Public release: v1.0 Commercial Release
- Internal version: 5.5.0
- Tag: v1.0-commercial-release

## No DSP Tone Changes

Epic 5F does not alter the DSP signal path.

## Final User-Facing Feature Set

- Commercial vocal DSP chain.
- Factory presets.
- User presets.
- Professional metering.
- Commercial GUI.
- Installer / packaging scripts.
- Trial / license layer.
- Documentation.
- Beta test assets.

## Final Validation Required

Before publishing, validate:

- Clean CMake configure.
- Release build.
- VST3 copied to install folder.
- Plugin scan in Cubase and Reaper.
- Preset load/save.
- Trial/license status display.
- Factory presets visible.
- No audio dropouts.
- No crash during bypass, automation, or sample-rate switching.
