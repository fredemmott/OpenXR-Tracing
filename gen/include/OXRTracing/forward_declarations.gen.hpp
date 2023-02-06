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
#include <openxr/openxr.h>

namespace OXRTracing {

extern PFN_xrDestroyInstance next_xrDestroyInstance;
extern PFN_xrGetInstanceProperties next_xrGetInstanceProperties;
extern PFN_xrPollEvent next_xrPollEvent;
extern PFN_xrResultToString next_xrResultToString;
extern PFN_xrStructureTypeToString next_xrStructureTypeToString;
extern PFN_xrGetSystem next_xrGetSystem;
extern PFN_xrGetSystemProperties next_xrGetSystemProperties;
extern PFN_xrEnumerateEnvironmentBlendModes
    next_xrEnumerateEnvironmentBlendModes;
extern PFN_xrCreateSession next_xrCreateSession;
extern PFN_xrDestroySession next_xrDestroySession;
extern PFN_xrEnumerateReferenceSpaces next_xrEnumerateReferenceSpaces;
extern PFN_xrCreateReferenceSpace next_xrCreateReferenceSpace;
extern PFN_xrGetReferenceSpaceBoundsRect next_xrGetReferenceSpaceBoundsRect;
extern PFN_xrCreateActionSpace next_xrCreateActionSpace;
extern PFN_xrLocateSpace next_xrLocateSpace;
extern PFN_xrDestroySpace next_xrDestroySpace;
extern PFN_xrEnumerateViewConfigurations next_xrEnumerateViewConfigurations;
extern PFN_xrGetViewConfigurationProperties
    next_xrGetViewConfigurationProperties;
extern PFN_xrEnumerateViewConfigurationViews
    next_xrEnumerateViewConfigurationViews;
extern PFN_xrEnumerateSwapchainFormats next_xrEnumerateSwapchainFormats;
extern PFN_xrCreateSwapchain next_xrCreateSwapchain;
extern PFN_xrDestroySwapchain next_xrDestroySwapchain;
extern PFN_xrEnumerateSwapchainImages next_xrEnumerateSwapchainImages;
extern PFN_xrAcquireSwapchainImage next_xrAcquireSwapchainImage;
extern PFN_xrWaitSwapchainImage next_xrWaitSwapchainImage;
extern PFN_xrReleaseSwapchainImage next_xrReleaseSwapchainImage;
extern PFN_xrBeginSession next_xrBeginSession;
extern PFN_xrEndSession next_xrEndSession;
extern PFN_xrRequestExitSession next_xrRequestExitSession;
extern PFN_xrWaitFrame next_xrWaitFrame;
extern PFN_xrBeginFrame next_xrBeginFrame;
extern PFN_xrEndFrame next_xrEndFrame;
extern PFN_xrLocateViews next_xrLocateViews;
extern PFN_xrStringToPath next_xrStringToPath;
extern PFN_xrPathToString next_xrPathToString;
extern PFN_xrCreateActionSet next_xrCreateActionSet;
extern PFN_xrDestroyActionSet next_xrDestroyActionSet;
extern PFN_xrCreateAction next_xrCreateAction;
extern PFN_xrDestroyAction next_xrDestroyAction;
extern PFN_xrSuggestInteractionProfileBindings
    next_xrSuggestInteractionProfileBindings;
extern PFN_xrAttachSessionActionSets next_xrAttachSessionActionSets;
extern PFN_xrGetCurrentInteractionProfile next_xrGetCurrentInteractionProfile;
extern PFN_xrGetActionStateBoolean next_xrGetActionStateBoolean;
extern PFN_xrGetActionStateFloat next_xrGetActionStateFloat;
extern PFN_xrGetActionStateVector2f next_xrGetActionStateVector2f;
extern PFN_xrGetActionStatePose next_xrGetActionStatePose;
extern PFN_xrSyncActions next_xrSyncActions;
extern PFN_xrEnumerateBoundSourcesForAction
    next_xrEnumerateBoundSourcesForAction;
extern PFN_xrGetInputSourceLocalizedName next_xrGetInputSourceLocalizedName;
extern PFN_xrApplyHapticFeedback next_xrApplyHapticFeedback;
extern PFN_xrStopHapticFeedback next_xrStopHapticFeedback;

} // namespace OXRTracing
