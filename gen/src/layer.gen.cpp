// *********** THIS FILE IS GENERATED - DO NOT EDIT ***********
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

#include <OXRTracing.hpp>
#include <openxr.h>

#include <TraceLoggingActivity.h>
#include <TraceLoggingProvider.h>

namespace OXRTracing {
TRACELOGGING_DECLARE_PROVIDER(gTraceProvider);
static thread_local XrInstance gXrInstance{};
extern PFN_xrGetInstanceProcAddr gXrNextGetInstanceProcAddr;
} // namespace OXRTracing

XrResult OXRTracing_xrGetInstanceProcAddr(
    XrInstance instance, const char* name, PFN_xrVoidFunction* function);

using namespace OXRTracing;

static PFN_xrDestroyInstance next_xrDestroyInstance{ nullptr };
static PFN_xrGetInstanceProperties next_xrGetInstanceProperties{ nullptr };
static PFN_xrPollEvent next_xrPollEvent{ nullptr };
static PFN_xrResultToString next_xrResultToString{ nullptr };
static PFN_xrStructureTypeToString next_xrStructureTypeToString{ nullptr };
static PFN_xrGetSystem next_xrGetSystem{ nullptr };
static PFN_xrGetSystemProperties next_xrGetSystemProperties{ nullptr };
static PFN_xrEnumerateEnvironmentBlendModes
    next_xrEnumerateEnvironmentBlendModes{ nullptr };
static PFN_xrCreateSession next_xrCreateSession{ nullptr };
static PFN_xrDestroySession next_xrDestroySession{ nullptr };
static PFN_xrEnumerateReferenceSpaces next_xrEnumerateReferenceSpaces{
	nullptr
};
static PFN_xrCreateReferenceSpace next_xrCreateReferenceSpace{ nullptr };
static PFN_xrGetReferenceSpaceBoundsRect next_xrGetReferenceSpaceBoundsRect{
	nullptr
};
static PFN_xrCreateActionSpace next_xrCreateActionSpace{ nullptr };
static PFN_xrLocateSpace next_xrLocateSpace{ nullptr };
static PFN_xrDestroySpace next_xrDestroySpace{ nullptr };
static PFN_xrEnumerateViewConfigurations next_xrEnumerateViewConfigurations{
	nullptr
};
static PFN_xrGetViewConfigurationProperties
    next_xrGetViewConfigurationProperties{ nullptr };
static PFN_xrEnumerateViewConfigurationViews
    next_xrEnumerateViewConfigurationViews{ nullptr };
static PFN_xrEnumerateSwapchainFormats next_xrEnumerateSwapchainFormats{
	nullptr
};
static PFN_xrCreateSwapchain next_xrCreateSwapchain{ nullptr };
static PFN_xrDestroySwapchain next_xrDestroySwapchain{ nullptr };
static PFN_xrEnumerateSwapchainImages next_xrEnumerateSwapchainImages{
	nullptr
};
static PFN_xrAcquireSwapchainImage next_xrAcquireSwapchainImage{ nullptr };
static PFN_xrWaitSwapchainImage next_xrWaitSwapchainImage{ nullptr };
static PFN_xrReleaseSwapchainImage next_xrReleaseSwapchainImage{ nullptr };
static PFN_xrBeginSession next_xrBeginSession{ nullptr };
static PFN_xrEndSession next_xrEndSession{ nullptr };
static PFN_xrRequestExitSession next_xrRequestExitSession{ nullptr };
static PFN_xrWaitFrame next_xrWaitFrame{ nullptr };
static PFN_xrBeginFrame next_xrBeginFrame{ nullptr };
static PFN_xrEndFrame next_xrEndFrame{ nullptr };
static PFN_xrLocateViews next_xrLocateViews{ nullptr };
static PFN_xrStringToPath next_xrStringToPath{ nullptr };
static PFN_xrPathToString next_xrPathToString{ nullptr };
static PFN_xrCreateActionSet next_xrCreateActionSet{ nullptr };
static PFN_xrDestroyActionSet next_xrDestroyActionSet{ nullptr };
static PFN_xrCreateAction next_xrCreateAction{ nullptr };
static PFN_xrDestroyAction next_xrDestroyAction{ nullptr };
static PFN_xrSuggestInteractionProfileBindings
    next_xrSuggestInteractionProfileBindings{ nullptr };
static PFN_xrAttachSessionActionSets next_xrAttachSessionActionSets{ nullptr };
static PFN_xrGetCurrentInteractionProfile next_xrGetCurrentInteractionProfile{
	nullptr
};
static PFN_xrGetActionStateBoolean next_xrGetActionStateBoolean{ nullptr };
static PFN_xrGetActionStateFloat next_xrGetActionStateFloat{ nullptr };
static PFN_xrGetActionStateVector2f next_xrGetActionStateVector2f{ nullptr };
static PFN_xrGetActionStatePose next_xrGetActionStatePose{ nullptr };
static PFN_xrSyncActions next_xrSyncActions{ nullptr };
static PFN_xrEnumerateBoundSourcesForAction
    next_xrEnumerateBoundSourcesForAction{ nullptr };
static PFN_xrGetInputSourceLocalizedName next_xrGetInputSourceLocalizedName{
	nullptr
};
static PFN_xrApplyHapticFeedback next_xrApplyHapticFeedback{ nullptr };
static PFN_xrStopHapticFeedback next_xrStopHapticFeedback{ nullptr };

