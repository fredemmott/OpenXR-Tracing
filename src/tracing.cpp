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
#include <unordered_map>

namespace OXRTracing {

std::string to_string(XrPath path)
{
	if (!path) {
		return "[null]";
	}
	thread_local XrInstance sXrInstance{};
	thread_local std::unordered_map<XrPath, std::string> sCache;

	if (gXrInstance != sXrInstance) {
		sCache.clear();
		sXrInstance = gXrInstance;
	}

	if (sCache.contains(path)) {
		return sCache.at(path);
	}

	if (!(sXrInstance && next_xrPathToString)) {
		return std::format("{:#016x}", path);
	}

	char buffer[1024];
	uint32_t size = std::size(buffer);
	if (XR_FAILED(next_xrPathToString(sXrInstance, path, size, &size, buffer))
	    || size < 1) {
		return std::format("{:#016x}", path);
	}

	const auto len = size - 1;

	const auto ret
	    = std::format("{:#016x} ({})", path, std::string_view{ buffer, len });
	sCache[path] = ret;
	return ret;
}

std::string to_string(const ConstCStr* const arr, size_t count)
{
	if (count == 0) {
		return "[empty]";
	}
	std::string out;
	for (size_t i = 0; i < count; ++i) {
		if (!out.empty()) {
			out += ",";
		}
		out += arr[i];
	}
	return out;
}

} // namespace OXRTracing
