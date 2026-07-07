# Epic 2 - DSP Core

## Phạm vi
Chỉ sửa DSP.

## Không sửa
- CMakeLists.txt
- PluginProcessor
- PluginEditor
- PluginEntry
- GUI
- Parameter IDs

## Chuỗi xử lý
Input → Preamp → Gate → EQ → DeEsser → Compressor → Saturation → HiEnd → Output

## Module có xử lý thật
- Preamp analog drive cơ bản.
- Gate có hysteresis/hold.
- EQ Low/Mid/High/Air.
- De-Esser split band cơ bản.
- Compressor peak/RMS style cơ bản.
- Saturation tube/tape style.
- Hi-End warm/width/tame/glue.
- Output limiter cơ bản.
- Gain Reduction meter lấy từ Compressor.

## Mục tiêu
Build sạch trước.
Sau đó nghe thử từng module.
