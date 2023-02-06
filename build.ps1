param(
  [switch] $SkipCodegen = $false,
  [switch] $Clang = $false
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
Copy-Item -Force .\APILayer.json out\APILayer.json
try {
  Set-Location out
  $compiler = 'cl.exe'
  if ($Clang) {
    $compiler = 'clang-cl.exe'
  }
  & $compiler `
    /std:c++20 `
    /Zc:__cplusplus `
    /permissive- `
    /D WIN32_LEAN_AND_MEAN=1 `
    /D NOMINMAX=1 `
    /I "$cwd/OpenXR-SDK-Source/build/include/openxr" `
    /I "$cwd/OpenXR-SDK-Source/build/include" `
    /I "$cwd/OpenXR-SDK-Source/src/common" `
    /I "$cwd/gen/include" `
    /I "$cwd/include" `
    /Fe:XR_APILAYER_FREDEMMOTT_OXRTracing.dll `
    /LD `
    "$cwd/gen/src/layer.gen.cpp" `
    "$cwd/src/layer.cpp" `
    "$cwd/src/tracing.cpp"
}
finally {
  Set-Location $cwd
}
