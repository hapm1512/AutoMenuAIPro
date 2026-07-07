Remove-Item -Recurse -Force .\build -ErrorAction SilentlyContinue
cmake -B build -S . -DJUCE_DIR="D:\JUCE8"
cmake --build build --config Release --parallel
