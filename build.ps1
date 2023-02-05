python.exe tracing-generator.py
foreach ($generated in Get-ChildItem -Path gen -Recurse -Include *.cpp, *.hpp) {
  clang-format -i $generated.FullName
}

cl.exe `
  /std:c++20 `
  /D WIN32_LEAN_AND_MEAN=1 `
  /D NOMINMAX=1 `
  /I OpenXR-SDK-Source/build/include/openxr `
  /I gen/include `
  /I include `
  gen/src/layer.gen.cpp
