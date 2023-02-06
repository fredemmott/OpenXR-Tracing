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
#include <openxr.h>

#include <TraceLoggingActivity.h>
#include <TraceLoggingProvider.h>

namespace OXRTracing {

thread_local XrInstance gXrInstance{};
PFN_xrGetInstanceProcAddr gXrNextGetInstanceProcAddr{ nullptr };

} // namespace OXRTracing

XrResult OXRTracing_xrGetInstanceProcAddr(
    XrInstance instance, const char* name, PFN_xrVoidFunction* function);

using namespace OXRTracing;

static XrResult OXRTracing_xrCreateApiLayerInstance(
    const XrInstanceCreateInfo* createInfo,
    const struct XrApiLayerCreateInfo* layerCreateInfo, XrInstance* instance)
{
	auto nextLayerCreateInfo = *layerCreateInfo;
	nextLayerCreateInfo.nextInfo = layerCreateInfo->nextInfo->next;

	const auto ret = layerCreateInfo->nextInfo->nextCreateApiLayerInstance(
	    createInfo, &nextLayerCreateInfo, instance);
	if (XR_FAILED(ret)) {
		return ret;
	}

	gXrNextGetInstanceProcAddr
	    = layerCreateInfo->nextInfo->nextGetInstanceProcAddr;

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
