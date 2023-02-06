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

namespace {
struct ActionInfo {
	std::string mName;
	XrActionSet mActionSet{};
};

struct Cache {
	XrInstance mXrInstance{};
	std::unordered_map<XrPath, std::string> mPaths;
	std::unordered_map<XrActionSet, std::string> mActionSets;
	std::unordered_map<XrAction, ActionInfo> mActions;
};
thread_local Cache sCache{};
} // namespace

namespace OXRTracing {

std::string to_string(XrPath path)
{
	if (!path) {
		return "[null]";
	}

	if (sCache.mXrInstance != gXrInstance) {
		sCache = { gXrInstance };
	}

	auto& sPathCache = sCache.mPaths;
	if (sPathCache.contains(path)) {
		return sPathCache.at(path);
	}

	if (!(gXrInstance && next_xrPathToString)) {
		return std::format("{:#016x}", path);
	}

	char buffer[1024];
	uint32_t size = std::size(buffer);
	if (XR_FAILED(next_xrPathToString(gXrInstance, path, size, &size, buffer))
	    || size < 1) {
		return std::format("{:#016x}", path);
	}

	const auto len = size - 1;

	const auto ret
	    = std::format("{} ({:#016x})", std::string_view{ buffer, len }, path);
	sPathCache[path] = ret;
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

std::string to_string(XrAction action)
{
	if (!sCache.mActions.contains(action)) {
		return std::format(
		    "{:#016x}", OXRTL_HANDLE_CAST<const uint64_t>(action));
	}

	const auto& data = sCache.mActions.at(action);
	return std::format("{} ({:#016x}) from {}", data.mName,
	    OXRTL_HANDLE_CAST<const uint64_t>(action), to_string(data.mActionSet));

	return {};
}

std::string to_string(XrActionSet actionSet)
{
	if (!sCache.mActionSets.contains(actionSet)) {
		return std::format(
		    "{:#016x}", OXRTL_HANDLE_CAST<const uint64_t>(actionSet));
	}
	return std::format("{}, ({:#016x})", sCache.mActionSets.at(actionSet),
	    OXRTL_HANDLE_CAST<const uint64_t>(actionSet));
}

void xrCreateActionSet_hook(XrResult result, XrInstance instance,
    const XrActionSetCreateInfo* createInfo, XrActionSet* actionSet) noexcept
{
	if (!XR_SUCCEEDED(result)) {
		return;
	}

	if (instance != sCache.mXrInstance) {
		sCache = { instance };
	}

	sCache.mActionSets[*actionSet] = createInfo->actionSetName;
}

void xrCreateAction_hook(XrResult result, XrActionSet actionSet,
    const XrActionCreateInfo* createInfo, XrAction* action)
{
	if (!XR_SUCCEEDED(result)) {
		return;
	}
	if (gXrInstance != sCache.mXrInstance) {
		sCache = { gXrInstance };
	}

	sCache.mActions[*action] = {
		.mName = createInfo->actionName,
		.mActionSet = actionSet,
	};
}

} // namespace OXRTracing