XrResult OXRTracing_xrDestroyInstance(XrInstance instance)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrDestroyInstance",
	    OXRTL_ARGS_XrInstance(instance, "instance"));

	const auto ret = next_xrDestroyInstance(instance);

	TraceLoggingWriteStop(localActivity, "xrDestroyInstance",
	    OXRTL_ARGS_XrResult(ret, "XrResult"));

	if (gXrInstance == instance) {
		gXrInstance = {};
	}

	return ret;
}

XrResult OXRTracing_xrGetInstanceProperties(
    XrInstance instance, XrInstanceProperties* instanceProperties)
{
	gXrInstance = instance;
	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrGetInstanceProperties",
	    OXRTL_ARGS_XrInstance(instance, "instance"));

	const auto ret = next_xrGetInstanceProperties(instance, instanceProperties);
	// XRTracing::TraceNext(localActivity,
	// "xrGetInstanceProperties_instanceProperties", instanceProperties);
	TraceLoggingWriteStop(localActivity, "xrGetInstanceProperties",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_XrInstanceProperties_P(
	        instanceProperties, "instanceProperties"));

	return ret;
}

XrResult OXRTracing_xrPollEvent(
    XrInstance instance, XrEventDataBuffer* eventData)
{
	gXrInstance = instance;
	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrPollEvent",
	    OXRTL_ARGS_XrInstance(instance, "instance"));

	const auto ret = next_xrPollEvent(instance, eventData);
	// XRTracing::TraceNext(localActivity, "xrPollEvent_eventData", eventData);
	TraceLoggingWriteStop(localActivity, "xrPollEvent",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_XrEventDataBuffer_P(eventData, "eventData"));

	return ret;
}

XrResult OXRTracing_xrResultToString(
    XrInstance instance, XrResult value, char buffer[XR_MAX_RESULT_STRING_SIZE])
{
	gXrInstance = instance;
	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrResultToString",
	    OXRTL_ARGS_XrInstance(instance, "instance"),
	    OXRTL_ARGS_XrResult(value, "value"));

	const auto ret = next_xrResultToString(instance, value, buffer);

	TraceLoggingWriteStop(localActivity, "xrResultToString",
	    OXRTL_ARGS_XrResult(ret, "XrResult"));

	return ret;
}

XrResult OXRTracing_xrStructureTypeToString(XrInstance instance,
    XrStructureType value, char buffer[XR_MAX_STRUCTURE_NAME_SIZE])
{
	gXrInstance = instance;
	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrStructureTypeToString",
	    OXRTL_ARGS_XrInstance(instance, "instance"),
	    OXRTL_ARGS_XrStructureType(value, "value"));

	const auto ret = next_xrStructureTypeToString(instance, value, buffer);

	TraceLoggingWriteStop(localActivity, "xrStructureTypeToString",
	    OXRTL_ARGS_XrResult(ret, "XrResult"));

	return ret;
}

XrResult OXRTracing_xrGetSystem(
    XrInstance instance, const XrSystemGetInfo* getInfo, XrSystemId* systemId)
{
	gXrInstance = instance;
	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrGetSystem",
	    OXRTL_ARGS_XrInstance(instance, "instance"),
	    OXRTL_ARGS_XrSystemGetInfo_P(getInfo, "getInfo"));
	// XRTracing::TraceNext(localActivity, "xrGetSystem_getInfo", getInfo);
	const auto ret = next_xrGetSystem(instance, getInfo, systemId);

	TraceLoggingWriteStop(localActivity, "xrGetSystem",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_XrSystemId((*systemId), "systemId"));

	return ret;
}

XrResult OXRTracing_xrGetSystemProperties(
    XrInstance instance, XrSystemId systemId, XrSystemProperties* properties)
{
	gXrInstance = instance;
	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrGetSystemProperties",
	    OXRTL_ARGS_XrInstance(instance, "instance"),
	    OXRTL_ARGS_XrSystemId(systemId, "systemId"));

	const auto ret = next_xrGetSystemProperties(instance, systemId, properties);
	// XRTracing::TraceNext(localActivity, "xrGetSystemProperties_properties",
	// properties);
	TraceLoggingWriteStop(localActivity, "xrGetSystemProperties",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_XrSystemProperties_P(properties, "properties"));

	return ret;
}

XrResult OXRTracing_xrEnumerateEnvironmentBlendModes(XrInstance instance,
    XrSystemId systemId, XrViewConfigurationType viewConfigurationType,
    uint32_t environmentBlendModeCapacityInput,
    uint32_t* environmentBlendModeCountOutput,
    XrEnvironmentBlendMode* environmentBlendModes)
{
	gXrInstance = instance;
	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrEnumerateEnvironmentBlendModes",
	    OXRTL_ARGS_XrInstance(instance, "instance"),
	    OXRTL_ARGS_XrSystemId(systemId, "systemId"),
	    OXRTL_ARGS_XrViewConfigurationType(
	        viewConfigurationType, "viewConfigurationType"),
	    OXRTL_ARGS_uint32_t(environmentBlendModeCapacityInput,
	        "environmentBlendModeCapacityInput"));

	const auto ret = next_xrEnumerateEnvironmentBlendModes(instance, systemId,
	    viewConfigurationType, environmentBlendModeCapacityInput,
	    environmentBlendModeCountOutput, environmentBlendModes);

	TraceLoggingWriteStop(localActivity, "xrEnumerateEnvironmentBlendModes",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_uint32_t((*environmentBlendModeCountOutput),
	        "environmentBlendModeCountOutput"));

	return ret;
}

