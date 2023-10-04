// MIT License
//
// Copyright(c) 2023 Fred Emmott
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright noticeand this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// clang-format off
#include <Windows.h>
#include <TraceLoggingProvider.h>
// clang-format on

#pragma comment(lib, "Advapi32.lib")

#include <OXRTracing/forward_declarations.hpp>
#include <string>

namespace OXRTracing {

/* PS >
 * [System.Diagnostics.Tracing.EventSource]::new("FredEmmott.OpenXRTracing").guid
 * 0f924f5f-21f0-513d-a2ab-2ede802e0b8f
 */
TRACELOGGING_DEFINE_PROVIDER(gTraceProvider, "FredEmmott.OpenXRTracing",
    (0x0f924f5f, 0x21f0, 0x513d, 0xa2, 0xab, 0x2e, 0xde, 0x80, 0x2e, 0x0b,
        0x8f));

const std::string gLayerName = "XR_APILAYER_FREDEMMOTT_OXRTracing";

} // namespace OXRTracing
