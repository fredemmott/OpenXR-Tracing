// MIT License
//
// Copyright(c) 2021-2022 Matthieu Bucchianeri
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

#pragma once

#include <OXRTracing/preamble.hpp>

#include <string>

namespace OXRTracing {
TRACELOGGING_DECLARE_PROVIDER(gTraceProvider);
extern thread_local XrInstance gXrInstance;
extern PFN_xrGetInstanceProcAddr gXrNextGetInstanceProcAddr;
extern const std::string gLayerName;

/* These are <type>_to_string because in 32-bit builds,
 * OpenXR handles are just type aliases to uint64_t, so
 * we can't use standard overload resolution. */
std::string XrAction_to_string(XrAction action);
std::string XrActionSet_to_string(XrActionSet actionSet);
std::string XrPath_to_string(XrPath path);
std::string XrSpace_to_string(XrSpace space);

using ConstCStr = const char*;
std::string to_string(const ConstCStr* const arr, size_t count);
std::string to_string(ConstCStr str, size_t count);

void xrCreateActionSet_hook(XrResult result, XrInstance instance,
    const XrActionSetCreateInfo* createInfo, XrActionSet* actionSet) noexcept;
void xrCreateAction_hook(XrResult result, XrActionSet actionSet,
    const XrActionCreateInfo* createInfo, XrAction* action);
void xrStringToPath_hook(
    XrResult result, XrInstance instance, const char* pathString, XrPath* path);
void xrCreateActionSpace_hook(XrResult result, XrSession session,
    const XrActionSpaceCreateInfo* createInfo, XrSpace* space);
void xrCreateReferenceSpace_hook(XrResult result, XrSession session,
    const XrReferenceSpaceCreateInfo* createInfo, XrSpace* space);

} // namespace OXRTracing

XrResult OXRTracing_xrEnumerateInstanceExtensionProperties(
    const char* layerName, uint32_t propertyCapacityInput,
    uint32_t* propertyCountOutput, XrExtensionProperties* properties);

XrResult OXRTracing_xrEnumerateApiLayerProperties(
    uint32_t propertyCapacityInput, uint32_t* propertyCountOutput,
    XrApiLayerProperties* properties);