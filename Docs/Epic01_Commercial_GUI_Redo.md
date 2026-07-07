# Epic 1 - Commercial GUI Redo

## Mục tiêu
Làm lại Epic 1 trên nền Sprint 10 stable.

## Nguyên tắc
- Không sửa CMakeLists.txt.
- Không sửa PluginProcessor.
- Không sửa PluginEditor.
- Không sửa PluginEntry.
- Không sửa DSP.
- Chỉ sửa GUI hiện có.
- Ưu tiên build sạch.

## Đã xử lý
- Sửa đồng bộ LevelMeter.h và LevelMeter.cpp.
- Tránh lỗi clipHold chưa khai báo.
- Nâng Knob Pro nhưng giữ nguyên API cũ.
- Giữ nguyên cấu trúc CMake ổn định.
- Giữ nguyên parameter ID.

## Ghi chú
Bản này ưu tiên build ổn định trước.
Sau khi build thành công, mới tiếp tục DSP.
