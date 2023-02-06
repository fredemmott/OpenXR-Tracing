# OpenXR-Tracing

Windows [Trace Logging] ([ETW]) as an OpenXR API layer.

## Dependencies

Python: installing from the MS store or `winget` should be fine.

## Building

1. Fetch submodules: `git submodule update --init`
2. Open an x64 developer powershell (or an x64 command prompt, then run `powershell`)
3. Run `./build.ps1`

`build.ps1` supports two flags:
- `-Clang`: use `clang-cl.exe` instead of `cl.exe`
- `-SkipCodegen`: don't create or update the `gen/` directory

## Usage

Two API layers are generated:
- `XR_APILAYER_FREDEMMOTT_OXRTracing`
- `XR_APILAYER_FREDEMMOTT_OXRTracing_Alternate`

These are identical, except for the name, which is included in the trace data; this allows tracing both sides of another OpenXR API layer.

Each of these layers has its' own DLL and json file in `out\` after building; add the `.json` file to the registry like any other OpenXR API layer. You may want to disable the layer when you're not actively using this layer - it has not been optimized for performance.

To start collecting data, as administrator:

    wpr -start OXRTracing.wprp -filemode

To finish collecting data and write an ETL file:

    wpr -stop MyTraceFile.etl

If you're looking for an event log rather than profiling information, I recommend [Tabnalysis].

[Trace Logging]: https://learn.microsoft.com/en-us/windows/win32/tracelogging/trace-logging-portal
[ETW]: https://learn.microsoft.com/en-us/windows/win32/etw/about-event-tracing
[Tabnalysis]: https://apps.microsoft.com/store/detail/tabnalysis/9NQLK2M4RP4J