XrResult OXRTracing_xrCreateSession(XrInstance instance,
    const XrSessionCreateInfo* createInfo, XrSession* session)
{
	gXrInstance = instance;
	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrCreateSession",
	    OXRTL_ARGS_XrInstance(instance, "instance"),
	    OXRTL_ARGS_XrSessionCreateInfo_P(createInfo, "createInfo"));
	// XRTracing::TraceNext(localActivity, "xrCreateSession_createInfo",
	// createInfo);
	const auto ret = next_xrCreateSession(instance, createInfo, session);

	TraceLoggingWriteStop(localActivity, "xrCreateSession",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_XrSession((*session), "session"));

	return ret;
}

XrResult OXRTracing_xrDestroySession(XrSession session)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrDestroySession",
	    OXRTL_ARGS_XrSession(session, "session"));

	const auto ret = next_xrDestroySession(session);

	TraceLoggingWriteStop(localActivity, "xrDestroySession",
	    OXRTL_ARGS_XrResult(ret, "XrResult"));

	return ret;
}

XrResult OXRTracing_xrEnumerateReferenceSpaces(XrSession session,
    uint32_t spaceCapacityInput, uint32_t* spaceCountOutput,
    XrReferenceSpaceType* spaces)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrEnumerateReferenceSpaces",
	    OXRTL_ARGS_XrSession(session, "session"),
	    OXRTL_ARGS_uint32_t(spaceCapacityInput, "spaceCapacityInput"));

	const auto ret = next_xrEnumerateReferenceSpaces(
	    session, spaceCapacityInput, spaceCountOutput, spaces);

	TraceLoggingWriteStop(localActivity, "xrEnumerateReferenceSpaces",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_uint32_t((*spaceCountOutput), "spaceCountOutput"));

	return ret;
}

XrResult OXRTracing_xrCreateReferenceSpace(XrSession session,
    const XrReferenceSpaceCreateInfo* createInfo, XrSpace* space)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrCreateReferenceSpace",
	    OXRTL_ARGS_XrSession(session, "session"),
	    OXRTL_ARGS_XrReferenceSpaceCreateInfo_P(createInfo, "createInfo"));
	// XRTracing::TraceNext(localActivity, "xrCreateReferenceSpace_createInfo",
	// createInfo);
	const auto ret = next_xrCreateReferenceSpace(session, createInfo, space);

	TraceLoggingWriteStop(localActivity, "xrCreateReferenceSpace",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_XrSpace((*space), "space"));

	return ret;
}

XrResult OXRTracing_xrGetReferenceSpaceBoundsRect(XrSession session,
    XrReferenceSpaceType referenceSpaceType, XrExtent2Df* bounds)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrGetReferenceSpaceBoundsRect",
	    OXRTL_ARGS_XrSession(session, "session"),
	    OXRTL_ARGS_XrReferenceSpaceType(
	        referenceSpaceType, "referenceSpaceType"));

	const auto ret = next_xrGetReferenceSpaceBoundsRect(
	    session, referenceSpaceType, bounds);
	// XRTracing::TraceNext(localActivity,
	// "xrGetReferenceSpaceBoundsRect_bounds", bounds);
	TraceLoggingWriteStop(localActivity, "xrGetReferenceSpaceBoundsRect",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_XrExtent2Df_P(bounds, "bounds"));

	return ret;
}

XrResult OXRTracing_xrCreateActionSpace(XrSession session,
    const XrActionSpaceCreateInfo* createInfo, XrSpace* space)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrCreateActionSpace",
	    OXRTL_ARGS_XrSession(session, "session"),
	    OXRTL_ARGS_XrActionSpaceCreateInfo_P(createInfo, "createInfo"));
	// XRTracing::TraceNext(localActivity, "xrCreateActionSpace_createInfo",
	// createInfo);
	const auto ret = next_xrCreateActionSpace(session, createInfo, space);

	TraceLoggingWriteStop(localActivity, "xrCreateActionSpace",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_XrSpace((*space), "space"));

	return ret;
}

XrResult OXRTracing_xrLocateSpace(
    XrSpace space, XrSpace baseSpace, XrTime time, XrSpaceLocation* location)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrLocateSpace",
	    OXRTL_ARGS_XrSpace(space, "space"),
	    OXRTL_ARGS_XrSpace(baseSpace, "baseSpace"),
	    OXRTL_ARGS_XrTime(time, "time"));

	const auto ret = next_xrLocateSpace(space, baseSpace, time, location);
	// XRTracing::TraceNext(localActivity, "xrLocateSpace_location", location);
	TraceLoggingWriteStop(localActivity, "xrLocateSpace",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_XrSpaceLocation_P(location, "location"));

	return ret;
}

XrResult OXRTracing_xrDestroySpace(XrSpace space)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(
	    localActivity, "xrDestroySpace", OXRTL_ARGS_XrSpace(space, "space"));

	const auto ret = next_xrDestroySpace(space);

	TraceLoggingWriteStop(
	    localActivity, "xrDestroySpace", OXRTL_ARGS_XrResult(ret, "XrResult"));

	return ret;
}

