# Milestone A - Tone Detection

Package này tập trung đúng mục tiêu live:

- Audio Input
- Noise Gate
- Pitch Detection
- Key Detection
- Tone Stability Filter
- Confidence
- RealtimeToneState
- GUI Binding

Không thêm YouTube, không thêm GUI, không mở rộng DSP viewer.

## Cách dùng

Copy toàn bộ file trong package đè vào project AutoMenuAIPro.

Build:

```powershell
cmake -S . -B Build
cmake --build Build --config Release
```

Nếu build thành công:

```powershell
git add .
git commit -m "Milestone A - Add stable live tone detection"
git push
git tag milestone-a-tone-detection-v1
git push origin milestone-a-tone-detection-v1
```
