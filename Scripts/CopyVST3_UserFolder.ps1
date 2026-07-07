# Copy thủ công vào VST3 user folder
$src = ".\build\VocalSuiteUltraPro_artefacts\Release\VST3\Vocal Suite Ultra Pro.vst3"
$dst = "$env:LOCALAPPDATA\Programs\Common\VST3"

New-Item -ItemType Directory -Force -Path $dst | Out-Null
Copy-Item -Recurse -Force $src $dst
