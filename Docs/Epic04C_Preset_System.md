# Epic 4C - Preset System

Status: Source integrated.

Implemented:
- Factory preset bank
- User preset save/load foundation
- A/B state slots
- Copy A to B
- Swap A/B
- Lightweight undo/redo stack
- Preset metadata in plugin state
- TopBar preset navigation hooks

Factory presets:
- Modern Vocal Pop
- Live Clean Vocal
- Warm Ballad Lead
- Bright Air Vocal

Notes:
- User presets are stored as `.vspreset` XML files.
- DSP chain from Epic 3 remains unchanged.
- Epic 4B metering remains active.
