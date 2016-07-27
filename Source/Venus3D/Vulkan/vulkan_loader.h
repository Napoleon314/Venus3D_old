////////////////////////////////////////////////////////////////////////////
//  
//  Copyright (c) 2015-2016 The Khronos Group Inc.
//  
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and/or associated documentation files (the
//  "Materials"), to deal in the Materials without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Materials, and to
//  permit persons to whom the Materials are furnished to do so, subject to
//  the following conditions:
//  
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Materials.
//  
//  THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
//  
//  ------------------------------------------------------------------------
//  
//  This file, vk.xml, is the Vulkan API Registry. It is a critically important
//  and normative part of the Vulkan Specification, including a canonical
//  machine-readable definition of the API, parameter and member validation
//  language incorporated into the Specification and reference pages, and other
//  material which is registered by Khronos, such as tags used by extension and
//  layer authors. The only authoritative version of vk.xml is the one
//  maintained in the master branch of the Khronos Vulkan Github project.
//      
//  ------------------------------------------------------------------------
//
//  Vulkan loader is generated for dynamically loading function of vulkan on
//  Windows and Linux.
//  This generator is written by Albert D Yang.
//
////////////////////////////////////////////////////////////////////////////

#ifndef VULKAN_LOADER_H_
#define VULKAN_LOADER_H_ 1

#include "vulkan.h"

extern PFN_vkCreateInstance _vkCreateInstance;
#define vkCreateInstance _vkCreateInstance

extern PFN_vkDestroyInstance _vkDestroyInstance;
#define vkDestroyInstance _vkDestroyInstance

extern PFN_vkEnumeratePhysicalDevices _vkEnumeratePhysicalDevices;
#define vkEnumeratePhysicalDevices _vkEnumeratePhysicalDevices

extern PFN_vkGetDeviceProcAddr _vkGetDeviceProcAddr;
#define vkGetDeviceProcAddr _vkGetDeviceProcAddr

extern PFN_vkGetInstanceProcAddr _vkGetInstanceProcAddr;
#define vkGetInstanceProcAddr _vkGetInstanceProcAddr

extern PFN_vkGetPhysicalDeviceProperties _vkGetPhysicalDeviceProperties;
#define vkGetPhysicalDeviceProperties _vkGetPhysicalDeviceProperties

extern PFN_vkGetPhysicalDeviceQueueFamilyProperties _vkGetPhysicalDeviceQueueFamilyProperties;
#define vkGetPhysicalDeviceQueueFamilyProperties _vkGetPhysicalDeviceQueueFamilyProperties

extern PFN_vkGetPhysicalDeviceMemoryProperties _vkGetPhysicalDeviceMemoryProperties;
#define vkGetPhysicalDeviceMemoryProperties _vkGetPhysicalDeviceMemoryProperties

extern PFN_vkGetPhysicalDeviceFeatures _vkGetPhysicalDeviceFeatures;
#define vkGetPhysicalDeviceFeatures _vkGetPhysicalDeviceFeatures

extern PFN_vkGetPhysicalDeviceFormatProperties _vkGetPhysicalDeviceFormatProperties;
#define vkGetPhysicalDeviceFormatProperties _vkGetPhysicalDeviceFormatProperties

extern PFN_vkGetPhysicalDeviceImageFormatProperties _vkGetPhysicalDeviceImageFormatProperties;
#define vkGetPhysicalDeviceImageFormatProperties _vkGetPhysicalDeviceImageFormatProperties

extern PFN_vkCreateDevice _vkCreateDevice;
#define vkCreateDevice _vkCreateDevice

extern PFN_vkDestroyDevice _vkDestroyDevice;
#define vkDestroyDevice _vkDestroyDevice

extern PFN_vkEnumerateInstanceLayerProperties _vkEnumerateInstanceLayerProperties;
#define vkEnumerateInstanceLayerProperties _vkEnumerateInstanceLayerProperties

extern PFN_vkEnumerateInstanceExtensionProperties _vkEnumerateInstanceExtensionProperties;
#define vkEnumerateInstanceExtensionProperties _vkEnumerateInstanceExtensionProperties

extern PFN_vkEnumerateDeviceLayerProperties _vkEnumerateDeviceLayerProperties;
#define vkEnumerateDeviceLayerProperties _vkEnumerateDeviceLayerProperties

extern PFN_vkEnumerateDeviceExtensionProperties _vkEnumerateDeviceExtensionProperties;
#define vkEnumerateDeviceExtensionProperties _vkEnumerateDeviceExtensionProperties

extern PFN_vkGetDeviceQueue _vkGetDeviceQueue;
#define vkGetDeviceQueue _vkGetDeviceQueue

extern PFN_vkQueueSubmit _vkQueueSubmit;
#define vkQueueSubmit _vkQueueSubmit

extern PFN_vkQueueWaitIdle _vkQueueWaitIdle;
#define vkQueueWaitIdle _vkQueueWaitIdle

extern PFN_vkDeviceWaitIdle _vkDeviceWaitIdle;
#define vkDeviceWaitIdle _vkDeviceWaitIdle

extern PFN_vkAllocateMemory _vkAllocateMemory;
#define vkAllocateMemory _vkAllocateMemory

extern PFN_vkFreeMemory _vkFreeMemory;
#define vkFreeMemory _vkFreeMemory

extern PFN_vkMapMemory _vkMapMemory;
#define vkMapMemory _vkMapMemory

extern PFN_vkUnmapMemory _vkUnmapMemory;
#define vkUnmapMemory _vkUnmapMemory

extern PFN_vkFlushMappedMemoryRanges _vkFlushMappedMemoryRanges;
#define vkFlushMappedMemoryRanges _vkFlushMappedMemoryRanges

extern PFN_vkInvalidateMappedMemoryRanges _vkInvalidateMappedMemoryRanges;
#define vkInvalidateMappedMemoryRanges _vkInvalidateMappedMemoryRanges

extern PFN_vkGetDeviceMemoryCommitment _vkGetDeviceMemoryCommitment;
#define vkGetDeviceMemoryCommitment _vkGetDeviceMemoryCommitment

extern PFN_vkGetBufferMemoryRequirements _vkGetBufferMemoryRequirements;
#define vkGetBufferMemoryRequirements _vkGetBufferMemoryRequirements

extern PFN_vkBindBufferMemory _vkBindBufferMemory;
#define vkBindBufferMemory _vkBindBufferMemory

extern PFN_vkGetImageMemoryRequirements _vkGetImageMemoryRequirements;
#define vkGetImageMemoryRequirements _vkGetImageMemoryRequirements

extern PFN_vkBindImageMemory _vkBindImageMemory;
#define vkBindImageMemory _vkBindImageMemory

extern PFN_vkGetImageSparseMemoryRequirements _vkGetImageSparseMemoryRequirements;
#define vkGetImageSparseMemoryRequirements _vkGetImageSparseMemoryRequirements

extern PFN_vkGetPhysicalDeviceSparseImageFormatProperties _vkGetPhysicalDeviceSparseImageFormatProperties;
#define vkGetPhysicalDeviceSparseImageFormatProperties _vkGetPhysicalDeviceSparseImageFormatProperties

extern PFN_vkQueueBindSparse _vkQueueBindSparse;
#define vkQueueBindSparse _vkQueueBindSparse

extern PFN_vkCreateFence _vkCreateFence;
#define vkCreateFence _vkCreateFence

extern PFN_vkDestroyFence _vkDestroyFence;
#define vkDestroyFence _vkDestroyFence

extern PFN_vkResetFences _vkResetFences;
#define vkResetFences _vkResetFences

extern PFN_vkGetFenceStatus _vkGetFenceStatus;
#define vkGetFenceStatus _vkGetFenceStatus

extern PFN_vkWaitForFences _vkWaitForFences;
#define vkWaitForFences _vkWaitForFences

extern PFN_vkCreateSemaphore _vkCreateSemaphore;
#define vkCreateSemaphore _vkCreateSemaphore

extern PFN_vkDestroySemaphore _vkDestroySemaphore;
#define vkDestroySemaphore _vkDestroySemaphore

extern PFN_vkCreateEvent _vkCreateEvent;
#define vkCreateEvent _vkCreateEvent

extern PFN_vkDestroyEvent _vkDestroyEvent;
#define vkDestroyEvent _vkDestroyEvent

extern PFN_vkGetEventStatus _vkGetEventStatus;
#define vkGetEventStatus _vkGetEventStatus

extern PFN_vkSetEvent _vkSetEvent;
#define vkSetEvent _vkSetEvent

extern PFN_vkResetEvent _vkResetEvent;
#define vkResetEvent _vkResetEvent

extern PFN_vkCreateQueryPool _vkCreateQueryPool;
#define vkCreateQueryPool _vkCreateQueryPool

extern PFN_vkDestroyQueryPool _vkDestroyQueryPool;
#define vkDestroyQueryPool _vkDestroyQueryPool

extern PFN_vkGetQueryPoolResults _vkGetQueryPoolResults;
#define vkGetQueryPoolResults _vkGetQueryPoolResults

extern PFN_vkCreateBuffer _vkCreateBuffer;
#define vkCreateBuffer _vkCreateBuffer

extern PFN_vkDestroyBuffer _vkDestroyBuffer;
#define vkDestroyBuffer _vkDestroyBuffer

extern PFN_vkCreateBufferView _vkCreateBufferView;
#define vkCreateBufferView _vkCreateBufferView

extern PFN_vkDestroyBufferView _vkDestroyBufferView;
#define vkDestroyBufferView _vkDestroyBufferView

extern PFN_vkCreateImage _vkCreateImage;
#define vkCreateImage _vkCreateImage

extern PFN_vkDestroyImage _vkDestroyImage;
#define vkDestroyImage _vkDestroyImage

extern PFN_vkGetImageSubresourceLayout _vkGetImageSubresourceLayout;
#define vkGetImageSubresourceLayout _vkGetImageSubresourceLayout

extern PFN_vkCreateImageView _vkCreateImageView;
#define vkCreateImageView _vkCreateImageView

extern PFN_vkDestroyImageView _vkDestroyImageView;
#define vkDestroyImageView _vkDestroyImageView

extern PFN_vkCreateShaderModule _vkCreateShaderModule;
#define vkCreateShaderModule _vkCreateShaderModule

extern PFN_vkDestroyShaderModule _vkDestroyShaderModule;
#define vkDestroyShaderModule _vkDestroyShaderModule

extern PFN_vkCreatePipelineCache _vkCreatePipelineCache;
#define vkCreatePipelineCache _vkCreatePipelineCache

extern PFN_vkDestroyPipelineCache _vkDestroyPipelineCache;
#define vkDestroyPipelineCache _vkDestroyPipelineCache

extern PFN_vkGetPipelineCacheData _vkGetPipelineCacheData;
#define vkGetPipelineCacheData _vkGetPipelineCacheData

extern PFN_vkMergePipelineCaches _vkMergePipelineCaches;
#define vkMergePipelineCaches _vkMergePipelineCaches

extern PFN_vkCreateGraphicsPipelines _vkCreateGraphicsPipelines;
#define vkCreateGraphicsPipelines _vkCreateGraphicsPipelines

extern PFN_vkCreateComputePipelines _vkCreateComputePipelines;
#define vkCreateComputePipelines _vkCreateComputePipelines

extern PFN_vkDestroyPipeline _vkDestroyPipeline;
#define vkDestroyPipeline _vkDestroyPipeline

extern PFN_vkCreatePipelineLayout _vkCreatePipelineLayout;
#define vkCreatePipelineLayout _vkCreatePipelineLayout

extern PFN_vkDestroyPipelineLayout _vkDestroyPipelineLayout;
#define vkDestroyPipelineLayout _vkDestroyPipelineLayout

extern PFN_vkCreateSampler _vkCreateSampler;
#define vkCreateSampler _vkCreateSampler

extern PFN_vkDestroySampler _vkDestroySampler;
#define vkDestroySampler _vkDestroySampler

extern PFN_vkCreateDescriptorSetLayout _vkCreateDescriptorSetLayout;
#define vkCreateDescriptorSetLayout _vkCreateDescriptorSetLayout

extern PFN_vkDestroyDescriptorSetLayout _vkDestroyDescriptorSetLayout;
#define vkDestroyDescriptorSetLayout _vkDestroyDescriptorSetLayout

extern PFN_vkCreateDescriptorPool _vkCreateDescriptorPool;
#define vkCreateDescriptorPool _vkCreateDescriptorPool

extern PFN_vkDestroyDescriptorPool _vkDestroyDescriptorPool;
#define vkDestroyDescriptorPool _vkDestroyDescriptorPool

extern PFN_vkResetDescriptorPool _vkResetDescriptorPool;
#define vkResetDescriptorPool _vkResetDescriptorPool

extern PFN_vkAllocateDescriptorSets _vkAllocateDescriptorSets;
#define vkAllocateDescriptorSets _vkAllocateDescriptorSets

extern PFN_vkFreeDescriptorSets _vkFreeDescriptorSets;
#define vkFreeDescriptorSets _vkFreeDescriptorSets

extern PFN_vkUpdateDescriptorSets _vkUpdateDescriptorSets;
#define vkUpdateDescriptorSets _vkUpdateDescriptorSets

extern PFN_vkCreateFramebuffer _vkCreateFramebuffer;
#define vkCreateFramebuffer _vkCreateFramebuffer

extern PFN_vkDestroyFramebuffer _vkDestroyFramebuffer;
#define vkDestroyFramebuffer _vkDestroyFramebuffer

extern PFN_vkCreateRenderPass _vkCreateRenderPass;
#define vkCreateRenderPass _vkCreateRenderPass

extern PFN_vkDestroyRenderPass _vkDestroyRenderPass;
#define vkDestroyRenderPass _vkDestroyRenderPass

extern PFN_vkGetRenderAreaGranularity _vkGetRenderAreaGranularity;
#define vkGetRenderAreaGranularity _vkGetRenderAreaGranularity

extern PFN_vkCreateCommandPool _vkCreateCommandPool;
#define vkCreateCommandPool _vkCreateCommandPool

extern PFN_vkDestroyCommandPool _vkDestroyCommandPool;
#define vkDestroyCommandPool _vkDestroyCommandPool

extern PFN_vkResetCommandPool _vkResetCommandPool;
#define vkResetCommandPool _vkResetCommandPool

extern PFN_vkAllocateCommandBuffers _vkAllocateCommandBuffers;
#define vkAllocateCommandBuffers _vkAllocateCommandBuffers

extern PFN_vkFreeCommandBuffers _vkFreeCommandBuffers;
#define vkFreeCommandBuffers _vkFreeCommandBuffers

extern PFN_vkBeginCommandBuffer _vkBeginCommandBuffer;
#define vkBeginCommandBuffer _vkBeginCommandBuffer

extern PFN_vkEndCommandBuffer _vkEndCommandBuffer;
#define vkEndCommandBuffer _vkEndCommandBuffer

extern PFN_vkResetCommandBuffer _vkResetCommandBuffer;
#define vkResetCommandBuffer _vkResetCommandBuffer

extern PFN_vkCmdBindPipeline _vkCmdBindPipeline;
#define vkCmdBindPipeline _vkCmdBindPipeline

extern PFN_vkCmdSetViewport _vkCmdSetViewport;
#define vkCmdSetViewport _vkCmdSetViewport

extern PFN_vkCmdSetScissor _vkCmdSetScissor;
#define vkCmdSetScissor _vkCmdSetScissor

extern PFN_vkCmdSetLineWidth _vkCmdSetLineWidth;
#define vkCmdSetLineWidth _vkCmdSetLineWidth

extern PFN_vkCmdSetDepthBias _vkCmdSetDepthBias;
#define vkCmdSetDepthBias _vkCmdSetDepthBias

extern PFN_vkCmdSetBlendConstants _vkCmdSetBlendConstants;
#define vkCmdSetBlendConstants _vkCmdSetBlendConstants

extern PFN_vkCmdSetDepthBounds _vkCmdSetDepthBounds;
#define vkCmdSetDepthBounds _vkCmdSetDepthBounds

extern PFN_vkCmdSetStencilCompareMask _vkCmdSetStencilCompareMask;
#define vkCmdSetStencilCompareMask _vkCmdSetStencilCompareMask

extern PFN_vkCmdSetStencilWriteMask _vkCmdSetStencilWriteMask;
#define vkCmdSetStencilWriteMask _vkCmdSetStencilWriteMask

extern PFN_vkCmdSetStencilReference _vkCmdSetStencilReference;
#define vkCmdSetStencilReference _vkCmdSetStencilReference

extern PFN_vkCmdBindDescriptorSets _vkCmdBindDescriptorSets;
#define vkCmdBindDescriptorSets _vkCmdBindDescriptorSets

extern PFN_vkCmdBindIndexBuffer _vkCmdBindIndexBuffer;
#define vkCmdBindIndexBuffer _vkCmdBindIndexBuffer

extern PFN_vkCmdBindVertexBuffers _vkCmdBindVertexBuffers;
#define vkCmdBindVertexBuffers _vkCmdBindVertexBuffers

extern PFN_vkCmdDraw _vkCmdDraw;
#define vkCmdDraw _vkCmdDraw

extern PFN_vkCmdDrawIndexed _vkCmdDrawIndexed;
#define vkCmdDrawIndexed _vkCmdDrawIndexed

extern PFN_vkCmdDrawIndirect _vkCmdDrawIndirect;
#define vkCmdDrawIndirect _vkCmdDrawIndirect

extern PFN_vkCmdDrawIndexedIndirect _vkCmdDrawIndexedIndirect;
#define vkCmdDrawIndexedIndirect _vkCmdDrawIndexedIndirect

extern PFN_vkCmdDispatch _vkCmdDispatch;
#define vkCmdDispatch _vkCmdDispatch

extern PFN_vkCmdDispatchIndirect _vkCmdDispatchIndirect;
#define vkCmdDispatchIndirect _vkCmdDispatchIndirect

extern PFN_vkCmdCopyBuffer _vkCmdCopyBuffer;
#define vkCmdCopyBuffer _vkCmdCopyBuffer

extern PFN_vkCmdCopyImage _vkCmdCopyImage;
#define vkCmdCopyImage _vkCmdCopyImage

extern PFN_vkCmdBlitImage _vkCmdBlitImage;
#define vkCmdBlitImage _vkCmdBlitImage

extern PFN_vkCmdCopyBufferToImage _vkCmdCopyBufferToImage;
#define vkCmdCopyBufferToImage _vkCmdCopyBufferToImage

extern PFN_vkCmdCopyImageToBuffer _vkCmdCopyImageToBuffer;
#define vkCmdCopyImageToBuffer _vkCmdCopyImageToBuffer

extern PFN_vkCmdUpdateBuffer _vkCmdUpdateBuffer;
#define vkCmdUpdateBuffer _vkCmdUpdateBuffer

extern PFN_vkCmdFillBuffer _vkCmdFillBuffer;
#define vkCmdFillBuffer _vkCmdFillBuffer

extern PFN_vkCmdClearColorImage _vkCmdClearColorImage;
#define vkCmdClearColorImage _vkCmdClearColorImage

extern PFN_vkCmdClearDepthStencilImage _vkCmdClearDepthStencilImage;
#define vkCmdClearDepthStencilImage _vkCmdClearDepthStencilImage

extern PFN_vkCmdClearAttachments _vkCmdClearAttachments;
#define vkCmdClearAttachments _vkCmdClearAttachments

extern PFN_vkCmdResolveImage _vkCmdResolveImage;
#define vkCmdResolveImage _vkCmdResolveImage

extern PFN_vkCmdSetEvent _vkCmdSetEvent;
#define vkCmdSetEvent _vkCmdSetEvent

extern PFN_vkCmdResetEvent _vkCmdResetEvent;
#define vkCmdResetEvent _vkCmdResetEvent

extern PFN_vkCmdWaitEvents _vkCmdWaitEvents;
#define vkCmdWaitEvents _vkCmdWaitEvents

extern PFN_vkCmdPipelineBarrier _vkCmdPipelineBarrier;
#define vkCmdPipelineBarrier _vkCmdPipelineBarrier

extern PFN_vkCmdBeginQuery _vkCmdBeginQuery;
#define vkCmdBeginQuery _vkCmdBeginQuery

extern PFN_vkCmdEndQuery _vkCmdEndQuery;
#define vkCmdEndQuery _vkCmdEndQuery

extern PFN_vkCmdResetQueryPool _vkCmdResetQueryPool;
#define vkCmdResetQueryPool _vkCmdResetQueryPool

extern PFN_vkCmdWriteTimestamp _vkCmdWriteTimestamp;
#define vkCmdWriteTimestamp _vkCmdWriteTimestamp

extern PFN_vkCmdCopyQueryPoolResults _vkCmdCopyQueryPoolResults;
#define vkCmdCopyQueryPoolResults _vkCmdCopyQueryPoolResults

extern PFN_vkCmdPushConstants _vkCmdPushConstants;
#define vkCmdPushConstants _vkCmdPushConstants

extern PFN_vkCmdBeginRenderPass _vkCmdBeginRenderPass;
#define vkCmdBeginRenderPass _vkCmdBeginRenderPass

extern PFN_vkCmdNextSubpass _vkCmdNextSubpass;
#define vkCmdNextSubpass _vkCmdNextSubpass

extern PFN_vkCmdEndRenderPass _vkCmdEndRenderPass;
#define vkCmdEndRenderPass _vkCmdEndRenderPass

extern PFN_vkCmdExecuteCommands _vkCmdExecuteCommands;
#define vkCmdExecuteCommands _vkCmdExecuteCommands

/* VK_KHR_surface */
extern PFN_vkDestroySurfaceKHR _vkDestroySurfaceKHR;
#define vkDestroySurfaceKHR _vkDestroySurfaceKHR

extern PFN_vkGetPhysicalDeviceSurfaceSupportKHR _vkGetPhysicalDeviceSurfaceSupportKHR;
#define vkGetPhysicalDeviceSurfaceSupportKHR _vkGetPhysicalDeviceSurfaceSupportKHR

extern PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR _vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
#define vkGetPhysicalDeviceSurfaceCapabilitiesKHR _vkGetPhysicalDeviceSurfaceCapabilitiesKHR

extern PFN_vkGetPhysicalDeviceSurfaceFormatsKHR _vkGetPhysicalDeviceSurfaceFormatsKHR;
#define vkGetPhysicalDeviceSurfaceFormatsKHR _vkGetPhysicalDeviceSurfaceFormatsKHR

extern PFN_vkGetPhysicalDeviceSurfacePresentModesKHR _vkGetPhysicalDeviceSurfacePresentModesKHR;
#define vkGetPhysicalDeviceSurfacePresentModesKHR _vkGetPhysicalDeviceSurfacePresentModesKHR

/* VK_KHR_swapchain */
extern PFN_vkCreateSwapchainKHR _vkCreateSwapchainKHR;
#define vkCreateSwapchainKHR _vkCreateSwapchainKHR

extern PFN_vkDestroySwapchainKHR _vkDestroySwapchainKHR;
#define vkDestroySwapchainKHR _vkDestroySwapchainKHR

extern PFN_vkGetSwapchainImagesKHR _vkGetSwapchainImagesKHR;
#define vkGetSwapchainImagesKHR _vkGetSwapchainImagesKHR

extern PFN_vkAcquireNextImageKHR _vkAcquireNextImageKHR;
#define vkAcquireNextImageKHR _vkAcquireNextImageKHR

extern PFN_vkQueuePresentKHR _vkQueuePresentKHR;
#define vkQueuePresentKHR _vkQueuePresentKHR

/* VK_KHR_display */
extern PFN_vkGetPhysicalDeviceDisplayPropertiesKHR _vkGetPhysicalDeviceDisplayPropertiesKHR;
#define vkGetPhysicalDeviceDisplayPropertiesKHR _vkGetPhysicalDeviceDisplayPropertiesKHR

extern PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR _vkGetPhysicalDeviceDisplayPlanePropertiesKHR;
#define vkGetPhysicalDeviceDisplayPlanePropertiesKHR _vkGetPhysicalDeviceDisplayPlanePropertiesKHR

extern PFN_vkGetDisplayPlaneSupportedDisplaysKHR _vkGetDisplayPlaneSupportedDisplaysKHR;
#define vkGetDisplayPlaneSupportedDisplaysKHR _vkGetDisplayPlaneSupportedDisplaysKHR

extern PFN_vkGetDisplayModePropertiesKHR _vkGetDisplayModePropertiesKHR;
#define vkGetDisplayModePropertiesKHR _vkGetDisplayModePropertiesKHR

extern PFN_vkCreateDisplayModeKHR _vkCreateDisplayModeKHR;
#define vkCreateDisplayModeKHR _vkCreateDisplayModeKHR

extern PFN_vkGetDisplayPlaneCapabilitiesKHR _vkGetDisplayPlaneCapabilitiesKHR;
#define vkGetDisplayPlaneCapabilitiesKHR _vkGetDisplayPlaneCapabilitiesKHR

extern PFN_vkCreateDisplayPlaneSurfaceKHR _vkCreateDisplayPlaneSurfaceKHR;
#define vkCreateDisplayPlaneSurfaceKHR _vkCreateDisplayPlaneSurfaceKHR

/* VK_KHR_display_swapchain */
extern PFN_vkCreateSharedSwapchainsKHR _vkCreateSharedSwapchainsKHR;
#define vkCreateSharedSwapchainsKHR _vkCreateSharedSwapchainsKHR

/* VK_KHR_xlib_surface */
#ifdef VK_USE_PLATFORM_XLIB_KHR

extern PFN_vkCreateXlibSurfaceKHR _vkCreateXlibSurfaceKHR;
#define vkCreateXlibSurfaceKHR _vkCreateXlibSurfaceKHR

extern PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR _vkGetPhysicalDeviceXlibPresentationSupportKHR;
#define vkGetPhysicalDeviceXlibPresentationSupportKHR _vkGetPhysicalDeviceXlibPresentationSupportKHR

#endif 

/* VK_KHR_xcb_surface */
#ifdef VK_USE_PLATFORM_XCB_KHR

extern PFN_vkCreateXcbSurfaceKHR _vkCreateXcbSurfaceKHR;
#define vkCreateXcbSurfaceKHR _vkCreateXcbSurfaceKHR

extern PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR _vkGetPhysicalDeviceXcbPresentationSupportKHR;
#define vkGetPhysicalDeviceXcbPresentationSupportKHR _vkGetPhysicalDeviceXcbPresentationSupportKHR

#endif 

/* VK_KHR_wayland_surface */
#ifdef VK_USE_PLATFORM_WAYLAND_KHR

extern PFN_vkCreateWaylandSurfaceKHR _vkCreateWaylandSurfaceKHR;
#define vkCreateWaylandSurfaceKHR _vkCreateWaylandSurfaceKHR

extern PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR _vkGetPhysicalDeviceWaylandPresentationSupportKHR;
#define vkGetPhysicalDeviceWaylandPresentationSupportKHR _vkGetPhysicalDeviceWaylandPresentationSupportKHR

#endif 

/* VK_KHR_mir_surface */
#ifdef VK_USE_PLATFORM_MIR_KHR

extern PFN_vkCreateMirSurfaceKHR _vkCreateMirSurfaceKHR;
#define vkCreateMirSurfaceKHR _vkCreateMirSurfaceKHR

extern PFN_vkGetPhysicalDeviceMirPresentationSupportKHR _vkGetPhysicalDeviceMirPresentationSupportKHR;
#define vkGetPhysicalDeviceMirPresentationSupportKHR _vkGetPhysicalDeviceMirPresentationSupportKHR

#endif 

/* VK_KHR_android_surface */
#ifdef VK_USE_PLATFORM_ANDROID_KHR

extern PFN_vkCreateAndroidSurfaceKHR _vkCreateAndroidSurfaceKHR;
#define vkCreateAndroidSurfaceKHR _vkCreateAndroidSurfaceKHR

#endif 

/* VK_KHR_win32_surface */
#ifdef VK_USE_PLATFORM_WIN32_KHR

extern PFN_vkCreateWin32SurfaceKHR _vkCreateWin32SurfaceKHR;
#define vkCreateWin32SurfaceKHR _vkCreateWin32SurfaceKHR

extern PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR _vkGetPhysicalDeviceWin32PresentationSupportKHR;
#define vkGetPhysicalDeviceWin32PresentationSupportKHR _vkGetPhysicalDeviceWin32PresentationSupportKHR

#endif 

/* VK_EXT_debug_report */
extern PFN_vkCreateDebugReportCallbackEXT _vkCreateDebugReportCallbackEXT;
#define vkCreateDebugReportCallbackEXT _vkCreateDebugReportCallbackEXT

extern PFN_vkDestroyDebugReportCallbackEXT _vkDestroyDebugReportCallbackEXT;
#define vkDestroyDebugReportCallbackEXT _vkDestroyDebugReportCallbackEXT

extern PFN_vkDebugReportMessageEXT _vkDebugReportMessageEXT;
#define vkDebugReportMessageEXT _vkDebugReportMessageEXT

/* VK_EXT_debug_marker */
extern PFN_vkDebugMarkerSetObjectTagEXT _vkDebugMarkerSetObjectTagEXT;
#define vkDebugMarkerSetObjectTagEXT _vkDebugMarkerSetObjectTagEXT

extern PFN_vkDebugMarkerSetObjectNameEXT _vkDebugMarkerSetObjectNameEXT;
#define vkDebugMarkerSetObjectNameEXT _vkDebugMarkerSetObjectNameEXT

extern PFN_vkCmdDebugMarkerBeginEXT _vkCmdDebugMarkerBeginEXT;
#define vkCmdDebugMarkerBeginEXT _vkCmdDebugMarkerBeginEXT

extern PFN_vkCmdDebugMarkerEndEXT _vkCmdDebugMarkerEndEXT;
#define vkCmdDebugMarkerEndEXT _vkCmdDebugMarkerEndEXT

extern PFN_vkCmdDebugMarkerInsertEXT _vkCmdDebugMarkerInsertEXT;
#define vkCmdDebugMarkerInsertEXT _vkCmdDebugMarkerInsertEXT

#define VK_LOADER_GLOBAL_FUNC \
	PFN_vkCreateInstance _vkCreateInstance = NULL; \
	PFN_vkDestroyInstance _vkDestroyInstance = NULL; \
	PFN_vkEnumeratePhysicalDevices _vkEnumeratePhysicalDevices = NULL; \
	PFN_vkGetDeviceProcAddr _vkGetDeviceProcAddr = NULL; \
	PFN_vkGetInstanceProcAddr _vkGetInstanceProcAddr = NULL; \
	PFN_vkGetPhysicalDeviceProperties _vkGetPhysicalDeviceProperties = NULL; \
	PFN_vkGetPhysicalDeviceQueueFamilyProperties _vkGetPhysicalDeviceQueueFamilyProperties = NULL; \
	PFN_vkGetPhysicalDeviceMemoryProperties _vkGetPhysicalDeviceMemoryProperties = NULL; \
	PFN_vkGetPhysicalDeviceFeatures _vkGetPhysicalDeviceFeatures = NULL; \
	PFN_vkGetPhysicalDeviceFormatProperties _vkGetPhysicalDeviceFormatProperties = NULL; \
	PFN_vkGetPhysicalDeviceImageFormatProperties _vkGetPhysicalDeviceImageFormatProperties = NULL; \
	PFN_vkCreateDevice _vkCreateDevice = NULL; \
	PFN_vkDestroyDevice _vkDestroyDevice = NULL; \
	PFN_vkEnumerateInstanceLayerProperties _vkEnumerateInstanceLayerProperties = NULL; \
	PFN_vkEnumerateInstanceExtensionProperties _vkEnumerateInstanceExtensionProperties = NULL; \
	PFN_vkEnumerateDeviceLayerProperties _vkEnumerateDeviceLayerProperties = NULL; \
	PFN_vkEnumerateDeviceExtensionProperties _vkEnumerateDeviceExtensionProperties = NULL; \
	PFN_vkGetDeviceQueue _vkGetDeviceQueue = NULL; \
	PFN_vkQueueSubmit _vkQueueSubmit = NULL; \
	PFN_vkQueueWaitIdle _vkQueueWaitIdle = NULL; \
	PFN_vkDeviceWaitIdle _vkDeviceWaitIdle = NULL; \
	PFN_vkAllocateMemory _vkAllocateMemory = NULL; \
	PFN_vkFreeMemory _vkFreeMemory = NULL; \
	PFN_vkMapMemory _vkMapMemory = NULL; \
	PFN_vkUnmapMemory _vkUnmapMemory = NULL; \
	PFN_vkFlushMappedMemoryRanges _vkFlushMappedMemoryRanges = NULL; \
	PFN_vkInvalidateMappedMemoryRanges _vkInvalidateMappedMemoryRanges = NULL; \
	PFN_vkGetDeviceMemoryCommitment _vkGetDeviceMemoryCommitment = NULL; \
	PFN_vkGetBufferMemoryRequirements _vkGetBufferMemoryRequirements = NULL; \
	PFN_vkBindBufferMemory _vkBindBufferMemory = NULL; \
	PFN_vkGetImageMemoryRequirements _vkGetImageMemoryRequirements = NULL; \
	PFN_vkBindImageMemory _vkBindImageMemory = NULL; \
	PFN_vkGetImageSparseMemoryRequirements _vkGetImageSparseMemoryRequirements = NULL; \
	PFN_vkGetPhysicalDeviceSparseImageFormatProperties _vkGetPhysicalDeviceSparseImageFormatProperties = NULL; \
	PFN_vkQueueBindSparse _vkQueueBindSparse = NULL; \
	PFN_vkCreateFence _vkCreateFence = NULL; \
	PFN_vkDestroyFence _vkDestroyFence = NULL; \
	PFN_vkResetFences _vkResetFences = NULL; \
	PFN_vkGetFenceStatus _vkGetFenceStatus = NULL; \
	PFN_vkWaitForFences _vkWaitForFences = NULL; \
	PFN_vkCreateSemaphore _vkCreateSemaphore = NULL; \
	PFN_vkDestroySemaphore _vkDestroySemaphore = NULL; \
	PFN_vkCreateEvent _vkCreateEvent = NULL; \
	PFN_vkDestroyEvent _vkDestroyEvent = NULL; \
	PFN_vkGetEventStatus _vkGetEventStatus = NULL; \
	PFN_vkSetEvent _vkSetEvent = NULL; \
	PFN_vkResetEvent _vkResetEvent = NULL; \
	PFN_vkCreateQueryPool _vkCreateQueryPool = NULL; \
	PFN_vkDestroyQueryPool _vkDestroyQueryPool = NULL; \
	PFN_vkGetQueryPoolResults _vkGetQueryPoolResults = NULL; \
	PFN_vkCreateBuffer _vkCreateBuffer = NULL; \
	PFN_vkDestroyBuffer _vkDestroyBuffer = NULL; \
	PFN_vkCreateBufferView _vkCreateBufferView = NULL; \
	PFN_vkDestroyBufferView _vkDestroyBufferView = NULL; \
	PFN_vkCreateImage _vkCreateImage = NULL; \
	PFN_vkDestroyImage _vkDestroyImage = NULL; \
	PFN_vkGetImageSubresourceLayout _vkGetImageSubresourceLayout = NULL; \
	PFN_vkCreateImageView _vkCreateImageView = NULL; \
	PFN_vkDestroyImageView _vkDestroyImageView = NULL; \
	PFN_vkCreateShaderModule _vkCreateShaderModule = NULL; \
	PFN_vkDestroyShaderModule _vkDestroyShaderModule = NULL; \
	PFN_vkCreatePipelineCache _vkCreatePipelineCache = NULL; \
	PFN_vkDestroyPipelineCache _vkDestroyPipelineCache = NULL; \
	PFN_vkGetPipelineCacheData _vkGetPipelineCacheData = NULL; \
	PFN_vkMergePipelineCaches _vkMergePipelineCaches = NULL; \
	PFN_vkCreateGraphicsPipelines _vkCreateGraphicsPipelines = NULL; \
	PFN_vkCreateComputePipelines _vkCreateComputePipelines = NULL; \
	PFN_vkDestroyPipeline _vkDestroyPipeline = NULL; \
	PFN_vkCreatePipelineLayout _vkCreatePipelineLayout = NULL; \
	PFN_vkDestroyPipelineLayout _vkDestroyPipelineLayout = NULL; \
	PFN_vkCreateSampler _vkCreateSampler = NULL; \
	PFN_vkDestroySampler _vkDestroySampler = NULL; \
	PFN_vkCreateDescriptorSetLayout _vkCreateDescriptorSetLayout = NULL; \
	PFN_vkDestroyDescriptorSetLayout _vkDestroyDescriptorSetLayout = NULL; \
	PFN_vkCreateDescriptorPool _vkCreateDescriptorPool = NULL; \
	PFN_vkDestroyDescriptorPool _vkDestroyDescriptorPool = NULL; \
	PFN_vkResetDescriptorPool _vkResetDescriptorPool = NULL; \
	PFN_vkAllocateDescriptorSets _vkAllocateDescriptorSets = NULL; \
	PFN_vkFreeDescriptorSets _vkFreeDescriptorSets = NULL; \
	PFN_vkUpdateDescriptorSets _vkUpdateDescriptorSets = NULL; \
	PFN_vkCreateFramebuffer _vkCreateFramebuffer = NULL; \
	PFN_vkDestroyFramebuffer _vkDestroyFramebuffer = NULL; \
	PFN_vkCreateRenderPass _vkCreateRenderPass = NULL; \
	PFN_vkDestroyRenderPass _vkDestroyRenderPass = NULL; \
	PFN_vkGetRenderAreaGranularity _vkGetRenderAreaGranularity = NULL; \
	PFN_vkCreateCommandPool _vkCreateCommandPool = NULL; \
	PFN_vkDestroyCommandPool _vkDestroyCommandPool = NULL; \
	PFN_vkResetCommandPool _vkResetCommandPool = NULL; \
	PFN_vkAllocateCommandBuffers _vkAllocateCommandBuffers = NULL; \
	PFN_vkFreeCommandBuffers _vkFreeCommandBuffers = NULL; \
	PFN_vkBeginCommandBuffer _vkBeginCommandBuffer = NULL; \
	PFN_vkEndCommandBuffer _vkEndCommandBuffer = NULL; \
	PFN_vkResetCommandBuffer _vkResetCommandBuffer = NULL; \
	PFN_vkCmdBindPipeline _vkCmdBindPipeline = NULL; \
	PFN_vkCmdSetViewport _vkCmdSetViewport = NULL; \
	PFN_vkCmdSetScissor _vkCmdSetScissor = NULL; \
	PFN_vkCmdSetLineWidth _vkCmdSetLineWidth = NULL; \
	PFN_vkCmdSetDepthBias _vkCmdSetDepthBias = NULL; \
	PFN_vkCmdSetBlendConstants _vkCmdSetBlendConstants = NULL; \
	PFN_vkCmdSetDepthBounds _vkCmdSetDepthBounds = NULL; \
	PFN_vkCmdSetStencilCompareMask _vkCmdSetStencilCompareMask = NULL; \
	PFN_vkCmdSetStencilWriteMask _vkCmdSetStencilWriteMask = NULL; \
	PFN_vkCmdSetStencilReference _vkCmdSetStencilReference = NULL; \
	PFN_vkCmdBindDescriptorSets _vkCmdBindDescriptorSets = NULL; \
	PFN_vkCmdBindIndexBuffer _vkCmdBindIndexBuffer = NULL; \
	PFN_vkCmdBindVertexBuffers _vkCmdBindVertexBuffers = NULL; \
	PFN_vkCmdDraw _vkCmdDraw = NULL; \
	PFN_vkCmdDrawIndexed _vkCmdDrawIndexed = NULL; \
	PFN_vkCmdDrawIndirect _vkCmdDrawIndirect = NULL; \
	PFN_vkCmdDrawIndexedIndirect _vkCmdDrawIndexedIndirect = NULL; \
	PFN_vkCmdDispatch _vkCmdDispatch = NULL; \
	PFN_vkCmdDispatchIndirect _vkCmdDispatchIndirect = NULL; \
	PFN_vkCmdCopyBuffer _vkCmdCopyBuffer = NULL; \
	PFN_vkCmdCopyImage _vkCmdCopyImage = NULL; \
	PFN_vkCmdBlitImage _vkCmdBlitImage = NULL; \
	PFN_vkCmdCopyBufferToImage _vkCmdCopyBufferToImage = NULL; \
	PFN_vkCmdCopyImageToBuffer _vkCmdCopyImageToBuffer = NULL; \
	PFN_vkCmdUpdateBuffer _vkCmdUpdateBuffer = NULL; \
	PFN_vkCmdFillBuffer _vkCmdFillBuffer = NULL; \
	PFN_vkCmdClearColorImage _vkCmdClearColorImage = NULL; \
	PFN_vkCmdClearDepthStencilImage _vkCmdClearDepthStencilImage = NULL; \
	PFN_vkCmdClearAttachments _vkCmdClearAttachments = NULL; \
	PFN_vkCmdResolveImage _vkCmdResolveImage = NULL; \
	PFN_vkCmdSetEvent _vkCmdSetEvent = NULL; \
	PFN_vkCmdResetEvent _vkCmdResetEvent = NULL; \
	PFN_vkCmdWaitEvents _vkCmdWaitEvents = NULL; \
	PFN_vkCmdPipelineBarrier _vkCmdPipelineBarrier = NULL; \
	PFN_vkCmdBeginQuery _vkCmdBeginQuery = NULL; \
	PFN_vkCmdEndQuery _vkCmdEndQuery = NULL; \
	PFN_vkCmdResetQueryPool _vkCmdResetQueryPool = NULL; \
	PFN_vkCmdWriteTimestamp _vkCmdWriteTimestamp = NULL; \
	PFN_vkCmdCopyQueryPoolResults _vkCmdCopyQueryPoolResults = NULL; \
	PFN_vkCmdPushConstants _vkCmdPushConstants = NULL; \
	PFN_vkCmdBeginRenderPass _vkCmdBeginRenderPass = NULL; \
	PFN_vkCmdNextSubpass _vkCmdNextSubpass = NULL; \
	PFN_vkCmdEndRenderPass _vkCmdEndRenderPass = NULL; \
	PFN_vkCmdExecuteCommands _vkCmdExecuteCommands = NULL; \
	PFN_vkDestroySurfaceKHR _vkDestroySurfaceKHR = NULL; \
	PFN_vkGetPhysicalDeviceSurfaceSupportKHR _vkGetPhysicalDeviceSurfaceSupportKHR = NULL; \
	PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR _vkGetPhysicalDeviceSurfaceCapabilitiesKHR = NULL; \
	PFN_vkGetPhysicalDeviceSurfaceFormatsKHR _vkGetPhysicalDeviceSurfaceFormatsKHR = NULL; \
	PFN_vkGetPhysicalDeviceSurfacePresentModesKHR _vkGetPhysicalDeviceSurfacePresentModesKHR = NULL; \
	PFN_vkCreateSwapchainKHR _vkCreateSwapchainKHR = NULL; \
	PFN_vkDestroySwapchainKHR _vkDestroySwapchainKHR = NULL; \
	PFN_vkGetSwapchainImagesKHR _vkGetSwapchainImagesKHR = NULL; \
	PFN_vkAcquireNextImageKHR _vkAcquireNextImageKHR = NULL; \
	PFN_vkQueuePresentKHR _vkQueuePresentKHR = NULL; \
	PFN_vkGetPhysicalDeviceDisplayPropertiesKHR _vkGetPhysicalDeviceDisplayPropertiesKHR = NULL; \
	PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR _vkGetPhysicalDeviceDisplayPlanePropertiesKHR = NULL; \
	PFN_vkGetDisplayPlaneSupportedDisplaysKHR _vkGetDisplayPlaneSupportedDisplaysKHR = NULL; \
	PFN_vkGetDisplayModePropertiesKHR _vkGetDisplayModePropertiesKHR = NULL; \
	PFN_vkCreateDisplayModeKHR _vkCreateDisplayModeKHR = NULL; \
	PFN_vkGetDisplayPlaneCapabilitiesKHR _vkGetDisplayPlaneCapabilitiesKHR = NULL; \
	PFN_vkCreateDisplayPlaneSurfaceKHR _vkCreateDisplayPlaneSurfaceKHR = NULL; \
	PFN_vkCreateSharedSwapchainsKHR _vkCreateSharedSwapchainsKHR = NULL; \
	PFN_vkCreateDebugReportCallbackEXT _vkCreateDebugReportCallbackEXT = NULL; \
	PFN_vkDestroyDebugReportCallbackEXT _vkDestroyDebugReportCallbackEXT = NULL; \
	PFN_vkDebugReportMessageEXT _vkDebugReportMessageEXT = NULL; \
	PFN_vkDebugMarkerSetObjectTagEXT _vkDebugMarkerSetObjectTagEXT = NULL; \
	PFN_vkDebugMarkerSetObjectNameEXT _vkDebugMarkerSetObjectNameEXT = NULL; \
	PFN_vkCmdDebugMarkerBeginEXT _vkCmdDebugMarkerBeginEXT = NULL; \
	PFN_vkCmdDebugMarkerEndEXT _vkCmdDebugMarkerEndEXT = NULL; \
	PFN_vkCmdDebugMarkerInsertEXT _vkCmdDebugMarkerInsertEXT = NULL;

#ifdef VK_USE_PLATFORM_XLIB_KHR
#define VK_USE_PLATFORM_XLIB_KHR_GLOBAL_FUNC \
	PFN_vkCreateXlibSurfaceKHR _vkCreateXlibSurfaceKHR = NULL; \
	PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR _vkGetPhysicalDeviceXlibPresentationSupportKHR = NULL;
#else
#define VK_USE_PLATFORM_XLIB_KHR_GLOBAL_FUNC
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
#define VK_USE_PLATFORM_XCB_KHR_GLOBAL_FUNC \
	PFN_vkCreateXcbSurfaceKHR _vkCreateXcbSurfaceKHR = NULL; \
	PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR _vkGetPhysicalDeviceXcbPresentationSupportKHR = NULL;
#else
#define VK_USE_PLATFORM_XCB_KHR_GLOBAL_FUNC
#endif

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
#define VK_USE_PLATFORM_WAYLAND_KHR_GLOBAL_FUNC \
	PFN_vkCreateWaylandSurfaceKHR _vkCreateWaylandSurfaceKHR = NULL; \
	PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR _vkGetPhysicalDeviceWaylandPresentationSupportKHR = NULL;
#else
#define VK_USE_PLATFORM_WAYLAND_KHR_GLOBAL_FUNC
#endif

#ifdef VK_USE_PLATFORM_MIR_KHR
#define VK_USE_PLATFORM_MIR_KHR_GLOBAL_FUNC \
	PFN_vkCreateMirSurfaceKHR _vkCreateMirSurfaceKHR = NULL; \
	PFN_vkGetPhysicalDeviceMirPresentationSupportKHR _vkGetPhysicalDeviceMirPresentationSupportKHR = NULL;
#else
#define VK_USE_PLATFORM_MIR_KHR_GLOBAL_FUNC
#endif

#ifdef VK_USE_PLATFORM_ANDROID_KHR
#define VK_USE_PLATFORM_ANDROID_KHR_GLOBAL_FUNC \
	PFN_vkCreateAndroidSurfaceKHR _vkCreateAndroidSurfaceKHR = NULL;
#else
#define VK_USE_PLATFORM_ANDROID_KHR_GLOBAL_FUNC
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
#define VK_USE_PLATFORM_WIN32_KHR_GLOBAL_FUNC \
	PFN_vkCreateWin32SurfaceKHR _vkCreateWin32SurfaceKHR = NULL; \
	PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR _vkGetPhysicalDeviceWin32PresentationSupportKHR = NULL;
#else
#define VK_USE_PLATFORM_WIN32_KHR_GLOBAL_FUNC
#endif

#if defined(VK_USE_PLATFORM_WIN32_KHR)
#define VK_LOADER_STATIC_MODULE_HANDLE static HMODULE _vkModule = NULL;
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
#define VK_LOADER_STATIC_MODULE_HANDLE static void* _vkModule = NULL;
#else
#define VK_LOADER_STATIC_MODULE_HANDLE static int _vkModule = 0;
#endif

#define VK_LOADER_GLOBAL \
	VK_LOADER_STATIC_MODULE_HANDLE; \
	VK_LOADER_GLOBAL_FUNC; \
	VK_USE_PLATFORM_XLIB_KHR_GLOBAL_FUNC; \
	VK_USE_PLATFORM_XCB_KHR_GLOBAL_FUNC; \
	VK_USE_PLATFORM_WAYLAND_KHR_GLOBAL_FUNC; \
	VK_USE_PLATFORM_MIR_KHR_GLOBAL_FUNC; \
	VK_USE_PLATFORM_ANDROID_KHR_GLOBAL_FUNC; \
	VK_USE_PLATFORM_WIN32_KHR_GLOBAL_FUNC;

#if defined(VK_USE_PLATFORM_WIN32_KHR)
#define VK_LOADER_LOAD_MODULE(n) if(!_vkModule) _vkModule = LoadLibraryA(n)
#define VK_LOADER_UNLOAD_MODULE() if(_vkModule) {FreeLibrary(_vkModule);_vkModule = NULL;}
#define VK_LOADER_GET_PROC(proc) GetProcAddress(_vkModule, proc)
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
#define VK_LOADER_LOAD_MODULE(n) if(!_vkModule) _vkModule = dlopen(n, RTLD_LAZY | RTLD_GLOBAL)
#define VK_LOADER_UNLOAD_MODULE() if(_vkModule) {dlclose(_vkModule);_vkModule = NULL;}
#define VK_LOADER_GET_PROC(proc) dlsym(_vkModule, proc)
#else
#define VK_LOADER_LOAD_MODULE(n)
#define VK_LOADER_UNLOAD_MODULE()
#define VK_LOADER_GET_PROC(proc)
#endif

#define VK_LOADER_HAS_LOADED() (_vkModule != 0)

#define VK_LOADER_LOAD_vkCreateInstance _vkCreateInstance = (PFN_vkCreateInstance)VK_LOADER_GET_PROC("vkCreateInstance")
#define VK_LOADER_LOAD_vkDestroyInstance _vkDestroyInstance = (PFN_vkDestroyInstance)VK_LOADER_GET_PROC("vkDestroyInstance")
#define VK_LOADER_LOAD_vkEnumeratePhysicalDevices _vkEnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)VK_LOADER_GET_PROC("vkEnumeratePhysicalDevices")
#define VK_LOADER_LOAD_vkGetDeviceProcAddr _vkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)VK_LOADER_GET_PROC("vkGetDeviceProcAddr")
#define VK_LOADER_LOAD_vkGetInstanceProcAddr _vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)VK_LOADER_GET_PROC("vkGetInstanceProcAddr")
#define VK_LOADER_LOAD_vkGetPhysicalDeviceProperties _vkGetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties)VK_LOADER_GET_PROC("vkGetPhysicalDeviceProperties")
#define VK_LOADER_LOAD_vkGetPhysicalDeviceQueueFamilyProperties _vkGetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)VK_LOADER_GET_PROC("vkGetPhysicalDeviceQueueFamilyProperties")
#define VK_LOADER_LOAD_vkGetPhysicalDeviceMemoryProperties _vkGetPhysicalDeviceMemoryProperties = (PFN_vkGetPhysicalDeviceMemoryProperties)VK_LOADER_GET_PROC("vkGetPhysicalDeviceMemoryProperties")
#define VK_LOADER_LOAD_vkGetPhysicalDeviceFeatures _vkGetPhysicalDeviceFeatures = (PFN_vkGetPhysicalDeviceFeatures)VK_LOADER_GET_PROC("vkGetPhysicalDeviceFeatures")
#define VK_LOADER_LOAD_vkGetPhysicalDeviceFormatProperties _vkGetPhysicalDeviceFormatProperties = (PFN_vkGetPhysicalDeviceFormatProperties)VK_LOADER_GET_PROC("vkGetPhysicalDeviceFormatProperties")
#define VK_LOADER_LOAD_vkGetPhysicalDeviceImageFormatProperties _vkGetPhysicalDeviceImageFormatProperties = (PFN_vkGetPhysicalDeviceImageFormatProperties)VK_LOADER_GET_PROC("vkGetPhysicalDeviceImageFormatProperties")
#define VK_LOADER_LOAD_vkCreateDevice _vkCreateDevice = (PFN_vkCreateDevice)VK_LOADER_GET_PROC("vkCreateDevice")
#define VK_LOADER_LOAD_vkDestroyDevice _vkDestroyDevice = (PFN_vkDestroyDevice)VK_LOADER_GET_PROC("vkDestroyDevice")
#define VK_LOADER_LOAD_vkEnumerateInstanceLayerProperties _vkEnumerateInstanceLayerProperties = (PFN_vkEnumerateInstanceLayerProperties)VK_LOADER_GET_PROC("vkEnumerateInstanceLayerProperties")
#define VK_LOADER_LOAD_vkEnumerateInstanceExtensionProperties _vkEnumerateInstanceExtensionProperties = (PFN_vkEnumerateInstanceExtensionProperties)VK_LOADER_GET_PROC("vkEnumerateInstanceExtensionProperties")
#define VK_LOADER_LOAD_vkEnumerateDeviceLayerProperties _vkEnumerateDeviceLayerProperties = (PFN_vkEnumerateDeviceLayerProperties)VK_LOADER_GET_PROC("vkEnumerateDeviceLayerProperties")
#define VK_LOADER_LOAD_vkEnumerateDeviceExtensionProperties _vkEnumerateDeviceExtensionProperties = (PFN_vkEnumerateDeviceExtensionProperties)VK_LOADER_GET_PROC("vkEnumerateDeviceExtensionProperties")
#define VK_LOADER_LOAD_vkGetDeviceQueue _vkGetDeviceQueue = (PFN_vkGetDeviceQueue)VK_LOADER_GET_PROC("vkGetDeviceQueue")
#define VK_LOADER_LOAD_vkQueueSubmit _vkQueueSubmit = (PFN_vkQueueSubmit)VK_LOADER_GET_PROC("vkQueueSubmit")
#define VK_LOADER_LOAD_vkQueueWaitIdle _vkQueueWaitIdle = (PFN_vkQueueWaitIdle)VK_LOADER_GET_PROC("vkQueueWaitIdle")
#define VK_LOADER_LOAD_vkDeviceWaitIdle _vkDeviceWaitIdle = (PFN_vkDeviceWaitIdle)VK_LOADER_GET_PROC("vkDeviceWaitIdle")
#define VK_LOADER_LOAD_vkAllocateMemory _vkAllocateMemory = (PFN_vkAllocateMemory)VK_LOADER_GET_PROC("vkAllocateMemory")
#define VK_LOADER_LOAD_vkFreeMemory _vkFreeMemory = (PFN_vkFreeMemory)VK_LOADER_GET_PROC("vkFreeMemory")
#define VK_LOADER_LOAD_vkMapMemory _vkMapMemory = (PFN_vkMapMemory)VK_LOADER_GET_PROC("vkMapMemory")
#define VK_LOADER_LOAD_vkUnmapMemory _vkUnmapMemory = (PFN_vkUnmapMemory)VK_LOADER_GET_PROC("vkUnmapMemory")
#define VK_LOADER_LOAD_vkFlushMappedMemoryRanges _vkFlushMappedMemoryRanges = (PFN_vkFlushMappedMemoryRanges)VK_LOADER_GET_PROC("vkFlushMappedMemoryRanges")
#define VK_LOADER_LOAD_vkInvalidateMappedMemoryRanges _vkInvalidateMappedMemoryRanges = (PFN_vkInvalidateMappedMemoryRanges)VK_LOADER_GET_PROC("vkInvalidateMappedMemoryRanges")
#define VK_LOADER_LOAD_vkGetDeviceMemoryCommitment _vkGetDeviceMemoryCommitment = (PFN_vkGetDeviceMemoryCommitment)VK_LOADER_GET_PROC("vkGetDeviceMemoryCommitment")
#define VK_LOADER_LOAD_vkGetBufferMemoryRequirements _vkGetBufferMemoryRequirements = (PFN_vkGetBufferMemoryRequirements)VK_LOADER_GET_PROC("vkGetBufferMemoryRequirements")
#define VK_LOADER_LOAD_vkBindBufferMemory _vkBindBufferMemory = (PFN_vkBindBufferMemory)VK_LOADER_GET_PROC("vkBindBufferMemory")
#define VK_LOADER_LOAD_vkGetImageMemoryRequirements _vkGetImageMemoryRequirements = (PFN_vkGetImageMemoryRequirements)VK_LOADER_GET_PROC("vkGetImageMemoryRequirements")
#define VK_LOADER_LOAD_vkBindImageMemory _vkBindImageMemory = (PFN_vkBindImageMemory)VK_LOADER_GET_PROC("vkBindImageMemory")
#define VK_LOADER_LOAD_vkGetImageSparseMemoryRequirements _vkGetImageSparseMemoryRequirements = (PFN_vkGetImageSparseMemoryRequirements)VK_LOADER_GET_PROC("vkGetImageSparseMemoryRequirements")
#define VK_LOADER_LOAD_vkGetPhysicalDeviceSparseImageFormatProperties _vkGetPhysicalDeviceSparseImageFormatProperties = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties)VK_LOADER_GET_PROC("vkGetPhysicalDeviceSparseImageFormatProperties")
#define VK_LOADER_LOAD_vkQueueBindSparse _vkQueueBindSparse = (PFN_vkQueueBindSparse)VK_LOADER_GET_PROC("vkQueueBindSparse")
#define VK_LOADER_LOAD_vkCreateFence _vkCreateFence = (PFN_vkCreateFence)VK_LOADER_GET_PROC("vkCreateFence")
#define VK_LOADER_LOAD_vkDestroyFence _vkDestroyFence = (PFN_vkDestroyFence)VK_LOADER_GET_PROC("vkDestroyFence")
#define VK_LOADER_LOAD_vkResetFences _vkResetFences = (PFN_vkResetFences)VK_LOADER_GET_PROC("vkResetFences")
#define VK_LOADER_LOAD_vkGetFenceStatus _vkGetFenceStatus = (PFN_vkGetFenceStatus)VK_LOADER_GET_PROC("vkGetFenceStatus")
#define VK_LOADER_LOAD_vkWaitForFences _vkWaitForFences = (PFN_vkWaitForFences)VK_LOADER_GET_PROC("vkWaitForFences")
#define VK_LOADER_LOAD_vkCreateSemaphore _vkCreateSemaphore = (PFN_vkCreateSemaphore)VK_LOADER_GET_PROC("vkCreateSemaphore")
#define VK_LOADER_LOAD_vkDestroySemaphore _vkDestroySemaphore = (PFN_vkDestroySemaphore)VK_LOADER_GET_PROC("vkDestroySemaphore")
#define VK_LOADER_LOAD_vkCreateEvent _vkCreateEvent = (PFN_vkCreateEvent)VK_LOADER_GET_PROC("vkCreateEvent")
#define VK_LOADER_LOAD_vkDestroyEvent _vkDestroyEvent = (PFN_vkDestroyEvent)VK_LOADER_GET_PROC("vkDestroyEvent")
#define VK_LOADER_LOAD_vkGetEventStatus _vkGetEventStatus = (PFN_vkGetEventStatus)VK_LOADER_GET_PROC("vkGetEventStatus")
#define VK_LOADER_LOAD_vkSetEvent _vkSetEvent = (PFN_vkSetEvent)VK_LOADER_GET_PROC("vkSetEvent")
#define VK_LOADER_LOAD_vkResetEvent _vkResetEvent = (PFN_vkResetEvent)VK_LOADER_GET_PROC("vkResetEvent")
#define VK_LOADER_LOAD_vkCreateQueryPool _vkCreateQueryPool = (PFN_vkCreateQueryPool)VK_LOADER_GET_PROC("vkCreateQueryPool")
#define VK_LOADER_LOAD_vkDestroyQueryPool _vkDestroyQueryPool = (PFN_vkDestroyQueryPool)VK_LOADER_GET_PROC("vkDestroyQueryPool")
#define VK_LOADER_LOAD_vkGetQueryPoolResults _vkGetQueryPoolResults = (PFN_vkGetQueryPoolResults)VK_LOADER_GET_PROC("vkGetQueryPoolResults")
#define VK_LOADER_LOAD_vkCreateBuffer _vkCreateBuffer = (PFN_vkCreateBuffer)VK_LOADER_GET_PROC("vkCreateBuffer")
#define VK_LOADER_LOAD_vkDestroyBuffer _vkDestroyBuffer = (PFN_vkDestroyBuffer)VK_LOADER_GET_PROC("vkDestroyBuffer")
#define VK_LOADER_LOAD_vkCreateBufferView _vkCreateBufferView = (PFN_vkCreateBufferView)VK_LOADER_GET_PROC("vkCreateBufferView")
#define VK_LOADER_LOAD_vkDestroyBufferView _vkDestroyBufferView = (PFN_vkDestroyBufferView)VK_LOADER_GET_PROC("vkDestroyBufferView")
#define VK_LOADER_LOAD_vkCreateImage _vkCreateImage = (PFN_vkCreateImage)VK_LOADER_GET_PROC("vkCreateImage")
#define VK_LOADER_LOAD_vkDestroyImage _vkDestroyImage = (PFN_vkDestroyImage)VK_LOADER_GET_PROC("vkDestroyImage")
#define VK_LOADER_LOAD_vkGetImageSubresourceLayout _vkGetImageSubresourceLayout = (PFN_vkGetImageSubresourceLayout)VK_LOADER_GET_PROC("vkGetImageSubresourceLayout")
#define VK_LOADER_LOAD_vkCreateImageView _vkCreateImageView = (PFN_vkCreateImageView)VK_LOADER_GET_PROC("vkCreateImageView")
#define VK_LOADER_LOAD_vkDestroyImageView _vkDestroyImageView = (PFN_vkDestroyImageView)VK_LOADER_GET_PROC("vkDestroyImageView")
#define VK_LOADER_LOAD_vkCreateShaderModule _vkCreateShaderModule = (PFN_vkCreateShaderModule)VK_LOADER_GET_PROC("vkCreateShaderModule")
#define VK_LOADER_LOAD_vkDestroyShaderModule _vkDestroyShaderModule = (PFN_vkDestroyShaderModule)VK_LOADER_GET_PROC("vkDestroyShaderModule")
#define VK_LOADER_LOAD_vkCreatePipelineCache _vkCreatePipelineCache = (PFN_vkCreatePipelineCache)VK_LOADER_GET_PROC("vkCreatePipelineCache")
#define VK_LOADER_LOAD_vkDestroyPipelineCache _vkDestroyPipelineCache = (PFN_vkDestroyPipelineCache)VK_LOADER_GET_PROC("vkDestroyPipelineCache")
#define VK_LOADER_LOAD_vkGetPipelineCacheData _vkGetPipelineCacheData = (PFN_vkGetPipelineCacheData)VK_LOADER_GET_PROC("vkGetPipelineCacheData")
#define VK_LOADER_LOAD_vkMergePipelineCaches _vkMergePipelineCaches = (PFN_vkMergePipelineCaches)VK_LOADER_GET_PROC("vkMergePipelineCaches")
#define VK_LOADER_LOAD_vkCreateGraphicsPipelines _vkCreateGraphicsPipelines = (PFN_vkCreateGraphicsPipelines)VK_LOADER_GET_PROC("vkCreateGraphicsPipelines")
#define VK_LOADER_LOAD_vkCreateComputePipelines _vkCreateComputePipelines = (PFN_vkCreateComputePipelines)VK_LOADER_GET_PROC("vkCreateComputePipelines")
#define VK_LOADER_LOAD_vkDestroyPipeline _vkDestroyPipeline = (PFN_vkDestroyPipeline)VK_LOADER_GET_PROC("vkDestroyPipeline")
#define VK_LOADER_LOAD_vkCreatePipelineLayout _vkCreatePipelineLayout = (PFN_vkCreatePipelineLayout)VK_LOADER_GET_PROC("vkCreatePipelineLayout")
#define VK_LOADER_LOAD_vkDestroyPipelineLayout _vkDestroyPipelineLayout = (PFN_vkDestroyPipelineLayout)VK_LOADER_GET_PROC("vkDestroyPipelineLayout")
#define VK_LOADER_LOAD_vkCreateSampler _vkCreateSampler = (PFN_vkCreateSampler)VK_LOADER_GET_PROC("vkCreateSampler")
#define VK_LOADER_LOAD_vkDestroySampler _vkDestroySampler = (PFN_vkDestroySampler)VK_LOADER_GET_PROC("vkDestroySampler")
#define VK_LOADER_LOAD_vkCreateDescriptorSetLayout _vkCreateDescriptorSetLayout = (PFN_vkCreateDescriptorSetLayout)VK_LOADER_GET_PROC("vkCreateDescriptorSetLayout")
#define VK_LOADER_LOAD_vkDestroyDescriptorSetLayout _vkDestroyDescriptorSetLayout = (PFN_vkDestroyDescriptorSetLayout)VK_LOADER_GET_PROC("vkDestroyDescriptorSetLayout")
#define VK_LOADER_LOAD_vkCreateDescriptorPool _vkCreateDescriptorPool = (PFN_vkCreateDescriptorPool)VK_LOADER_GET_PROC("vkCreateDescriptorPool")
#define VK_LOADER_LOAD_vkDestroyDescriptorPool _vkDestroyDescriptorPool = (PFN_vkDestroyDescriptorPool)VK_LOADER_GET_PROC("vkDestroyDescriptorPool")
#define VK_LOADER_LOAD_vkResetDescriptorPool _vkResetDescriptorPool = (PFN_vkResetDescriptorPool)VK_LOADER_GET_PROC("vkResetDescriptorPool")
#define VK_LOADER_LOAD_vkAllocateDescriptorSets _vkAllocateDescriptorSets = (PFN_vkAllocateDescriptorSets)VK_LOADER_GET_PROC("vkAllocateDescriptorSets")
#define VK_LOADER_LOAD_vkFreeDescriptorSets _vkFreeDescriptorSets = (PFN_vkFreeDescriptorSets)VK_LOADER_GET_PROC("vkFreeDescriptorSets")
#define VK_LOADER_LOAD_vkUpdateDescriptorSets _vkUpdateDescriptorSets = (PFN_vkUpdateDescriptorSets)VK_LOADER_GET_PROC("vkUpdateDescriptorSets")
#define VK_LOADER_LOAD_vkCreateFramebuffer _vkCreateFramebuffer = (PFN_vkCreateFramebuffer)VK_LOADER_GET_PROC("vkCreateFramebuffer")
#define VK_LOADER_LOAD_vkDestroyFramebuffer _vkDestroyFramebuffer = (PFN_vkDestroyFramebuffer)VK_LOADER_GET_PROC("vkDestroyFramebuffer")
#define VK_LOADER_LOAD_vkCreateRenderPass _vkCreateRenderPass = (PFN_vkCreateRenderPass)VK_LOADER_GET_PROC("vkCreateRenderPass")
#define VK_LOADER_LOAD_vkDestroyRenderPass _vkDestroyRenderPass = (PFN_vkDestroyRenderPass)VK_LOADER_GET_PROC("vkDestroyRenderPass")
#define VK_LOADER_LOAD_vkGetRenderAreaGranularity _vkGetRenderAreaGranularity = (PFN_vkGetRenderAreaGranularity)VK_LOADER_GET_PROC("vkGetRenderAreaGranularity")
#define VK_LOADER_LOAD_vkCreateCommandPool _vkCreateCommandPool = (PFN_vkCreateCommandPool)VK_LOADER_GET_PROC("vkCreateCommandPool")
#define VK_LOADER_LOAD_vkDestroyCommandPool _vkDestroyCommandPool = (PFN_vkDestroyCommandPool)VK_LOADER_GET_PROC("vkDestroyCommandPool")
#define VK_LOADER_LOAD_vkResetCommandPool _vkResetCommandPool = (PFN_vkResetCommandPool)VK_LOADER_GET_PROC("vkResetCommandPool")
#define VK_LOADER_LOAD_vkAllocateCommandBuffers _vkAllocateCommandBuffers = (PFN_vkAllocateCommandBuffers)VK_LOADER_GET_PROC("vkAllocateCommandBuffers")
#define VK_LOADER_LOAD_vkFreeCommandBuffers _vkFreeCommandBuffers = (PFN_vkFreeCommandBuffers)VK_LOADER_GET_PROC("vkFreeCommandBuffers")
#define VK_LOADER_LOAD_vkBeginCommandBuffer _vkBeginCommandBuffer = (PFN_vkBeginCommandBuffer)VK_LOADER_GET_PROC("vkBeginCommandBuffer")
#define VK_LOADER_LOAD_vkEndCommandBuffer _vkEndCommandBuffer = (PFN_vkEndCommandBuffer)VK_LOADER_GET_PROC("vkEndCommandBuffer")
#define VK_LOADER_LOAD_vkResetCommandBuffer _vkResetCommandBuffer = (PFN_vkResetCommandBuffer)VK_LOADER_GET_PROC("vkResetCommandBuffer")
#define VK_LOADER_LOAD_vkCmdBindPipeline _vkCmdBindPipeline = (PFN_vkCmdBindPipeline)VK_LOADER_GET_PROC("vkCmdBindPipeline")
#define VK_LOADER_LOAD_vkCmdSetViewport _vkCmdSetViewport = (PFN_vkCmdSetViewport)VK_LOADER_GET_PROC("vkCmdSetViewport")
#define VK_LOADER_LOAD_vkCmdSetScissor _vkCmdSetScissor = (PFN_vkCmdSetScissor)VK_LOADER_GET_PROC("vkCmdSetScissor")
#define VK_LOADER_LOAD_vkCmdSetLineWidth _vkCmdSetLineWidth = (PFN_vkCmdSetLineWidth)VK_LOADER_GET_PROC("vkCmdSetLineWidth")
#define VK_LOADER_LOAD_vkCmdSetDepthBias _vkCmdSetDepthBias = (PFN_vkCmdSetDepthBias)VK_LOADER_GET_PROC("vkCmdSetDepthBias")
#define VK_LOADER_LOAD_vkCmdSetBlendConstants _vkCmdSetBlendConstants = (PFN_vkCmdSetBlendConstants)VK_LOADER_GET_PROC("vkCmdSetBlendConstants")
#define VK_LOADER_LOAD_vkCmdSetDepthBounds _vkCmdSetDepthBounds = (PFN_vkCmdSetDepthBounds)VK_LOADER_GET_PROC("vkCmdSetDepthBounds")
#define VK_LOADER_LOAD_vkCmdSetStencilCompareMask _vkCmdSetStencilCompareMask = (PFN_vkCmdSetStencilCompareMask)VK_LOADER_GET_PROC("vkCmdSetStencilCompareMask")
#define VK_LOADER_LOAD_vkCmdSetStencilWriteMask _vkCmdSetStencilWriteMask = (PFN_vkCmdSetStencilWriteMask)VK_LOADER_GET_PROC("vkCmdSetStencilWriteMask")
#define VK_LOADER_LOAD_vkCmdSetStencilReference _vkCmdSetStencilReference = (PFN_vkCmdSetStencilReference)VK_LOADER_GET_PROC("vkCmdSetStencilReference")
#define VK_LOADER_LOAD_vkCmdBindDescriptorSets _vkCmdBindDescriptorSets = (PFN_vkCmdBindDescriptorSets)VK_LOADER_GET_PROC("vkCmdBindDescriptorSets")
#define VK_LOADER_LOAD_vkCmdBindIndexBuffer _vkCmdBindIndexBuffer = (PFN_vkCmdBindIndexBuffer)VK_LOADER_GET_PROC("vkCmdBindIndexBuffer")
#define VK_LOADER_LOAD_vkCmdBindVertexBuffers _vkCmdBindVertexBuffers = (PFN_vkCmdBindVertexBuffers)VK_LOADER_GET_PROC("vkCmdBindVertexBuffers")
#define VK_LOADER_LOAD_vkCmdDraw _vkCmdDraw = (PFN_vkCmdDraw)VK_LOADER_GET_PROC("vkCmdDraw")
#define VK_LOADER_LOAD_vkCmdDrawIndexed _vkCmdDrawIndexed = (PFN_vkCmdDrawIndexed)VK_LOADER_GET_PROC("vkCmdDrawIndexed")
#define VK_LOADER_LOAD_vkCmdDrawIndirect _vkCmdDrawIndirect = (PFN_vkCmdDrawIndirect)VK_LOADER_GET_PROC("vkCmdDrawIndirect")
#define VK_LOADER_LOAD_vkCmdDrawIndexedIndirect _vkCmdDrawIndexedIndirect = (PFN_vkCmdDrawIndexedIndirect)VK_LOADER_GET_PROC("vkCmdDrawIndexedIndirect")
#define VK_LOADER_LOAD_vkCmdDispatch _vkCmdDispatch = (PFN_vkCmdDispatch)VK_LOADER_GET_PROC("vkCmdDispatch")
#define VK_LOADER_LOAD_vkCmdDispatchIndirect _vkCmdDispatchIndirect = (PFN_vkCmdDispatchIndirect)VK_LOADER_GET_PROC("vkCmdDispatchIndirect")
#define VK_LOADER_LOAD_vkCmdCopyBuffer _vkCmdCopyBuffer = (PFN_vkCmdCopyBuffer)VK_LOADER_GET_PROC("vkCmdCopyBuffer")
#define VK_LOADER_LOAD_vkCmdCopyImage _vkCmdCopyImage = (PFN_vkCmdCopyImage)VK_LOADER_GET_PROC("vkCmdCopyImage")
#define VK_LOADER_LOAD_vkCmdBlitImage _vkCmdBlitImage = (PFN_vkCmdBlitImage)VK_LOADER_GET_PROC("vkCmdBlitImage")
#define VK_LOADER_LOAD_vkCmdCopyBufferToImage _vkCmdCopyBufferToImage = (PFN_vkCmdCopyBufferToImage)VK_LOADER_GET_PROC("vkCmdCopyBufferToImage")
#define VK_LOADER_LOAD_vkCmdCopyImageToBuffer _vkCmdCopyImageToBuffer = (PFN_vkCmdCopyImageToBuffer)VK_LOADER_GET_PROC("vkCmdCopyImageToBuffer")
#define VK_LOADER_LOAD_vkCmdUpdateBuffer _vkCmdUpdateBuffer = (PFN_vkCmdUpdateBuffer)VK_LOADER_GET_PROC("vkCmdUpdateBuffer")
#define VK_LOADER_LOAD_vkCmdFillBuffer _vkCmdFillBuffer = (PFN_vkCmdFillBuffer)VK_LOADER_GET_PROC("vkCmdFillBuffer")
#define VK_LOADER_LOAD_vkCmdClearColorImage _vkCmdClearColorImage = (PFN_vkCmdClearColorImage)VK_LOADER_GET_PROC("vkCmdClearColorImage")
#define VK_LOADER_LOAD_vkCmdClearDepthStencilImage _vkCmdClearDepthStencilImage = (PFN_vkCmdClearDepthStencilImage)VK_LOADER_GET_PROC("vkCmdClearDepthStencilImage")
#define VK_LOADER_LOAD_vkCmdClearAttachments _vkCmdClearAttachments = (PFN_vkCmdClearAttachments)VK_LOADER_GET_PROC("vkCmdClearAttachments")
#define VK_LOADER_LOAD_vkCmdResolveImage _vkCmdResolveImage = (PFN_vkCmdResolveImage)VK_LOADER_GET_PROC("vkCmdResolveImage")
#define VK_LOADER_LOAD_vkCmdSetEvent _vkCmdSetEvent = (PFN_vkCmdSetEvent)VK_LOADER_GET_PROC("vkCmdSetEvent")
#define VK_LOADER_LOAD_vkCmdResetEvent _vkCmdResetEvent = (PFN_vkCmdResetEvent)VK_LOADER_GET_PROC("vkCmdResetEvent")
#define VK_LOADER_LOAD_vkCmdWaitEvents _vkCmdWaitEvents = (PFN_vkCmdWaitEvents)VK_LOADER_GET_PROC("vkCmdWaitEvents")
#define VK_LOADER_LOAD_vkCmdPipelineBarrier _vkCmdPipelineBarrier = (PFN_vkCmdPipelineBarrier)VK_LOADER_GET_PROC("vkCmdPipelineBarrier")
#define VK_LOADER_LOAD_vkCmdBeginQuery _vkCmdBeginQuery = (PFN_vkCmdBeginQuery)VK_LOADER_GET_PROC("vkCmdBeginQuery")
#define VK_LOADER_LOAD_vkCmdEndQuery _vkCmdEndQuery = (PFN_vkCmdEndQuery)VK_LOADER_GET_PROC("vkCmdEndQuery")
#define VK_LOADER_LOAD_vkCmdResetQueryPool _vkCmdResetQueryPool = (PFN_vkCmdResetQueryPool)VK_LOADER_GET_PROC("vkCmdResetQueryPool")
#define VK_LOADER_LOAD_vkCmdWriteTimestamp _vkCmdWriteTimestamp = (PFN_vkCmdWriteTimestamp)VK_LOADER_GET_PROC("vkCmdWriteTimestamp")
#define VK_LOADER_LOAD_vkCmdCopyQueryPoolResults _vkCmdCopyQueryPoolResults = (PFN_vkCmdCopyQueryPoolResults)VK_LOADER_GET_PROC("vkCmdCopyQueryPoolResults")
#define VK_LOADER_LOAD_vkCmdPushConstants _vkCmdPushConstants = (PFN_vkCmdPushConstants)VK_LOADER_GET_PROC("vkCmdPushConstants")
#define VK_LOADER_LOAD_vkCmdBeginRenderPass _vkCmdBeginRenderPass = (PFN_vkCmdBeginRenderPass)VK_LOADER_GET_PROC("vkCmdBeginRenderPass")
#define VK_LOADER_LOAD_vkCmdNextSubpass _vkCmdNextSubpass = (PFN_vkCmdNextSubpass)VK_LOADER_GET_PROC("vkCmdNextSubpass")
#define VK_LOADER_LOAD_vkCmdEndRenderPass _vkCmdEndRenderPass = (PFN_vkCmdEndRenderPass)VK_LOADER_GET_PROC("vkCmdEndRenderPass")
#define VK_LOADER_LOAD_vkCmdExecuteCommands _vkCmdExecuteCommands = (PFN_vkCmdExecuteCommands)VK_LOADER_GET_PROC("vkCmdExecuteCommands")
#define VK_LOADER_LOAD_vkDestroySurfaceKHR _vkDestroySurfaceKHR = (PFN_vkDestroySurfaceKHR)VK_LOADER_GET_PROC("vkDestroySurfaceKHR")
#define VK_LOADER_LOAD_vkGetPhysicalDeviceSurfaceSupportKHR _vkGetPhysicalDeviceSurfaceSupportKHR = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)VK_LOADER_GET_PROC("vkGetPhysicalDeviceSurfaceSupportKHR")
#define VK_LOADER_LOAD_vkGetPhysicalDeviceSurfaceCapabilitiesKHR _vkGetPhysicalDeviceSurfaceCapabilitiesKHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)VK_LOADER_GET_PROC("vkGetPhysicalDeviceSurfaceCapabilitiesKHR")
#define VK_LOADER_LOAD_vkGetPhysicalDeviceSurfaceFormatsKHR _vkGetPhysicalDeviceSurfaceFormatsKHR = (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)VK_LOADER_GET_PROC("vkGetPhysicalDeviceSurfaceFormatsKHR")
#define VK_LOADER_LOAD_vkGetPhysicalDeviceSurfacePresentModesKHR _vkGetPhysicalDeviceSurfacePresentModesKHR = (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)VK_LOADER_GET_PROC("vkGetPhysicalDeviceSurfacePresentModesKHR")
#define VK_LOADER_LOAD_vkCreateSwapchainKHR _vkCreateSwapchainKHR = (PFN_vkCreateSwapchainKHR)VK_LOADER_GET_PROC("vkCreateSwapchainKHR")
#define VK_LOADER_LOAD_vkDestroySwapchainKHR _vkDestroySwapchainKHR = (PFN_vkDestroySwapchainKHR)VK_LOADER_GET_PROC("vkDestroySwapchainKHR")
#define VK_LOADER_LOAD_vkGetSwapchainImagesKHR _vkGetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR)VK_LOADER_GET_PROC("vkGetSwapchainImagesKHR")
#define VK_LOADER_LOAD_vkAcquireNextImageKHR _vkAcquireNextImageKHR = (PFN_vkAcquireNextImageKHR)VK_LOADER_GET_PROC("vkAcquireNextImageKHR")
#define VK_LOADER_LOAD_vkQueuePresentKHR _vkQueuePresentKHR = (PFN_vkQueuePresentKHR)VK_LOADER_GET_PROC("vkQueuePresentKHR")
#define VK_LOADER_LOAD_vkGetPhysicalDeviceDisplayPropertiesKHR _vkGetPhysicalDeviceDisplayPropertiesKHR = (PFN_vkGetPhysicalDeviceDisplayPropertiesKHR)VK_LOADER_GET_PROC("vkGetPhysicalDeviceDisplayPropertiesKHR")
#define VK_LOADER_LOAD_vkGetPhysicalDeviceDisplayPlanePropertiesKHR _vkGetPhysicalDeviceDisplayPlanePropertiesKHR = (PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR)VK_LOADER_GET_PROC("vkGetPhysicalDeviceDisplayPlanePropertiesKHR")
#define VK_LOADER_LOAD_vkGetDisplayPlaneSupportedDisplaysKHR _vkGetDisplayPlaneSupportedDisplaysKHR = (PFN_vkGetDisplayPlaneSupportedDisplaysKHR)VK_LOADER_GET_PROC("vkGetDisplayPlaneSupportedDisplaysKHR")
#define VK_LOADER_LOAD_vkGetDisplayModePropertiesKHR _vkGetDisplayModePropertiesKHR = (PFN_vkGetDisplayModePropertiesKHR)VK_LOADER_GET_PROC("vkGetDisplayModePropertiesKHR")
#define VK_LOADER_LOAD_vkCreateDisplayModeKHR _vkCreateDisplayModeKHR = (PFN_vkCreateDisplayModeKHR)VK_LOADER_GET_PROC("vkCreateDisplayModeKHR")
#define VK_LOADER_LOAD_vkGetDisplayPlaneCapabilitiesKHR _vkGetDisplayPlaneCapabilitiesKHR = (PFN_vkGetDisplayPlaneCapabilitiesKHR)VK_LOADER_GET_PROC("vkGetDisplayPlaneCapabilitiesKHR")
#define VK_LOADER_LOAD_vkCreateDisplayPlaneSurfaceKHR _vkCreateDisplayPlaneSurfaceKHR = (PFN_vkCreateDisplayPlaneSurfaceKHR)VK_LOADER_GET_PROC("vkCreateDisplayPlaneSurfaceKHR")
#define VK_LOADER_LOAD_vkCreateSharedSwapchainsKHR _vkCreateSharedSwapchainsKHR = (PFN_vkCreateSharedSwapchainsKHR)VK_LOADER_GET_PROC("vkCreateSharedSwapchainsKHR")
#ifdef VK_USE_PLATFORM_XLIB_KHR
#define VK_LOADER_LOAD_vkCreateXlibSurfaceKHR _vkCreateXlibSurfaceKHR = (PFN_vkCreateXlibSurfaceKHR)VK_LOADER_GET_PROC("vkCreateXlibSurfaceKHR")
#define VK_LOADER_UNLOAD_vkCreateXlibSurfaceKHR _vkCreateXlibSurfaceKHR = NULL
#define VK_LOADER_LOAD_vkGetPhysicalDeviceXlibPresentationSupportKHR _vkGetPhysicalDeviceXlibPresentationSupportKHR = (PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR)VK_LOADER_GET_PROC("vkGetPhysicalDeviceXlibPresentationSupportKHR")
#define VK_LOADER_UNLOAD_vkGetPhysicalDeviceXlibPresentationSupportKHR _vkGetPhysicalDeviceXlibPresentationSupportKHR = NULL
#else
#define VK_LOADER_LOAD_vkCreateXlibSurfaceKHR
#define VK_LOADER_UNLOAD_vkCreateXlibSurfaceKHR
#define VK_LOADER_LOAD_vkGetPhysicalDeviceXlibPresentationSupportKHR
#define VK_LOADER_UNLOAD_vkGetPhysicalDeviceXlibPresentationSupportKHR
#endif
#ifdef VK_USE_PLATFORM_XCB_KHR
#define VK_LOADER_LOAD_vkCreateXcbSurfaceKHR _vkCreateXcbSurfaceKHR = (PFN_vkCreateXcbSurfaceKHR)VK_LOADER_GET_PROC("vkCreateXcbSurfaceKHR")
#define VK_LOADER_UNLOAD_vkCreateXcbSurfaceKHR _vkCreateXcbSurfaceKHR = NULL
#define VK_LOADER_LOAD_vkGetPhysicalDeviceXcbPresentationSupportKHR _vkGetPhysicalDeviceXcbPresentationSupportKHR = (PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR)VK_LOADER_GET_PROC("vkGetPhysicalDeviceXcbPresentationSupportKHR")
#define VK_LOADER_UNLOAD_vkGetPhysicalDeviceXcbPresentationSupportKHR _vkGetPhysicalDeviceXcbPresentationSupportKHR = NULL
#else
#define VK_LOADER_LOAD_vkCreateXcbSurfaceKHR
#define VK_LOADER_UNLOAD_vkCreateXcbSurfaceKHR
#define VK_LOADER_LOAD_vkGetPhysicalDeviceXcbPresentationSupportKHR
#define VK_LOADER_UNLOAD_vkGetPhysicalDeviceXcbPresentationSupportKHR
#endif
#ifdef VK_USE_PLATFORM_WAYLAND_KHR
#define VK_LOADER_LOAD_vkCreateWaylandSurfaceKHR _vkCreateWaylandSurfaceKHR = (PFN_vkCreateWaylandSurfaceKHR)VK_LOADER_GET_PROC("vkCreateWaylandSurfaceKHR")
#define VK_LOADER_UNLOAD_vkCreateWaylandSurfaceKHR _vkCreateWaylandSurfaceKHR = NULL
#define VK_LOADER_LOAD_vkGetPhysicalDeviceWaylandPresentationSupportKHR _vkGetPhysicalDeviceWaylandPresentationSupportKHR = (PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR)VK_LOADER_GET_PROC("vkGetPhysicalDeviceWaylandPresentationSupportKHR")
#define VK_LOADER_UNLOAD_vkGetPhysicalDeviceWaylandPresentationSupportKHR _vkGetPhysicalDeviceWaylandPresentationSupportKHR = NULL
#else
#define VK_LOADER_LOAD_vkCreateWaylandSurfaceKHR
#define VK_LOADER_UNLOAD_vkCreateWaylandSurfaceKHR
#define VK_LOADER_LOAD_vkGetPhysicalDeviceWaylandPresentationSupportKHR
#define VK_LOADER_UNLOAD_vkGetPhysicalDeviceWaylandPresentationSupportKHR
#endif
#ifdef VK_USE_PLATFORM_MIR_KHR
#define VK_LOADER_LOAD_vkCreateMirSurfaceKHR _vkCreateMirSurfaceKHR = (PFN_vkCreateMirSurfaceKHR)VK_LOADER_GET_PROC("vkCreateMirSurfaceKHR")
#define VK_LOADER_UNLOAD_vkCreateMirSurfaceKHR _vkCreateMirSurfaceKHR = NULL
#define VK_LOADER_LOAD_vkGetPhysicalDeviceMirPresentationSupportKHR _vkGetPhysicalDeviceMirPresentationSupportKHR = (PFN_vkGetPhysicalDeviceMirPresentationSupportKHR)VK_LOADER_GET_PROC("vkGetPhysicalDeviceMirPresentationSupportKHR")
#define VK_LOADER_UNLOAD_vkGetPhysicalDeviceMirPresentationSupportKHR _vkGetPhysicalDeviceMirPresentationSupportKHR = NULL
#else
#define VK_LOADER_LOAD_vkCreateMirSurfaceKHR
#define VK_LOADER_UNLOAD_vkCreateMirSurfaceKHR
#define VK_LOADER_LOAD_vkGetPhysicalDeviceMirPresentationSupportKHR
#define VK_LOADER_UNLOAD_vkGetPhysicalDeviceMirPresentationSupportKHR
#endif
#ifdef VK_USE_PLATFORM_ANDROID_KHR
#define VK_LOADER_LOAD_vkCreateAndroidSurfaceKHR _vkCreateAndroidSurfaceKHR = (PFN_vkCreateAndroidSurfaceKHR)VK_LOADER_GET_PROC("vkCreateAndroidSurfaceKHR")
#define VK_LOADER_UNLOAD_vkCreateAndroidSurfaceKHR _vkCreateAndroidSurfaceKHR = NULL
#else
#define VK_LOADER_LOAD_vkCreateAndroidSurfaceKHR
#define VK_LOADER_UNLOAD_vkCreateAndroidSurfaceKHR
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
#define VK_LOADER_LOAD_vkCreateWin32SurfaceKHR _vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)VK_LOADER_GET_PROC("vkCreateWin32SurfaceKHR")
#define VK_LOADER_UNLOAD_vkCreateWin32SurfaceKHR _vkCreateWin32SurfaceKHR = NULL
#define VK_LOADER_LOAD_vkGetPhysicalDeviceWin32PresentationSupportKHR _vkGetPhysicalDeviceWin32PresentationSupportKHR = (PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR)VK_LOADER_GET_PROC("vkGetPhysicalDeviceWin32PresentationSupportKHR")
#define VK_LOADER_UNLOAD_vkGetPhysicalDeviceWin32PresentationSupportKHR _vkGetPhysicalDeviceWin32PresentationSupportKHR = NULL
#else
#define VK_LOADER_LOAD_vkCreateWin32SurfaceKHR
#define VK_LOADER_UNLOAD_vkCreateWin32SurfaceKHR
#define VK_LOADER_LOAD_vkGetPhysicalDeviceWin32PresentationSupportKHR
#define VK_LOADER_UNLOAD_vkGetPhysicalDeviceWin32PresentationSupportKHR
#endif
#define VK_LOADER_LOAD_vkCreateDebugReportCallbackEXT _vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)VK_LOADER_GET_PROC("vkCreateDebugReportCallbackEXT")
#define VK_LOADER_LOAD_vkDestroyDebugReportCallbackEXT _vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)VK_LOADER_GET_PROC("vkDestroyDebugReportCallbackEXT")
#define VK_LOADER_LOAD_vkDebugReportMessageEXT _vkDebugReportMessageEXT = (PFN_vkDebugReportMessageEXT)VK_LOADER_GET_PROC("vkDebugReportMessageEXT")
#define VK_LOADER_LOAD_vkDebugMarkerSetObjectTagEXT _vkDebugMarkerSetObjectTagEXT = (PFN_vkDebugMarkerSetObjectTagEXT)VK_LOADER_GET_PROC("vkDebugMarkerSetObjectTagEXT")
#define VK_LOADER_LOAD_vkDebugMarkerSetObjectNameEXT _vkDebugMarkerSetObjectNameEXT = (PFN_vkDebugMarkerSetObjectNameEXT)VK_LOADER_GET_PROC("vkDebugMarkerSetObjectNameEXT")
#define VK_LOADER_LOAD_vkCmdDebugMarkerBeginEXT _vkCmdDebugMarkerBeginEXT = (PFN_vkCmdDebugMarkerBeginEXT)VK_LOADER_GET_PROC("vkCmdDebugMarkerBeginEXT")
#define VK_LOADER_LOAD_vkCmdDebugMarkerEndEXT _vkCmdDebugMarkerEndEXT = (PFN_vkCmdDebugMarkerEndEXT)VK_LOADER_GET_PROC("vkCmdDebugMarkerEndEXT")
#define VK_LOADER_LOAD_vkCmdDebugMarkerInsertEXT _vkCmdDebugMarkerInsertEXT = (PFN_vkCmdDebugMarkerInsertEXT)VK_LOADER_GET_PROC("vkCmdDebugMarkerInsertEXT")

#define VK_LOADER_LOAD_ALL(n) \
	VK_LOADER_LOAD_MODULE(n); \
	if(_vkModule) { \
		VK_LOADER_LOAD_vkCreateInstance; \
		VK_LOADER_LOAD_vkDestroyInstance; \
		VK_LOADER_LOAD_vkEnumeratePhysicalDevices; \
		VK_LOADER_LOAD_vkGetDeviceProcAddr; \
		VK_LOADER_LOAD_vkGetInstanceProcAddr; \
		VK_LOADER_LOAD_vkGetPhysicalDeviceProperties; \
		VK_LOADER_LOAD_vkGetPhysicalDeviceQueueFamilyProperties; \
		VK_LOADER_LOAD_vkGetPhysicalDeviceMemoryProperties; \
		VK_LOADER_LOAD_vkGetPhysicalDeviceFeatures; \
		VK_LOADER_LOAD_vkGetPhysicalDeviceFormatProperties; \
		VK_LOADER_LOAD_vkGetPhysicalDeviceImageFormatProperties; \
		VK_LOADER_LOAD_vkCreateDevice; \
		VK_LOADER_LOAD_vkDestroyDevice; \
		VK_LOADER_LOAD_vkEnumerateInstanceLayerProperties; \
		VK_LOADER_LOAD_vkEnumerateInstanceExtensionProperties; \
		VK_LOADER_LOAD_vkEnumerateDeviceLayerProperties; \
		VK_LOADER_LOAD_vkEnumerateDeviceExtensionProperties; \
		VK_LOADER_LOAD_vkGetDeviceQueue; \
		VK_LOADER_LOAD_vkQueueSubmit; \
		VK_LOADER_LOAD_vkQueueWaitIdle; \
		VK_LOADER_LOAD_vkDeviceWaitIdle; \
		VK_LOADER_LOAD_vkAllocateMemory; \
		VK_LOADER_LOAD_vkFreeMemory; \
		VK_LOADER_LOAD_vkMapMemory; \
		VK_LOADER_LOAD_vkUnmapMemory; \
		VK_LOADER_LOAD_vkFlushMappedMemoryRanges; \
		VK_LOADER_LOAD_vkInvalidateMappedMemoryRanges; \
		VK_LOADER_LOAD_vkGetDeviceMemoryCommitment; \
		VK_LOADER_LOAD_vkGetBufferMemoryRequirements; \
		VK_LOADER_LOAD_vkBindBufferMemory; \
		VK_LOADER_LOAD_vkGetImageMemoryRequirements; \
		VK_LOADER_LOAD_vkBindImageMemory; \
		VK_LOADER_LOAD_vkGetImageSparseMemoryRequirements; \
		VK_LOADER_LOAD_vkGetPhysicalDeviceSparseImageFormatProperties; \
		VK_LOADER_LOAD_vkQueueBindSparse; \
		VK_LOADER_LOAD_vkCreateFence; \
		VK_LOADER_LOAD_vkDestroyFence; \
		VK_LOADER_LOAD_vkResetFences; \
		VK_LOADER_LOAD_vkGetFenceStatus; \
		VK_LOADER_LOAD_vkWaitForFences; \
		VK_LOADER_LOAD_vkCreateSemaphore; \
		VK_LOADER_LOAD_vkDestroySemaphore; \
		VK_LOADER_LOAD_vkCreateEvent; \
		VK_LOADER_LOAD_vkDestroyEvent; \
		VK_LOADER_LOAD_vkGetEventStatus; \
		VK_LOADER_LOAD_vkSetEvent; \
		VK_LOADER_LOAD_vkResetEvent; \
		VK_LOADER_LOAD_vkCreateQueryPool; \
		VK_LOADER_LOAD_vkDestroyQueryPool; \
		VK_LOADER_LOAD_vkGetQueryPoolResults; \
		VK_LOADER_LOAD_vkCreateBuffer; \
		VK_LOADER_LOAD_vkDestroyBuffer; \
		VK_LOADER_LOAD_vkCreateBufferView; \
		VK_LOADER_LOAD_vkDestroyBufferView; \
		VK_LOADER_LOAD_vkCreateImage; \
		VK_LOADER_LOAD_vkDestroyImage; \
		VK_LOADER_LOAD_vkGetImageSubresourceLayout; \
		VK_LOADER_LOAD_vkCreateImageView; \
		VK_LOADER_LOAD_vkDestroyImageView; \
		VK_LOADER_LOAD_vkCreateShaderModule; \
		VK_LOADER_LOAD_vkDestroyShaderModule; \
		VK_LOADER_LOAD_vkCreatePipelineCache; \
		VK_LOADER_LOAD_vkDestroyPipelineCache; \
		VK_LOADER_LOAD_vkGetPipelineCacheData; \
		VK_LOADER_LOAD_vkMergePipelineCaches; \
		VK_LOADER_LOAD_vkCreateGraphicsPipelines; \
		VK_LOADER_LOAD_vkCreateComputePipelines; \
		VK_LOADER_LOAD_vkDestroyPipeline; \
		VK_LOADER_LOAD_vkCreatePipelineLayout; \
		VK_LOADER_LOAD_vkDestroyPipelineLayout; \
		VK_LOADER_LOAD_vkCreateSampler; \
		VK_LOADER_LOAD_vkDestroySampler; \
		VK_LOADER_LOAD_vkCreateDescriptorSetLayout; \
		VK_LOADER_LOAD_vkDestroyDescriptorSetLayout; \
		VK_LOADER_LOAD_vkCreateDescriptorPool; \
		VK_LOADER_LOAD_vkDestroyDescriptorPool; \
		VK_LOADER_LOAD_vkResetDescriptorPool; \
		VK_LOADER_LOAD_vkAllocateDescriptorSets; \
		VK_LOADER_LOAD_vkFreeDescriptorSets; \
		VK_LOADER_LOAD_vkUpdateDescriptorSets; \
		VK_LOADER_LOAD_vkCreateFramebuffer; \
		VK_LOADER_LOAD_vkDestroyFramebuffer; \
		VK_LOADER_LOAD_vkCreateRenderPass; \
		VK_LOADER_LOAD_vkDestroyRenderPass; \
		VK_LOADER_LOAD_vkGetRenderAreaGranularity; \
		VK_LOADER_LOAD_vkCreateCommandPool; \
		VK_LOADER_LOAD_vkDestroyCommandPool; \
		VK_LOADER_LOAD_vkResetCommandPool; \
		VK_LOADER_LOAD_vkAllocateCommandBuffers; \
		VK_LOADER_LOAD_vkFreeCommandBuffers; \
		VK_LOADER_LOAD_vkBeginCommandBuffer; \
		VK_LOADER_LOAD_vkEndCommandBuffer; \
		VK_LOADER_LOAD_vkResetCommandBuffer; \
		VK_LOADER_LOAD_vkCmdBindPipeline; \
		VK_LOADER_LOAD_vkCmdSetViewport; \
		VK_LOADER_LOAD_vkCmdSetScissor; \
		VK_LOADER_LOAD_vkCmdSetLineWidth; \
		VK_LOADER_LOAD_vkCmdSetDepthBias; \
		VK_LOADER_LOAD_vkCmdSetBlendConstants; \
		VK_LOADER_LOAD_vkCmdSetDepthBounds; \
		VK_LOADER_LOAD_vkCmdSetStencilCompareMask; \
		VK_LOADER_LOAD_vkCmdSetStencilWriteMask; \
		VK_LOADER_LOAD_vkCmdSetStencilReference; \
		VK_LOADER_LOAD_vkCmdBindDescriptorSets; \
		VK_LOADER_LOAD_vkCmdBindIndexBuffer; \
		VK_LOADER_LOAD_vkCmdBindVertexBuffers; \
		VK_LOADER_LOAD_vkCmdDraw; \
		VK_LOADER_LOAD_vkCmdDrawIndexed; \
		VK_LOADER_LOAD_vkCmdDrawIndirect; \
		VK_LOADER_LOAD_vkCmdDrawIndexedIndirect; \
		VK_LOADER_LOAD_vkCmdDispatch; \
		VK_LOADER_LOAD_vkCmdDispatchIndirect; \
		VK_LOADER_LOAD_vkCmdCopyBuffer; \
		VK_LOADER_LOAD_vkCmdCopyImage; \
		VK_LOADER_LOAD_vkCmdBlitImage; \
		VK_LOADER_LOAD_vkCmdCopyBufferToImage; \
		VK_LOADER_LOAD_vkCmdCopyImageToBuffer; \
		VK_LOADER_LOAD_vkCmdUpdateBuffer; \
		VK_LOADER_LOAD_vkCmdFillBuffer; \
		VK_LOADER_LOAD_vkCmdClearColorImage; \
		VK_LOADER_LOAD_vkCmdClearDepthStencilImage; \
		VK_LOADER_LOAD_vkCmdClearAttachments; \
		VK_LOADER_LOAD_vkCmdResolveImage; \
		VK_LOADER_LOAD_vkCmdSetEvent; \
		VK_LOADER_LOAD_vkCmdResetEvent; \
		VK_LOADER_LOAD_vkCmdWaitEvents; \
		VK_LOADER_LOAD_vkCmdPipelineBarrier; \
		VK_LOADER_LOAD_vkCmdBeginQuery; \
		VK_LOADER_LOAD_vkCmdEndQuery; \
		VK_LOADER_LOAD_vkCmdResetQueryPool; \
		VK_LOADER_LOAD_vkCmdWriteTimestamp; \
		VK_LOADER_LOAD_vkCmdCopyQueryPoolResults; \
		VK_LOADER_LOAD_vkCmdPushConstants; \
		VK_LOADER_LOAD_vkCmdBeginRenderPass; \
		VK_LOADER_LOAD_vkCmdNextSubpass; \
		VK_LOADER_LOAD_vkCmdEndRenderPass; \
		VK_LOADER_LOAD_vkCmdExecuteCommands; \
		VK_LOADER_LOAD_vkDestroySurfaceKHR; \
		VK_LOADER_LOAD_vkGetPhysicalDeviceSurfaceSupportKHR; \
		VK_LOADER_LOAD_vkGetPhysicalDeviceSurfaceCapabilitiesKHR; \
		VK_LOADER_LOAD_vkGetPhysicalDeviceSurfaceFormatsKHR; \
		VK_LOADER_LOAD_vkGetPhysicalDeviceSurfacePresentModesKHR; \
		VK_LOADER_LOAD_vkCreateSwapchainKHR; \
		VK_LOADER_LOAD_vkDestroySwapchainKHR; \
		VK_LOADER_LOAD_vkGetSwapchainImagesKHR; \
		VK_LOADER_LOAD_vkAcquireNextImageKHR; \
		VK_LOADER_LOAD_vkQueuePresentKHR; \
		VK_LOADER_LOAD_vkGetPhysicalDeviceDisplayPropertiesKHR; \
		VK_LOADER_LOAD_vkGetPhysicalDeviceDisplayPlanePropertiesKHR; \
		VK_LOADER_LOAD_vkGetDisplayPlaneSupportedDisplaysKHR; \
		VK_LOADER_LOAD_vkGetDisplayModePropertiesKHR; \
		VK_LOADER_LOAD_vkCreateDisplayModeKHR; \
		VK_LOADER_LOAD_vkGetDisplayPlaneCapabilitiesKHR; \
		VK_LOADER_LOAD_vkCreateDisplayPlaneSurfaceKHR; \
		VK_LOADER_LOAD_vkCreateSharedSwapchainsKHR; \
		VK_LOADER_LOAD_vkCreateXlibSurfaceKHR; \
		VK_LOADER_LOAD_vkGetPhysicalDeviceXlibPresentationSupportKHR; \
		VK_LOADER_LOAD_vkCreateXcbSurfaceKHR; \
		VK_LOADER_LOAD_vkGetPhysicalDeviceXcbPresentationSupportKHR; \
		VK_LOADER_LOAD_vkCreateWaylandSurfaceKHR; \
		VK_LOADER_LOAD_vkGetPhysicalDeviceWaylandPresentationSupportKHR; \
		VK_LOADER_LOAD_vkCreateMirSurfaceKHR; \
		VK_LOADER_LOAD_vkGetPhysicalDeviceMirPresentationSupportKHR; \
		VK_LOADER_LOAD_vkCreateAndroidSurfaceKHR; \
		VK_LOADER_LOAD_vkCreateWin32SurfaceKHR; \
		VK_LOADER_LOAD_vkGetPhysicalDeviceWin32PresentationSupportKHR; \
		VK_LOADER_LOAD_vkCreateDebugReportCallbackEXT; \
		VK_LOADER_LOAD_vkDestroyDebugReportCallbackEXT; \
		VK_LOADER_LOAD_vkDebugReportMessageEXT; \
		VK_LOADER_LOAD_vkDebugMarkerSetObjectTagEXT; \
		VK_LOADER_LOAD_vkDebugMarkerSetObjectNameEXT; \
		VK_LOADER_LOAD_vkCmdDebugMarkerBeginEXT; \
		VK_LOADER_LOAD_vkCmdDebugMarkerEndEXT; \
		VK_LOADER_LOAD_vkCmdDebugMarkerInsertEXT; \
	}

#define VK_LOADER_UNLOAD_ALL() \
	VK_LOADER_UNLOAD_MODULE(); \
	_vkCreateInstance = NULL; \
	_vkDestroyInstance = NULL; \
	_vkEnumeratePhysicalDevices = NULL; \
	_vkGetDeviceProcAddr = NULL; \
	_vkGetInstanceProcAddr = NULL; \
	_vkGetPhysicalDeviceProperties = NULL; \
	_vkGetPhysicalDeviceQueueFamilyProperties = NULL; \
	_vkGetPhysicalDeviceMemoryProperties = NULL; \
	_vkGetPhysicalDeviceFeatures = NULL; \
	_vkGetPhysicalDeviceFormatProperties = NULL; \
	_vkGetPhysicalDeviceImageFormatProperties = NULL; \
	_vkCreateDevice = NULL; \
	_vkDestroyDevice = NULL; \
	_vkEnumerateInstanceLayerProperties = NULL; \
	_vkEnumerateInstanceExtensionProperties = NULL; \
	_vkEnumerateDeviceLayerProperties = NULL; \
	_vkEnumerateDeviceExtensionProperties = NULL; \
	_vkGetDeviceQueue = NULL; \
	_vkQueueSubmit = NULL; \
	_vkQueueWaitIdle = NULL; \
	_vkDeviceWaitIdle = NULL; \
	_vkAllocateMemory = NULL; \
	_vkFreeMemory = NULL; \
	_vkMapMemory = NULL; \
	_vkUnmapMemory = NULL; \
	_vkFlushMappedMemoryRanges = NULL; \
	_vkInvalidateMappedMemoryRanges = NULL; \
	_vkGetDeviceMemoryCommitment = NULL; \
	_vkGetBufferMemoryRequirements = NULL; \
	_vkBindBufferMemory = NULL; \
	_vkGetImageMemoryRequirements = NULL; \
	_vkBindImageMemory = NULL; \
	_vkGetImageSparseMemoryRequirements = NULL; \
	_vkGetPhysicalDeviceSparseImageFormatProperties = NULL; \
	_vkQueueBindSparse = NULL; \
	_vkCreateFence = NULL; \
	_vkDestroyFence = NULL; \
	_vkResetFences = NULL; \
	_vkGetFenceStatus = NULL; \
	_vkWaitForFences = NULL; \
	_vkCreateSemaphore = NULL; \
	_vkDestroySemaphore = NULL; \
	_vkCreateEvent = NULL; \
	_vkDestroyEvent = NULL; \
	_vkGetEventStatus = NULL; \
	_vkSetEvent = NULL; \
	_vkResetEvent = NULL; \
	_vkCreateQueryPool = NULL; \
	_vkDestroyQueryPool = NULL; \
	_vkGetQueryPoolResults = NULL; \
	_vkCreateBuffer = NULL; \
	_vkDestroyBuffer = NULL; \
	_vkCreateBufferView = NULL; \
	_vkDestroyBufferView = NULL; \
	_vkCreateImage = NULL; \
	_vkDestroyImage = NULL; \
	_vkGetImageSubresourceLayout = NULL; \
	_vkCreateImageView = NULL; \
	_vkDestroyImageView = NULL; \
	_vkCreateShaderModule = NULL; \
	_vkDestroyShaderModule = NULL; \
	_vkCreatePipelineCache = NULL; \
	_vkDestroyPipelineCache = NULL; \
	_vkGetPipelineCacheData = NULL; \
	_vkMergePipelineCaches = NULL; \
	_vkCreateGraphicsPipelines = NULL; \
	_vkCreateComputePipelines = NULL; \
	_vkDestroyPipeline = NULL; \
	_vkCreatePipelineLayout = NULL; \
	_vkDestroyPipelineLayout = NULL; \
	_vkCreateSampler = NULL; \
	_vkDestroySampler = NULL; \
	_vkCreateDescriptorSetLayout = NULL; \
	_vkDestroyDescriptorSetLayout = NULL; \
	_vkCreateDescriptorPool = NULL; \
	_vkDestroyDescriptorPool = NULL; \
	_vkResetDescriptorPool = NULL; \
	_vkAllocateDescriptorSets = NULL; \
	_vkFreeDescriptorSets = NULL; \
	_vkUpdateDescriptorSets = NULL; \
	_vkCreateFramebuffer = NULL; \
	_vkDestroyFramebuffer = NULL; \
	_vkCreateRenderPass = NULL; \
	_vkDestroyRenderPass = NULL; \
	_vkGetRenderAreaGranularity = NULL; \
	_vkCreateCommandPool = NULL; \
	_vkDestroyCommandPool = NULL; \
	_vkResetCommandPool = NULL; \
	_vkAllocateCommandBuffers = NULL; \
	_vkFreeCommandBuffers = NULL; \
	_vkBeginCommandBuffer = NULL; \
	_vkEndCommandBuffer = NULL; \
	_vkResetCommandBuffer = NULL; \
	_vkCmdBindPipeline = NULL; \
	_vkCmdSetViewport = NULL; \
	_vkCmdSetScissor = NULL; \
	_vkCmdSetLineWidth = NULL; \
	_vkCmdSetDepthBias = NULL; \
	_vkCmdSetBlendConstants = NULL; \
	_vkCmdSetDepthBounds = NULL; \
	_vkCmdSetStencilCompareMask = NULL; \
	_vkCmdSetStencilWriteMask = NULL; \
	_vkCmdSetStencilReference = NULL; \
	_vkCmdBindDescriptorSets = NULL; \
	_vkCmdBindIndexBuffer = NULL; \
	_vkCmdBindVertexBuffers = NULL; \
	_vkCmdDraw = NULL; \
	_vkCmdDrawIndexed = NULL; \
	_vkCmdDrawIndirect = NULL; \
	_vkCmdDrawIndexedIndirect = NULL; \
	_vkCmdDispatch = NULL; \
	_vkCmdDispatchIndirect = NULL; \
	_vkCmdCopyBuffer = NULL; \
	_vkCmdCopyImage = NULL; \
	_vkCmdBlitImage = NULL; \
	_vkCmdCopyBufferToImage = NULL; \
	_vkCmdCopyImageToBuffer = NULL; \
	_vkCmdUpdateBuffer = NULL; \
	_vkCmdFillBuffer = NULL; \
	_vkCmdClearColorImage = NULL; \
	_vkCmdClearDepthStencilImage = NULL; \
	_vkCmdClearAttachments = NULL; \
	_vkCmdResolveImage = NULL; \
	_vkCmdSetEvent = NULL; \
	_vkCmdResetEvent = NULL; \
	_vkCmdWaitEvents = NULL; \
	_vkCmdPipelineBarrier = NULL; \
	_vkCmdBeginQuery = NULL; \
	_vkCmdEndQuery = NULL; \
	_vkCmdResetQueryPool = NULL; \
	_vkCmdWriteTimestamp = NULL; \
	_vkCmdCopyQueryPoolResults = NULL; \
	_vkCmdPushConstants = NULL; \
	_vkCmdBeginRenderPass = NULL; \
	_vkCmdNextSubpass = NULL; \
	_vkCmdEndRenderPass = NULL; \
	_vkCmdExecuteCommands = NULL; \
	_vkDestroySurfaceKHR = NULL; \
	_vkGetPhysicalDeviceSurfaceSupportKHR = NULL; \
	_vkGetPhysicalDeviceSurfaceCapabilitiesKHR = NULL; \
	_vkGetPhysicalDeviceSurfaceFormatsKHR = NULL; \
	_vkGetPhysicalDeviceSurfacePresentModesKHR = NULL; \
	_vkCreateSwapchainKHR = NULL; \
	_vkDestroySwapchainKHR = NULL; \
	_vkGetSwapchainImagesKHR = NULL; \
	_vkAcquireNextImageKHR = NULL; \
	_vkQueuePresentKHR = NULL; \
	_vkGetPhysicalDeviceDisplayPropertiesKHR = NULL; \
	_vkGetPhysicalDeviceDisplayPlanePropertiesKHR = NULL; \
	_vkGetDisplayPlaneSupportedDisplaysKHR = NULL; \
	_vkGetDisplayModePropertiesKHR = NULL; \
	_vkCreateDisplayModeKHR = NULL; \
	_vkGetDisplayPlaneCapabilitiesKHR = NULL; \
	_vkCreateDisplayPlaneSurfaceKHR = NULL; \
	_vkCreateSharedSwapchainsKHR = NULL; \
	VK_LOADER_UNLOAD_vkCreateXlibSurfaceKHR; \
	VK_LOADER_UNLOAD_vkGetPhysicalDeviceXlibPresentationSupportKHR; \
	VK_LOADER_UNLOAD_vkCreateXcbSurfaceKHR; \
	VK_LOADER_UNLOAD_vkGetPhysicalDeviceXcbPresentationSupportKHR; \
	VK_LOADER_UNLOAD_vkCreateWaylandSurfaceKHR; \
	VK_LOADER_UNLOAD_vkGetPhysicalDeviceWaylandPresentationSupportKHR; \
	VK_LOADER_UNLOAD_vkCreateMirSurfaceKHR; \
	VK_LOADER_UNLOAD_vkGetPhysicalDeviceMirPresentationSupportKHR; \
	VK_LOADER_UNLOAD_vkCreateAndroidSurfaceKHR; \
	VK_LOADER_UNLOAD_vkCreateWin32SurfaceKHR; \
	VK_LOADER_UNLOAD_vkGetPhysicalDeviceWin32PresentationSupportKHR; \
	_vkCreateDebugReportCallbackEXT = NULL; \
	_vkDestroyDebugReportCallbackEXT = NULL; \
	_vkDebugReportMessageEXT = NULL; \
	_vkDebugMarkerSetObjectTagEXT = NULL; \
	_vkDebugMarkerSetObjectNameEXT = NULL; \
	_vkCmdDebugMarkerBeginEXT = NULL; \
	_vkCmdDebugMarkerEndEXT = NULL; \
	_vkCmdDebugMarkerInsertEXT = NULL;

#endif
