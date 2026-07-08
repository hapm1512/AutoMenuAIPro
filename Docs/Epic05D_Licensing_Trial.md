# Epic 5D - Licensing / Trial

## Status
Implemented source baseline for licensing and trial infrastructure.

## Added
- `Source/Core/LicenseManager.h`
- `Source/Core/LicenseManager.cpp`
- Offline machine-code activation layer
- 14-day trial state
- Activated / Trial / Expired / Invalid states
- Local license file persistence
- License status display in GUI status area
- License state included in processor state metadata

## Design
Epic 5D intentionally does not hard-lock DSP processing.
This keeps build safety and host stability during the release-candidate phase.
Commercial enforcement can be enabled later after beta validation.

## License file location
Windows user data folder:

```text
%APPDATA%/Hai Pham Audio/Vocal Suite Ultra Pro/license.xml
```

## Offline activation flow
1. Plugin shows machine code.
2. Customer sends machine code.
3. Vendor generates offline key.
4. Plugin validates name, email, machine code, and key.

## Author
Hai Pham Audio
