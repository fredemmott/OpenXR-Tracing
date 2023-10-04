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
#include <loader_interfaces.h>
#include <openxr/openxr.h>

#include <TraceLoggingActivity.h>
#include <TraceLoggingProvider.h>

namespace OXRTracing {

thread_local XrInstance gXrInstance{ XR_NULL_HANDLE };
PFN_xrGetInstanceProcAddr gXrNextGetInstanceProcAddr{ nullptr };
PFN_xrEnumerateInstanceExtensionProperties
    gXrNextEnumerateInstanceExtensionProperties{ nullptr };

} // namespace OXRTracing

XrResult XRAPI_CALL OXRTracing_xrGetInstanceProcAddr(
    XrInstance instance, const char* name, PFN_xrVoidFunction* function);

using namespace OXRTracing;

XrResult OXRTracing_xrEnumerateInstanceExtensionProperties(
    const char* layerName, uint32_t propertyCapacityInput,
    uint32_t* propertyCountOutput, XrExtensionProperties* properties)
{
	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity,
	    "xrEnumerateInstanceExtensionProperties",
	    TraceLoggingString(layerName, "layerName"),
	    TraceLoggingUInt32(propertyCapacityInput, "propertyCapacityInput"));

	// The OpenXR specification says that this should work without an instance,
	// however, a 'next' pointer is needed to query extensions that might be
	// provided by another API layer, and the OpenXR *Loader* spec says:
	//
	// "an implicit API layer, it must add it’s own instance extension
	// contents to the list of extensions."; this means we - and any lower
	// layers - need the 'next' pointer.

	if (layerName == NULL && gXrNextEnumerateInstanceExtensionProperties) {
		const auto ret = gXrNextEnumerateInstanceExtensionProperties(
		    layerName, propertyCapacityInput, propertyCountOutput, properties);

		if (XR_SUCCEEDED(ret)) {
			const auto propertyCount
			    = std::min(propertyCapacityInput, *propertyCountOutput);

			for (auto i = 0; i < propertyCount; ++i) {
				OXRTL_DUMP_XrExtensionProperties(localActivity,
				    "xrEnumerateInstanceExtensionProperties", "properties[]",
				    (properties[i]));
			}
		}

		TraceLoggingWriteStop(localActivity,
		    "xrEnumerateInstanceExtensionProperties",
		    TraceLoggingUInt32(*propertyCountOutput, "propertyCountOutput"),
		    OXRTL_ARGS_XrResult(ret, "XrResult"));
		return ret;
	}
	*propertyCountOutput = 0;
	TraceLoggingWriteStop(localActivity,
	    "xrEnumerateInstanceExtensionProperties",
	    TraceLoggingUInt32(0, "propertyCountOutput"),
	    OXRTL_ARGS_XrResult(XR_SUCCESS, "XrResult"));
	return XR_SUCCESS;
}

XrResult OXRTracing_xrEnumerateApiLayerProperties(
    uint32_t propertyCapacityInput, uint32_t* propertyCountOutput,
    XrApiLayerProperties* properties)
{
	*propertyCountOutput = 0;
	return XR_SUCCESS;
}

static XrResult XRAPI_CALL OXRTracing_xrCreateApiLayerInstance(
    const XrInstanceCreateInfo* info,
    const struct XrApiLayerCreateInfo* layerInfo, XrInstance* instance)
{
	TraceLoggingActivity<gTraceProvider> localActivity;
	TraceLoggingWriteStart(localActivity, "xrCreateApiLayerInstance",
	    OXRTL_ARGS_XrInstanceCreateInfo((*info), "info"));

	auto nextLayerCreateInfo = *layerInfo;
	nextLayerCreateInfo.nextInfo = layerInfo->nextInfo->next;

	const auto ret = layerInfo->nextInfo->nextCreateApiLayerInstance(
	    info, &nextLayerCreateInfo, instance);

	TraceLoggingWriteStop(localActivity, "xrCreateApiLayerInstance",
	    OXRTL_ARGS_XrResult(ret, "XrResult"),
	    OXRTL_ARGS_XrInstance((*instance), "instance"));

	if (XR_FAILED(ret)) {
		return ret;
	}

	gXrInstance = *instance;
	gXrNextGetInstanceProcAddr = layerInfo->nextInfo->nextGetInstanceProcAddr;
	gXrNextGetInstanceProcAddr(*instance,
	    "xrEnumerateInstanceExtensionProperties",
	    reinterpret_cast<PFN_xrVoidFunction*>(
	        &gXrNextEnumerateInstanceExtensionProperties));

	return ret;
}

BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		TraceLoggingRegister(gTraceProvider);
		break;
	case DLL_PROCESS_DETACH:
		TraceLoggingUnregister(gTraceProvider);
		break;
	}
	return TRUE;
}

extern "C" {
XrResult __declspec(dllexport) XRAPI_CALL
    OXRTracing_xrNegotiateLoaderApiLayerInterface(
        const XrNegotiateLoaderInfo* loaderInfo, const char* layerName,
        XrNegotiateApiLayerRequest* apiLayerRequest)
{

	// TODO: check version fields etc in loaderInfo

	apiLayerRequest->layerInterfaceVersion
	    = XR_CURRENT_LOADER_API_LAYER_VERSION;
	apiLayerRequest->layerApiVersion = XR_CURRENT_API_VERSION;
	apiLayerRequest->getInstanceProcAddr = &OXRTracing_xrGetInstanceProcAddr;
	apiLayerRequest->createApiLayerInstance
	    = &OXRTracing_xrCreateApiLayerInstance;
	return XR_SUCCESS;
}
}
