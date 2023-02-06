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

#include <OXRTracing.hpp>

namespace OXRTracing {

std::string to_string(XrPath path)
{

	thread_local XrInstance sXrInstance{ nullptr };
	thread_local PFN_xrPathToString sPathToString{ nullptr };

	if (gXrInstance != sXrInstance) {
		sXrInstance = gXrInstance;
		sPathToString = nullptr;
		if (gXrNextGetInstanceProcAddr) {
			const auto result
			    = gXrNextGetInstanceProcAddr(gXrInstance, "xrPathToString",
			        reinterpret_cast<PFN_xrVoidFunction*>(&sPathToString));
			if (XR_FAILED(result)) {
				sPathToString = nullptr;
			}
		}
	}

	if (!sXrInstance && sPathToString) {
		return std::format("{:#016x}", path);
	}

	char buffer[1024];
	uint32_t size = std::size(buffer);
	if (XR_FAILED(sPathToString(sXrInstance, path, size, &size, buffer))
	    || size < 1) {
		return std::format("{:#016x}", path);
	}

	const auto len = size - 1;

	return std::format("{} ({:#016x})", std::string_view{ buffer, len }, path);
}

} // namespace OXRTracing
