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
#include <openxr.h>

#include <format>
#include <string.h>

///////////////////////////
///// Primitive types /////
///////////////////////////

#define OXRTL_ARGS_float(x, name) TraceLoggingValue(x, name)
#define OXRTL_ARGS_int8_t(x, name) TraceLoggingValue(x, name)
#define OXRTL_ARGS_uint8_t(x, name) TraceLoggingValue(x, name)
#define OXRTL_ARGS_int16_t(x, name) TraceLoggingValue(x, name)
#define OXRTL_ARGS_uint16_t(x, name) TraceLoggingValue(x, name)
#define OXRTL_ARGS_int32_t(x, name) TraceLoggingValue(x, name)
#define OXRTL_ARGS_uint32_t(x, name) TraceLoggingValue(x, name)
#define OXRTL_ARGS_int64_t(x, name) TraceLoggingValue(x, name)
#define OXRTL_ARGS_uint64_t(x, name) TraceLoggingValue(x, name)
#define OXRTL_ARGS_size_t(x, name) TraceLoggingValue(x, name)
#define OXRTL_ARGS_uintptr_t(x, name) TraceLoggingValue(x, name)

#define OXRTL_ARGS_char_P(x, name) TraceLoggingValue(x, name)

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
#define OXRTL_ARGS_HANDLE(x, name) OXRTL_ARGS_POINTER(x, name)
#else
#define OXRTL_ARGS_HANDLE(x, name) \
	TraceLoggingValue( \
	    std::format("{:#016x}", static_cast<const uint64_t>(x)).c_str(), name)
#endif

#define OXRTL_ARGS_void_P OXRTL_ARGS_POINTER

/////////////////////////////////////
///// Base types: special cases /////
/////////////////////////////////////

#define OXRTL_ARGS_XrVersion(x, name) \
	TraceLoggingValue(std::format("{} (v {}.{}.{})", x, XR_VERSION_MAJOR(x), \
	                      XR_VERSION_MINOR(x), XR_VERSION_PATCH(x)) \
	                      .c_str(), \
	    name)

#define OXRTL_ARGS_char_FA(x, name, maxLen) \
	TraceLoggingCountedString(x, strnlen_s(x, maxLen), name)

#define OXRTL_ARGS_char_P_DA(x, name, count) \
	TraceLoggingValue(::OXRTracing::to_string(x, count).c_str(), name)

#define OXRTL_ARGS_XrPath(oxrtlIt, name) \
	TraceLoggingValue(::OXRTracing::to_string(oxrtlIt).c_str(), name)

// TODO: this is XrEventDataBuffer.varying
// Ideally we should trace the real data for a subtype of XrEventDataBuffer
#define OXRTL_ARGS_uint8_t_FA(x, name, count) \
	TraceLoggingValue("binary data", name)

//////////////////////////////////
///// Structs: special cases /////
//////////////////////////////////

// This doesn't have 'child' structs, but we still want to log the type
#define OXRTL_ARGS_XrEventDataBuffer(oxrtlIt, name) \
	TraceLoggingStruct(1, name), \
	    OXRTL_ARGS_XrStructureType(oxrtlIt.type, "type")
