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

#include <OXRTracing/forward_declarations.hpp>
#include <openxr/openxr.h>

#include <format>
#include <string.h>

//////////////////////
///// Base types /////
//////////////////////

#define OXRTL_ARGS_ATOM(x, name) \
	TraceLoggingValue(std::format("{:#016x}", x).c_str(), name)
#define OXRTL_ARGS_POINTER(x, name) \
	TraceLoggingValue( \
	    std::format("{:#016x}", reinterpret_cast<const uint64_t>(x)).c_str(), \
	    name)
#if (XR_PTR_SIZE == 8)
#define OXRTL_HANDLE_CAST reinterpret_cast
#else
#define OXRTL_HANDLE_CAST static_cast
#endif
#define OXRTL_ARGS_HANDLE(x, name) \
	TraceLoggingValue( \
	    std::format("{:#016x}", OXRTL_HANDLE_CAST<const uint64_t>(x)).c_str(), \
	    name)

#define OXRTL_ARGS_void_P OXRTL_ARGS_POINTER

#define OXRTL_ARGS_IUnknown(x, name) OXRTL_ARGS_POINTER((&x), name)
#define OXRTL_ARGS_IUnknown_P(x, name) OXRTL_ARGS_POINTER(x, name)

/////////////////////////////////////
///// Base types: special cases /////
/////////////////////////////////////

#define OXRTL_ARGS_XrVersion(x, name) \
	TraceLoggingValue(std::format("{} (v {}.{}.{})", x, XR_VERSION_MAJOR(x), \
	                      XR_VERSION_MINOR(x), XR_VERSION_PATCH(x)) \
	                      .c_str(), \
	    name)

#define OXRTL_ARGS_char_P(x, name) TraceLoggingValue(x, name)

#define OXRTL_ARGS_char_FA(x, name, maxLen) \
	TraceLoggingCountedString(x, strnlen_s(x, maxLen), name)

#define OXRTL_ARGS_char_P_DA(x, name, count) \
	TraceLoggingValue(::OXRTracing::to_string(x, count).c_str(), name)

#define OXRTL_ARGS_XrPath(oxrtlIt, name) \
	TraceLoggingValue(::OXRTracing::to_string(oxrtlIt).c_str(), name)
#define OXRTL_ARGS_XrAction(oxrtlIt, name) \
	TraceLoggingValue(::OXRTracing::to_string(oxrtlIt).c_str(), name)
#define OXRTL_ARGS_XrActionSet(oxrtlIt, name) \
	TraceLoggingValue(::OXRTracing::to_string(oxrtlIt).c_str(), name)
#define OXRTL_ARGS_XrSpace(oxrtlIt, oxrtlName) \
	TraceLoggingValue((::OXRTracing::to_string(oxrtlIt).c_str()), oxrtlName)

#define OXRTL_ARGS_uint8_t_FA(x, name, count) \
	TraceLoggingValue("binary data", name)

//////////////////////////////////
///// Structs: special cases /////
//////////////////////////////////

// This doesn't have 'child' structs, but we still want to log the type
#define OXRTL_ARGS_XrEventDataBuffer(oxrtlIt, name) \
	TraceLoggingStruct(1, name), \
	    OXRTL_ARGS_XrStructureType(oxrtlIt.type, "type")