XrResult OXRTracing_xrEnumerateViewConfigurations(XrInstance instance,
    XrSystemId systemId, uint32_t viewConfigurationTypeCapacityInput,
    uint32_t* viewConfigurationTypeCountOutput,
    XrViewConfigurationType* viewConfigurationTypes)
{
	gXrInstance = instance;
	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrEnumerateViewConfigurations",
	    OXRTL_ARGS_XrInstance(instance, "instance"),
	    OXRTL_ARGS_XrSystemId(systemId, "systemId"),
	    OXRTL_ARGS_uint32_t(viewConfigurationTypeCapacityInput,
	        "viewConfigurationTypeCapacityInput"));

	const auto ret = next_xrEnumerateViewConfigurations(instance, systemId,
	    viewConfigurationTypeCapacityInput, viewConfigurationTypeCountOutput,
	    viewConfigurationTypes);

	TraceLoggingWriteStop(localActivity, "xrEnumerateViewConfigurations",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_uint32_t((*viewConfigurationTypeCountOutput),
	        "viewConfigurationTypeCountOutput"));

	return ret;
}

XrResult OXRTracing_xrGetViewConfigurationProperties(XrInstance instance,
    XrSystemId systemId, XrViewConfigurationType viewConfigurationType,
    XrViewConfigurationProperties* configurationProperties)
{
	gXrInstance = instance;
	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrGetViewConfigurationProperties",
	    OXRTL_ARGS_XrInstance(instance, "instance"),
	    OXRTL_ARGS_XrSystemId(systemId, "systemId"),
	    OXRTL_ARGS_XrViewConfigurationType(
	        viewConfigurationType, "viewConfigurationType"));

	const auto ret = next_xrGetViewConfigurationProperties(
	    instance, systemId, viewConfigurationType, configurationProperties);
	// XRTracing::TraceNext(localActivity,
	// "xrGetViewConfigurationProperties_configurationProperties",
	// configurationProperties);
	TraceLoggingWriteStop(localActivity, "xrGetViewConfigurationProperties",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_XrViewConfigurationProperties_P(
	        configurationProperties, "configurationProperties"));

	return ret;
}

XrResult OXRTracing_xrEnumerateViewConfigurationViews(XrInstance instance,
    XrSystemId systemId, XrViewConfigurationType viewConfigurationType,
    uint32_t viewCapacityInput, uint32_t* viewCountOutput,
    XrViewConfigurationView* views)
{
	gXrInstance = instance;
	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrEnumerateViewConfigurationViews",
	    OXRTL_ARGS_XrInstance(instance, "instance"),
	    OXRTL_ARGS_XrSystemId(systemId, "systemId"),
	    OXRTL_ARGS_XrViewConfigurationType(
	        viewConfigurationType, "viewConfigurationType"),
	    OXRTL_ARGS_uint32_t(viewCapacityInput, "viewCapacityInput"));

	const auto ret = next_xrEnumerateViewConfigurationViews(instance, systemId,
	    viewConfigurationType, viewCapacityInput, viewCountOutput, views);

	TraceLoggingWriteStop(localActivity, "xrEnumerateViewConfigurationViews",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_uint32_t((*viewCountOutput), "viewCountOutput"));

	return ret;
}

XrResult OXRTracing_xrEnumerateSwapchainFormats(XrSession session,
    uint32_t formatCapacityInput, uint32_t* formatCountOutput, int64_t* formats)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrEnumerateSwapchainFormats",
	    OXRTL_ARGS_XrSession(session, "session"),
	    OXRTL_ARGS_uint32_t(formatCapacityInput, "formatCapacityInput"));

	const auto ret = next_xrEnumerateSwapchainFormats(
	    session, formatCapacityInput, formatCountOutput, formats);

	TraceLoggingWriteStop(localActivity, "xrEnumerateSwapchainFormats",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_uint32_t((*formatCountOutput), "formatCountOutput"));

	return ret;
}

XrResult OXRTracing_xrCreateSwapchain(XrSession session,
    const XrSwapchainCreateInfo* createInfo, XrSwapchain* swapchain)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrCreateSwapchain",
	    OXRTL_ARGS_XrSession(session, "session"),
	    OXRTL_ARGS_XrSwapchainCreateInfo_P(createInfo, "createInfo"));
	// XRTracing::TraceNext(localActivity, "xrCreateSwapchain_createInfo",
	// createInfo);
	const auto ret = next_xrCreateSwapchain(session, createInfo, swapchain);

	TraceLoggingWriteStop(localActivity, "xrCreateSwapchain",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_XrSwapchain((*swapchain), "swapchain"));

	return ret;
}

XrResult OXRTracing_xrDestroySwapchain(XrSwapchain swapchain)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrDestroySwapchain",
	    OXRTL_ARGS_XrSwapchain(swapchain, "swapchain"));

	const auto ret = next_xrDestroySwapchain(swapchain);

	TraceLoggingWriteStop(localActivity, "xrDestroySwapchain",
	    OXRTL_ARGS_XrResult(ret, "XrResult"));

	return ret;
}

