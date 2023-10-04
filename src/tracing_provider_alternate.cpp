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
 * [System.Diagnostics.Tracing.EventSource]::new("FredEmmott.OpenXRTracing.Alternate").guid
 * 002f5359-fd69-56b8-a91f-26443c30187d
 */
TRACELOGGING_DEFINE_PROVIDER(gTraceProvider,
    "FredEmmott.OpenXRTracing.Alternate",
    (0x002f5359, 0xfd69, 0x56b8, 0xa9, 0x1f, 0x26, 0x44, 0x3c, 0x30, 0x18,
        0x7d));

const std::string gLayerName = "XR_APILAYER_FREDEMMOTT_OXRTracing_Alternate";

} // namespace OXRTracing
