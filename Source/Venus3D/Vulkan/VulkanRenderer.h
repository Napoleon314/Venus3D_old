////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Albert D Yang
// -------------------------------------------------------------------------
//  Module:      Vulkan
//  File name:   VulkanRenderer.h
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

#pragma once

#ifdef VE_ENABLE_VULKAN

#if defined(BUILD_PLATFORM_WIN)
#define LIB_VULKAN "vulkan-1.dll"
#elif defined(BUILD_PLATFORM_LINUX)
#define LIB_VULKAN "libvulkan.so"
#endif

class VulkanRenderWindow;

class VulkanRenderer : public VeRenderer
{
	VeNoCopy(VulkanRenderer);
	VeRTTIDecl(VulkanRenderer);
public:
	struct Device
	{
		VkPhysicalDevice m_hGPU;
		vtd::vector<VkQueueFamilyProperties> m_kQueueList;
	};

	VulkanRenderer() noexcept;

	virtual ~VulkanRenderer() noexcept;

	virtual void Init() override;

	virtual void Term() override;

	virtual void BeginSyncCopy() noexcept override;

	virtual void EndSyncCopy() noexcept override;

	virtual VeRenderWindowPtr CreateRenderWindow(const VeWindowPtr& spWindow) noexcept override;

protected:
	void InitInstance();

	void TermInstance();

	void InitDevice();

	void TermDevice();

	friend class VulkanRenderWindow;

	VkInstance m_hVulkan = nullptr;
	vtd::vector<Device> m_kDeviceList;

	vtd::intrusive_list<VulkanRenderWindow*> m_kRenderWindowList;

};

#endif