XrResult OXRTracing_xrEnumerateSwapchainImages(XrSwapchain swapchain,
    uint32_t imageCapacityInput, uint32_t* imageCountOutput,
    XrSwapchainImageBaseHeader* images)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrEnumerateSwapchainImages",
	    OXRTL_ARGS_XrSwapchain(swapchain, "swapchain"),
	    OXRTL_ARGS_uint32_t(imageCapacityInput, "imageCapacityInput"));

	const auto ret = next_xrEnumerateSwapchainImages(
	    swapchain, imageCapacityInput, imageCountOutput, images);

	TraceLoggingWriteStop(localActivity, "xrEnumerateSwapchainImages",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_uint32_t((*imageCountOutput), "imageCountOutput"));

	return ret;
}

XrResult OXRTracing_xrAcquireSwapchainImage(XrSwapchain swapchain,
    const XrSwapchainImageAcquireInfo* acquireInfo, uint32_t* index)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrAcquireSwapchainImage",
	    OXRTL_ARGS_XrSwapchain(swapchain, "swapchain"),
	    OXRTL_ARGS_XrSwapchainImageAcquireInfo_P(acquireInfo, "acquireInfo"));
	// XRTracing::TraceNext(localActivity,
	// "xrAcquireSwapchainImage_acquireInfo", acquireInfo);
	const auto ret
	    = next_xrAcquireSwapchainImage(swapchain, acquireInfo, index);

	TraceLoggingWriteStop(localActivity, "xrAcquireSwapchainImage",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_uint32_t((*index), "index"));

	return ret;
}

XrResult OXRTracing_xrWaitSwapchainImage(
    XrSwapchain swapchain, const XrSwapchainImageWaitInfo* waitInfo)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrWaitSwapchainImage",
	    OXRTL_ARGS_XrSwapchain(swapchain, "swapchain"),
	    OXRTL_ARGS_XrSwapchainImageWaitInfo_P(waitInfo, "waitInfo"));
	// XRTracing::TraceNext(localActivity, "xrWaitSwapchainImage_waitInfo",
	// waitInfo);
	const auto ret = next_xrWaitSwapchainImage(swapchain, waitInfo);

	TraceLoggingWriteStop(localActivity, "xrWaitSwapchainImage",
	    OXRTL_ARGS_XrResult(ret, "XrResult"));

	return ret;
}

XrResult OXRTracing_xrReleaseSwapchainImage(
    XrSwapchain swapchain, const XrSwapchainImageReleaseInfo* releaseInfo)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrReleaseSwapchainImage",
	    OXRTL_ARGS_XrSwapchain(swapchain, "swapchain"),
	    OXRTL_ARGS_XrSwapchainImageReleaseInfo_P(releaseInfo, "releaseInfo"));
	// XRTracing::TraceNext(localActivity,
	// "xrReleaseSwapchainImage_releaseInfo", releaseInfo);
	const auto ret = next_xrReleaseSwapchainImage(swapchain, releaseInfo);

	TraceLoggingWriteStop(localActivity, "xrReleaseSwapchainImage",
	    OXRTL_ARGS_XrResult(ret, "XrResult"));

	return ret;
}

XrResult OXRTracing_xrBeginSession(
    XrSession session, const XrSessionBeginInfo* beginInfo)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrBeginSession",
	    OXRTL_ARGS_XrSession(session, "session"),
	    OXRTL_ARGS_XrSessionBeginInfo_P(beginInfo, "beginInfo"));
	// XRTracing::TraceNext(localActivity, "xrBeginSession_beginInfo",
	// beginInfo);
	const auto ret = next_xrBeginSession(session, beginInfo);

	TraceLoggingWriteStop(
	    localActivity, "xrBeginSession", OXRTL_ARGS_XrResult(ret, "XrResult"));

	return ret;
}

XrResult OXRTracing_xrEndSession(XrSession session)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrEndSession",
	    OXRTL_ARGS_XrSession(session, "session"));

	const auto ret = next_xrEndSession(session);

	TraceLoggingWriteStop(
	    localActivity, "xrEndSession", OXRTL_ARGS_XrResult(ret, "XrResult"));

	return ret;
}

XrResult OXRTracing_xrRequestExitSession(XrSession session)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrRequestExitSession",
	    OXRTL_ARGS_XrSession(session, "session"));

	const auto ret = next_xrRequestExitSession(session);

	TraceLoggingWriteStop(localActivity, "xrRequestExitSession",
	    OXRTL_ARGS_XrResult(ret, "XrResult"));

	return ret;
}

XrResult OXRTracing_xrWaitFrame(XrSession session,
    const XrFrameWaitInfo* frameWaitInfo, XrFrameState* frameState)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrWaitFrame",
	    OXRTL_ARGS_XrSession(session, "session"),
	    OXRTL_ARGS_XrFrameWaitInfo_P(frameWaitInfo, "frameWaitInfo"));
	// XRTracing::TraceNext(localActivity, "xrWaitFrame_frameWaitInfo",
	// frameWaitInfo);
	const auto ret = next_xrWaitFrame(session, frameWaitInfo, frameState);
	// XRTracing::TraceNext(localActivity, "xrWaitFrame_frameState",
	// frameState);
	TraceLoggingWriteStop(localActivity, "xrWaitFrame",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_XrFrameState_P(frameState, "frameState"));

	return ret;
}

