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

namespace {
struct ActionInfo {
	std::string mName;
	XrActionSet mActionSet{};
};

struct Cache {
	XrInstance mXrInstance{};
	std::unordered_map<XrPath, std::string> mPaths;
	std::unordered_map<XrActionSet, std::string> mActionSets;
	std::unordered_map<XrAction, std::string> mActions;

	std::unordered_map<XrSpace, std::string> mSpaces;
};
thread_local Cache sCache{};

static inline void ValidateCache(XrInstance instance = gXrInstance)
{
	if (sCache.mXrInstance != instance) {
		sCache = { instance };
	}
}

constexpr XrPosef XR_POSEF_IDENTITY{
	.orientation = { 0.f, 0.f, 0.f, 1.f },
	.position = { 0.f, 0.f, 0.f },
};

} // namespace

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

std::string XrPath_to_string(XrPath path)
{
	if (!path) {
		return "XR_NULL_PATH";
	}

	if (!sCache.mPaths.contains(path)) {
		return std::format("{:#016x}", path);
	}

	return sCache.mPaths.at(path);
}

std::string XrAction_to_string(XrAction action)
{
	if (!sCache.mActions.contains(action)) {
		return std::format(
		    "{:#016x}", OXRTL_HANDLE_CAST<const uint64_t>(action));
	}

	return sCache.mActions.at(action);
}

std::string XrActionSet_to_string(XrActionSet actionSet)
{
	if (!sCache.mActionSets.contains(actionSet)) {
		return std::format(
		    "{:#016x}", OXRTL_HANDLE_CAST<const uint64_t>(actionSet));
	}

	return sCache.mActionSets.at(actionSet);
}

std::string XrSpace_to_string(XrSpace space)
{
	if (!sCache.mSpaces.contains(space)) {
		return std::format(
		    "{:#016x}", OXRTL_HANDLE_CAST<const uint64_t>(space));
	}
	return sCache.mSpaces.at(space);
}

void xrCreateActionSet_hook(XrResult result, XrInstance instance,
    const XrActionSetCreateInfo* createInfo, XrActionSet* actionSet) noexcept
{
	if (!XR_SUCCEEDED(result)) {
		return;
	}

	ValidateCache(instance);

	sCache.mActionSets[*actionSet]
	    = std::format("{} {:#016x}", createInfo->actionSetName,
	        OXRTL_HANDLE_CAST<const uint64_t>(*actionSet));
}

void xrCreateAction_hook(XrResult result, XrActionSet actionSet,
    const XrActionCreateInfo* createInfo, XrAction* action)
{
	if (!XR_SUCCEEDED(result)) {
		return;
	}

	ValidateCache();

	sCache.mActions[*action] = std::format("{} ({:#016x}) from {}",
	    createInfo->actionName, OXRTL_HANDLE_CAST<const uint64_t>(*action),
	    XrActionSet_to_string(actionSet));
}

void xrStringToPath_hook(
    XrResult result, XrInstance instance, const char* pathString, XrPath* path)
{
	if (!XR_SUCCEEDED(result)) {
		return;
	}

	ValidateCache(instance);

	sCache.mPaths[*path] = std::format("{} ({:#016x})", pathString, *path);
}

void xrCreateActionSpace_hook(XrResult result, XrSession session,
    const XrActionSpaceCreateInfo* createInfo, XrSpace* space)
{
	if (!XR_SUCCEEDED(result)) {
		return;
	}

	ValidateCache();

	if (!createInfo->subactionPath) {
		sCache.mSpaces[*space] = std::format("actionSpace {} - ({:#016x})",
		    XrAction_to_string(createInfo->action),
		    OXRTL_HANDLE_CAST<const uint64_t>(*space));
		return;
	}
	sCache.mSpaces[*space] = std::format("actionSpace {} @ {} - ({:#016x})",
	    XrAction_to_string(createInfo->action),
	    XrPath_to_string(createInfo->subactionPath),
	    OXRTL_HANDLE_CAST<const uint64_t>(*space));
}

void xrCreateReferenceSpace_hook(XrResult result, XrSession session,
    const XrReferenceSpaceCreateInfo* createInfo, XrSpace* space)
{
	if (!XR_SUCCEEDED(result)) {
		return;
	}

	ValidateCache();

	const auto& pose = createInfo->poseInReferenceSpace;
	if (memcmp(&pose, &XR_POSEF_IDENTITY, sizeof(XrPosef)) == 0) {
		sCache.mSpaces[*space] = std::format("{} ({:#016x})",
		    to_string(createInfo->referenceSpaceType),
		    OXRTL_HANDLE_CAST<const uint64_t>(*space));
		return;
	}

	const auto& o = pose.orientation;
	const auto& p = pose.position;
	sCache.mSpaces[*space]
	    = std::format("{} @ o({}, {}, {}, {}) p({}, {}, {}) - ({:#016x})",
	        to_string(createInfo->referenceSpaceType), o.x, o.y, o.z, o.w, p.x,
	        p.y, p.z, OXRTL_HANDLE_CAST<const uint64_t>(*space));
}

} // namespace OXRTracing
