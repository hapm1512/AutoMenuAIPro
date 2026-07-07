# Sprint 02 GUI

## Phạm vi
- Chỉ nâng cấp giao diện.
- Không thay đổi DSP.
- Không thay đổi audio path.

## Đã cập nhật
- Theme màu mới.
- Channel Strip layout tinh chỉnh.
- Header/Preset bar polish.
- Module panel mới.
- LED ON/OFF vẽ trực tiếp.
- Knob đẹp hơn.
- Meter đẹp hơn.
- Footer/status bar.

## Build
```powershell
cd D:\ProjectsVST3\VocalSuiteUltraPro
Remove-Item -Recurse -Force .\build -ErrorAction SilentlyContinue
cmake -B build -S . -DJUCE_DIR="D:\JUCE8"
cmake --build build --config Release
```