XrResult OXRTracing_xrBeginFrame(
    XrSession session, const XrFrameBeginInfo* frameBeginInfo)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrBeginFrame",
	    OXRTL_ARGS_XrSession(session, "session"),
	    OXRTL_ARGS_XrFrameBeginInfo_P(frameBeginInfo, "frameBeginInfo"));
	// XRTracing::TraceNext(localActivity, "xrBeginFrame_frameBeginInfo",
	// frameBeginInfo);
	const auto ret = next_xrBeginFrame(session, frameBeginInfo);

	TraceLoggingWriteStop(
	    localActivity, "xrBeginFrame", OXRTL_ARGS_XrResult(ret, "XrResult"));

	return ret;
}

XrResult OXRTracing_xrEndFrame(
    XrSession session, const XrFrameEndInfo* frameEndInfo)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrEndFrame",
	    OXRTL_ARGS_XrSession(session, "session"),
	    OXRTL_ARGS_XrFrameEndInfo_P(frameEndInfo, "frameEndInfo"));
	// XRTracing::TraceNext(localActivity, "xrEndFrame_frameEndInfo",
	// frameEndInfo);
	const auto ret = next_xrEndFrame(session, frameEndInfo);

	TraceLoggingWriteStop(
	    localActivity, "xrEndFrame", OXRTL_ARGS_XrResult(ret, "XrResult"));

	return ret;
}

XrResult OXRTracing_xrLocateViews(XrSession session,
    const XrViewLocateInfo* viewLocateInfo, XrViewState* viewState,
    uint32_t viewCapacityInput, uint32_t* viewCountOutput, XrView* views)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrLocateViews",
	    OXRTL_ARGS_XrSession(session, "session"),
	    OXRTL_ARGS_XrViewLocateInfo_P(viewLocateInfo, "viewLocateInfo"),
	    OXRTL_ARGS_uint32_t(viewCapacityInput, "viewCapacityInput"));
	// XRTracing::TraceNext(localActivity, "xrLocateViews_viewLocateInfo",
	// viewLocateInfo);
	const auto ret = next_xrLocateViews(session, viewLocateInfo, viewState,
	    viewCapacityInput, viewCountOutput, views);
	// XRTracing::TraceNext(localActivity, "xrLocateViews_viewState",
	// viewState);
	TraceLoggingWriteStop(localActivity, "xrLocateViews",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_XrViewState_P(viewState, "viewState"),
	    OXRTL_ARGS_uint32_t((*viewCountOutput), "viewCountOutput"));

	return ret;
}

XrResult OXRTracing_xrStringToPath(
    XrInstance instance, const char* pathString, XrPath* path)
{
	gXrInstance = instance;
	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrStringToPath",
	    OXRTL_ARGS_XrInstance(instance, "instance"),
	    OXRTL_ARGS_char_P(pathString, "pathString"));

	const auto ret = next_xrStringToPath(instance, pathString, path);

	TraceLoggingWriteStop(localActivity, "xrStringToPath",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_XrPath((*path), "path"));

	return ret;
}

XrResult OXRTracing_xrPathToString(XrInstance instance, XrPath path,
    uint32_t bufferCapacityInput, uint32_t* bufferCountOutput, char* buffer)
{
	gXrInstance = instance;
	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrPathToString",
	    OXRTL_ARGS_XrInstance(instance, "instance"),
	    OXRTL_ARGS_XrPath(path, "path"),
	    OXRTL_ARGS_uint32_t(bufferCapacityInput, "bufferCapacityInput"));

	const auto ret = next_xrPathToString(
	    instance, path, bufferCapacityInput, bufferCountOutput, buffer);

	TraceLoggingWriteStop(localActivity, "xrPathToString",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_uint32_t((*bufferCountOutput), "bufferCountOutput"));

	return ret;
}

XrResult OXRTracing_xrCreateActionSet(XrInstance instance,
    const XrActionSetCreateInfo* createInfo, XrActionSet* actionSet)
{
	gXrInstance = instance;
	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrCreateActionSet",
	    OXRTL_ARGS_XrInstance(instance, "instance"),
	    OXRTL_ARGS_XrActionSetCreateInfo_P(createInfo, "createInfo"));
	// XRTracing::TraceNext(localActivity, "xrCreateActionSet_createInfo",
	// createInfo);
	const auto ret = next_xrCreateActionSet(instance, createInfo, actionSet);

	TraceLoggingWriteStop(localActivity, "xrCreateActionSet",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_XrActionSet((*actionSet), "actionSet"));

	return ret;
}

XrResult OXRTracing_xrDestroyActionSet(XrActionSet actionSet)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrDestroyActionSet",
	    OXRTL_ARGS_XrActionSet(actionSet, "actionSet"));

	const auto ret = next_xrDestroyActionSet(actionSet);

	TraceLoggingWriteStop(localActivity, "xrDestroyActionSet",
	    OXRTL_ARGS_XrResult(ret, "XrResult"));

	return ret;
}

XrResult OXRTracing_xrCreateAction(XrActionSet actionSet,
    const XrActionCreateInfo* createInfo, XrAction* action)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrCreateAction",
	    OXRTL_ARGS_XrActionSet(actionSet, "actionSet"),
	    OXRTL_ARGS_XrActionCreateInfo_P(createInfo, "createInfo"));
	// XRTracing::TraceNext(localActivity, "xrCreateAction_createInfo",
	// createInfo);
	const auto ret = next_xrCreateAction(actionSet, createInfo, action);

	TraceLoggingWriteStop(localActivity, "xrCreateAction",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_XrAction((*action), "action"));

	return ret;
}

