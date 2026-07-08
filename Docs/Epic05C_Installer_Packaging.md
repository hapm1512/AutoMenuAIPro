# Epic 5C — Installer & Packaging

## Goal

Prepare Vocal Suite Ultra Pro for commercial Windows distribution without changing DSP tone.

## Added

- Release package folder structure.
- Local VST3 installer script.
- Local VST3 uninstaller script.
- Package builder script.
- Installer README.
- Distribution checklist.
- SHA256 manifest generation.

## Package Structure

```text
VocalSuiteUltraPro_Package/
├── Plugin/
│   └── VocalSuiteUltraPro.vst3
├── Docs/
│   ├── UserManual.md
│   ├── QuickStart.md
│   ├── PresetGuide.md
│   ├── HostSetup.md
│   └── Troubleshooting.md
├── Presets/
│   └── FactoryPresetManifest.xml
├── Installer/
│   ├── Install_VST3_User.ps1
│   ├── Install_VST3_System.ps1
│   └── Uninstall_VST3.ps1
├── CHANGELOG.md
├── README.md
└── SHA256SUMS.txt
```

## Install Targets

User install:

```text
%LOCALAPPDATA%\Programs\Common\VST3\Vocal Suite Ultra Pro.vst3
```

System install:

```text
C:\Program Files\Common Files\VST3\Vocal Suite Ultra Pro.vst3
```

## Notes

- User install does not require admin rights.
- System install requires admin rights.
- Presets are copied to the user document folder.
- This epic adds packaging workflow only.
- DSP and parameter sound are unchanged.
