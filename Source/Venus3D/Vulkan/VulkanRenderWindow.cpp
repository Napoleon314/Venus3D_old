////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Vulkan
//  File name:   VulkanRenderWindow.cpp
//  Created:     2016/07/28 by Albert
//  Description:
// -------------------------------------------------------------------------
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
// -------------------------------------------------------------------------
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
// -------------------------------------------------------------------------
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#ifdef VE_ENABLE_VULKAN
#	if defined(BUILD_PLATFORM_WIN)
#		define VK_USE_PLATFORM_WIN32_KHR
#	elif defined(BUILD_PLATFORM_LINUX)
#		define VK_USE_PLATFORM_XLIB_KHR
#	endif
#	include "vulkan_loader.h"
#endif
#include "VulkanRenderer.h"
#include "VulkanRenderWindow.h"

//--------------------------------------------------------------------------
#ifdef VE_ENABLE_VULKAN
//--------------------------------------------------------------------------
VeRTTIImpl(VulkanRenderWindow, VeRenderWindow);
//--------------------------------------------------------------------------
VulkanRenderWindow::VulkanRenderWindow(const VeWindowPtr& spWindow) noexcept
	: VeRenderWindow(spWindow)
{
	m_kNode._Content = this;
}
//--------------------------------------------------------------------------
VulkanRenderWindow::~VulkanRenderWindow() noexcept
{
	Term();
}
//--------------------------------------------------------------------------
void VulkanRenderWindow::Init(VulkanRenderer& kRenderer) noexcept
{
	if ((!m_kNode.is_attach()) && m_spTargetWindow)
	{
#		ifdef VK_USE_PLATFORM_WIN32_KHR
		VkWin32SurfaceCreateInfoKHR kSurfaceInfo =
		{
			VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			NULL,
			0,
			venus3d.GetInitData().m_hInstance,
			(HWND)(m_spTargetWindow->GetNativeHandle())
		};
		VE_ASSERT_EQ(vkCreateWin32SurfaceKHR(kRenderer.m_hVulkan,
			&kSurfaceInfo, nullptr, &m_hSurface), VK_SUCCESS);

		/*VE_ASSERT(kRenderer.m_kGPUList.size());
		auto& kDevice = kRenderer.m_kGPUList.front();
		VeDyanmicStack<VkBool32> kSupports(kDevice.m_kQueueList.size());
		for (size_t i(0); i < kDevice.m_kQueueList.size(); ++i)
		{
			VE_ASSERT_EQ(vkGetPhysicalDeviceSurfaceSupportKHR(kDevice.m_hGPU,
				(uint32_t)i, m_hSurface, &kSupports[i]), VK_SUCCESS);
		}
		uint32_t u32GraphicsQueueIndex = UINT32_MAX;
		uint32_t u32PresentQueueIndex = UINT32_MAX;
		for (uint32_t i = 0; i < kDevice.m_kQueueList.size(); ++i)
		{
			if ((kDevice.m_kQueueList[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
			{
				if (u32GraphicsQueueIndex == UINT32_MAX)
				{
					u32GraphicsQueueIndex = i;
				}

				if (kSupports[i] == VK_TRUE)
				{
					u32GraphicsQueueIndex = i;
					u32PresentQueueIndex = i;
					break;
				}
			}
		}
		if (u32GraphicsQueueIndex == UINT32_MAX)
		{
			for (uint32_t i = 0; i < kDevice.m_kQueueList.size(); ++i)
			{
				if (kSupports[i] == VK_TRUE)
				{
					u32PresentQueueIndex = i;
					break;
				}
			}
		}*/

#		endif

		m_spTargetWindow->Show();

		kRenderer.m_kRenderWindowList.attach_back(m_kNode);
	}
}
//--------------------------------------------------------------------------
void VulkanRenderWindow::Term() noexcept
{
	if (m_kNode.is_attach())
	{
		VulkanRenderer& kRenderer = *vtd::member_cast(
			&VulkanRenderer::m_kRenderWindowList, m_kNode.get_list());

		vkDestroySurfaceKHR(kRenderer.m_hVulkan, m_hSurface, nullptr);




		m_kNode.detach();
	}
}
//--------------------------------------------------------------------------
bool VulkanRenderWindow::IsValid() noexcept
{
	return VeRenderWindow::IsValid() ? m_kNode.is_attach() : false;
}
//--------------------------------------------------------------------------
void VulkanRenderWindow::Begin() noexcept
{

}
//--------------------------------------------------------------------------
void VulkanRenderWindow::End() noexcept
{

}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