XrResult OXRTracing_xrDestroyAction(XrAction action)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrDestroyAction",
	    OXRTL_ARGS_XrAction(action, "action"));

	const auto ret = next_xrDestroyAction(action);

	TraceLoggingWriteStop(
	    localActivity, "xrDestroyAction", OXRTL_ARGS_XrResult(ret, "XrResult"));

	return ret;
}

XrResult OXRTracing_xrSuggestInteractionProfileBindings(XrInstance instance,
    const XrInteractionProfileSuggestedBinding* suggestedBindings)
{
	gXrInstance = instance;
	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrSuggestInteractionProfileBindings",
	    OXRTL_ARGS_XrInstance(instance, "instance"),
	    OXRTL_ARGS_XrInteractionProfileSuggestedBinding_P(
	        suggestedBindings, "suggestedBindings"));
	// XRTracing::TraceNext(localActivity,
	// "xrSuggestInteractionProfileBindings_suggestedBindings",
	// suggestedBindings);
	const auto ret
	    = next_xrSuggestInteractionProfileBindings(instance, suggestedBindings);

	TraceLoggingWriteStop(localActivity, "xrSuggestInteractionProfileBindings",
	    OXRTL_ARGS_XrResult(ret, "XrResult"));

	return ret;
}

XrResult OXRTracing_xrAttachSessionActionSets(
    XrSession session, const XrSessionActionSetsAttachInfo* attachInfo)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrAttachSessionActionSets",
	    OXRTL_ARGS_XrSession(session, "session"),
	    OXRTL_ARGS_XrSessionActionSetsAttachInfo_P(attachInfo, "attachInfo"));
	// XRTracing::TraceNext(localActivity,
	// "xrAttachSessionActionSets_attachInfo", attachInfo);
	const auto ret = next_xrAttachSessionActionSets(session, attachInfo);

	TraceLoggingWriteStop(localActivity, "xrAttachSessionActionSets",
	    OXRTL_ARGS_XrResult(ret, "XrResult"));

	return ret;
}

XrResult OXRTracing_xrGetCurrentInteractionProfile(XrSession session,
    XrPath topLevelUserPath, XrInteractionProfileState* interactionProfile)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrGetCurrentInteractionProfile",
	    OXRTL_ARGS_XrSession(session, "session"),
	    OXRTL_ARGS_XrPath(topLevelUserPath, "topLevelUserPath"));

	const auto ret = next_xrGetCurrentInteractionProfile(
	    session, topLevelUserPath, interactionProfile);
	// XRTracing::TraceNext(localActivity,
	// "xrGetCurrentInteractionProfile_interactionProfile", interactionProfile);
	TraceLoggingWriteStop(localActivity, "xrGetCurrentInteractionProfile",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_XrInteractionProfileState_P(
	        interactionProfile, "interactionProfile"));

	return ret;
}

XrResult OXRTracing_xrGetActionStateBoolean(XrSession session,
    const XrActionStateGetInfo* getInfo, XrActionStateBoolean* state)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrGetActionStateBoolean",
	    OXRTL_ARGS_XrSession(session, "session"),
	    OXRTL_ARGS_XrActionStateGetInfo_P(getInfo, "getInfo"));
	// XRTracing::TraceNext(localActivity, "xrGetActionStateBoolean_getInfo",
	// getInfo);
	const auto ret = next_xrGetActionStateBoolean(session, getInfo, state);
	// XRTracing::TraceNext(localActivity, "xrGetActionStateBoolean_state",
	// state);
	TraceLoggingWriteStop(localActivity, "xrGetActionStateBoolean",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_XrActionStateBoolean_P(state, "state"));

	return ret;
}

XrResult OXRTracing_xrGetActionStateFloat(XrSession session,
    const XrActionStateGetInfo* getInfo, XrActionStateFloat* state)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrGetActionStateFloat",
	    OXRTL_ARGS_XrSession(session, "session"),
	    OXRTL_ARGS_XrActionStateGetInfo_P(getInfo, "getInfo"));
	// XRTracing::TraceNext(localActivity, "xrGetActionStateFloat_getInfo",
	// getInfo);
	const auto ret = next_xrGetActionStateFloat(session, getInfo, state);
	// XRTracing::TraceNext(localActivity, "xrGetActionStateFloat_state",
	// state);
	TraceLoggingWriteStop(localActivity, "xrGetActionStateFloat",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_XrActionStateFloat_P(state, "state"));

	return ret;
}

XrResult OXRTracing_xrGetActionStateVector2f(XrSession session,
    const XrActionStateGetInfo* getInfo, XrActionStateVector2f* state)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrGetActionStateVector2f",
	    OXRTL_ARGS_XrSession(session, "session"),
	    OXRTL_ARGS_XrActionStateGetInfo_P(getInfo, "getInfo"));
	// XRTracing::TraceNext(localActivity, "xrGetActionStateVector2f_getInfo",
	// getInfo);
	const auto ret = next_xrGetActionStateVector2f(session, getInfo, state);
	// XRTracing::TraceNext(localActivity, "xrGetActionStateVector2f_state",
	// state);
	TraceLoggingWriteStop(localActivity, "xrGetActionStateVector2f",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_XrActionStateVector2f_P(state, "state"));

	return ret;
}

