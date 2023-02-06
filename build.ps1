param(
  [switch] $SkipCodegen = $false,
  [switch] $Clang = $false
)

$ErrorActionPreference = "Stop"

if (-not $SkipCodegen) {
  python.exe tracing-generator.py
  if ($LastExitCode -ne 0) {
    return $lastExitCode
  }
}

foreach ($generated in Get-ChildItem -Path gen -Recurse -Include *.cpp, *.hpp) {
  clang-format -i $generated.FullName
}

$cwd = Get-Location
New-Item -Path out -ItemType Directory -Force | Out-Null
Copy-Item -Force .\APILayer.json out\APILayer.json
(Get-Content APILayer.json) `
  -replace "XR_APILAYER_FREDEMMOTT_OXRTracing", "XR_APILAYER_FREDEMMOTT_OXRTracing_Alternate" `
| Set-Content out\APILayer_Alternate.json
try {
  Set-Location out
  $compiler = 'cl.exe'
  if ($Clang) {
    $compiler = 'clang-cl.exe'
  }
  $baseArgs = @(
    '/std:c++20'
    '/Zc:__cplusplus'
    '/permissive-'
    '/D', 'WIN32_LEAN_AND_MEAN=1',
    '/D', 'NOMINMAX=1',
    '/I', "$cwd/OpenXR-SDK-Source/build/include/openxr",
    '/I', "$cwd/OpenXR-SDK-Source/build/include",
    '/I', "$cwd/OpenXR-SDK-Source/src/common",
    '/I', "$cwd/gen/include",
    '/I', "$cwd/include"
  )
  $sources = @(
    "$cwd/gen/src/layer.gen.cpp",
    "$cwd/src/layer.cpp",
    "$cwd/src/tracing.cpp"
  )

  $objs = $sources | % { $_ -replace '^.+/([^/]+).cpp', '$1.obj' }
  & $compiler @baseArgs -c $sources
  if ($LastExitCode -ne 0) {
    return $LastExitCode
  }
  & $compiler @baseArgs $objs `
    "$cwd/src/tracing_provider.cpp" `
    "/Fe:XR_APILAYER_FREDEMMOTT_OXRTracing.dll" `
    "/LD"
  if ($LastExitCode -ne 0) {
    return $LastExitCode
  }
  & $compiler @baseArgs $objs `
    "$cwd/src/tracing_provider_alternate.cpp" `
    "/Fe:XR_APILAYER_FREDEMMOTT_OXRTracing_Alternate.dll" `
    "/LD"
  if ($LastExitCode -ne 0) {
    return $LastExitCode
  }
}
finally {
  Set-Location $cwd
}
