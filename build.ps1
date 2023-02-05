param(
  [switch] $SkipCodegen = $false
)

$ErrorActionPreference = "Stop"

if (-not $SkipCodegen) {
  python.exe tracing-generator.py
  if ($LastExitCode -ne 0) {
    return 1
  }
}

foreach ($generated in Get-ChildItem -Path gen -Recurse -Include *.cpp, *.hpp) {
  clang-format -i $generated.FullName
}

$cwd = Get-Location
New-Item -Path out -ItemType Directory -Force | Out-Null
try {
  Set-Location out
  cl.exe `
    /std:c++20 `
    /D WIN32_LEAN_AND_MEAN=1 `
    /D NOMINMAX=1 `
    /I "$cwd/OpenXR-SDK-Source/build/include/openxr" `
    /I "$cwd/gen/include" `
    /I "$cwd/include" `
    /Fe:XR_APILAYER_FREDEMMOTT_OXRTracing.dll `
    /LD `
    "$cwd/gen/src/layer.gen.cpp" `
    "$cwd/src/layer.cpp"
}
finally {
  Set-Location $cwd
}