XrResult OXRTracing_xrGetActionStatePose(XrSession session,
    const XrActionStateGetInfo* getInfo, XrActionStatePose* state)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrGetActionStatePose",
	    OXRTL_ARGS_XrSession(session, "session"),
	    OXRTL_ARGS_XrActionStateGetInfo_P(getInfo, "getInfo"));
	// XRTracing::TraceNext(localActivity, "xrGetActionStatePose_getInfo",
	// getInfo);
	const auto ret = next_xrGetActionStatePose(session, getInfo, state);
	// XRTracing::TraceNext(localActivity, "xrGetActionStatePose_state", state);
	TraceLoggingWriteStop(localActivity, "xrGetActionStatePose",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_XrActionStatePose_P(state, "state"));

	return ret;
}

XrResult OXRTracing_xrSyncActions(
    XrSession session, const XrActionsSyncInfo* syncInfo)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrSyncActions",
	    OXRTL_ARGS_XrSession(session, "session"),
	    OXRTL_ARGS_XrActionsSyncInfo_P(syncInfo, "syncInfo"));
	// XRTracing::TraceNext(localActivity, "xrSyncActions_syncInfo", syncInfo);
	const auto ret = next_xrSyncActions(session, syncInfo);

	TraceLoggingWriteStop(
	    localActivity, "xrSyncActions", OXRTL_ARGS_XrResult(ret, "XrResult"));

	return ret;
}

XrResult OXRTracing_xrEnumerateBoundSourcesForAction(XrSession session,
    const XrBoundSourcesForActionEnumerateInfo* enumerateInfo,
    uint32_t sourceCapacityInput, uint32_t* sourceCountOutput, XrPath* sources)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrEnumerateBoundSourcesForAction",
	    OXRTL_ARGS_XrSession(session, "session"),
	    OXRTL_ARGS_XrBoundSourcesForActionEnumerateInfo_P(
	        enumerateInfo, "enumerateInfo"),
	    OXRTL_ARGS_uint32_t(sourceCapacityInput, "sourceCapacityInput"));
	// XRTracing::TraceNext(localActivity,
	// "xrEnumerateBoundSourcesForAction_enumerateInfo", enumerateInfo);
	const auto ret = next_xrEnumerateBoundSourcesForAction(session,
	    enumerateInfo, sourceCapacityInput, sourceCountOutput, sources);

	TraceLoggingWriteStop(localActivity, "xrEnumerateBoundSourcesForAction",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_uint32_t((*sourceCountOutput), "sourceCountOutput"));

	return ret;
}

XrResult OXRTracing_xrGetInputSourceLocalizedName(XrSession session,
    const XrInputSourceLocalizedNameGetInfo* getInfo,
    uint32_t bufferCapacityInput, uint32_t* bufferCountOutput, char* buffer)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrGetInputSourceLocalizedName",
	    OXRTL_ARGS_XrSession(session, "session"),
	    OXRTL_ARGS_XrInputSourceLocalizedNameGetInfo_P(getInfo, "getInfo"),
	    OXRTL_ARGS_uint32_t(bufferCapacityInput, "bufferCapacityInput"));
	// XRTracing::TraceNext(localActivity,
	// "xrGetInputSourceLocalizedName_getInfo", getInfo);
	const auto ret = next_xrGetInputSourceLocalizedName(
	    session, getInfo, bufferCapacityInput, bufferCountOutput, buffer);

	TraceLoggingWriteStop(localActivity, "xrGetInputSourceLocalizedName",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_uint32_t((*bufferCountOutput), "bufferCountOutput"));

	return ret;
}

XrResult OXRTracing_xrApplyHapticFeedback(XrSession session,
    const XrHapticActionInfo* hapticActionInfo,
    const XrHapticBaseHeader* hapticFeedback)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrApplyHapticFeedback",
	    OXRTL_ARGS_XrSession(session, "session"),
	    OXRTL_ARGS_XrHapticActionInfo_P(hapticActionInfo, "hapticActionInfo"),
	    OXRTL_ARGS_XrHapticBaseHeader_P(hapticFeedback, "hapticFeedback"));
	// XRTracing::TraceNext(localActivity,
	// "xrApplyHapticFeedback_hapticActionInfo", hapticActionInfo);
	// XRTracing::TraceNext(localActivity,
	// "xrApplyHapticFeedback_hapticFeedback", hapticFeedback);
	const auto ret
	    = next_xrApplyHapticFeedback(session, hapticActionInfo, hapticFeedback);

	TraceLoggingWriteStop(localActivity, "xrApplyHapticFeedback",
	    OXRTL_ARGS_XrResult(ret, "XrResult"));

	return ret;
}

XrResult OXRTracing_xrStopHapticFeedback(
    XrSession session, const XrHapticActionInfo* hapticActionInfo)
{

	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrStopHapticFeedback",
	    OXRTL_ARGS_XrSession(session, "session"),
	    OXRTL_ARGS_XrHapticActionInfo_P(hapticActionInfo, "hapticActionInfo"));
	// XRTracing::TraceNext(localActivity,
	// "xrStopHapticFeedback_hapticActionInfo", hapticActionInfo);
	const auto ret = next_xrStopHapticFeedback(session, hapticActionInfo);

	TraceLoggingWriteStop(localActivity, "xrStopHapticFeedback",
	    OXRTL_ARGS_XrResult(ret, "XrResult"));

	return ret;
}
