# Milestone C - Live Reliability Package

Copy the included files over the project root, then build:

```powershell
cmake -S . -B Build
cmake --build Build --config Release
```

Included changes:

- WorkspaceState save/load to user AppData.
- Startup restore.
- Audio and MIDI auto reconnect.
- Performance/setup mode state.
- Dynamic Footer status.
- AppLogger file logging.
- AutoMenuCore live-session lifecycle.
