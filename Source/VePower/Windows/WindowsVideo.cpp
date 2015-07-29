////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   WindowsVideo.cpp
//  Created:     2015/07/27 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "../VePowerPch.h"
#include "WindowsVideo.h"

//--------------------------------------------------------------------------
VeVideoDevicePtr CreateVideoDevice() noexcept
{
	return VE_NEW WindowsVideoDevice();
}
//--------------------------------------------------------------------------
WindowsVideoDevice::WindowsVideoDevice() noexcept
{
	m_kName = "windows";
	m_kDesc = "Venus3D Windows Video Driver";
}
//--------------------------------------------------------------------------
WindowsVideoDevice::~WindowsVideoDevice() noexcept
{

}
//--------------------------------------------------------------------------
void WindowsVideoDevice::Init() noexcept
{
	InitModes();
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::Term() noexcept
{
	TermModes();
}
//--------------------------------------------------------------------------
static bool GetDisplayMode(LPCSTR deviceName, DWORD index,
	VeDisplayMode* mode) noexcept
{
	VeDisplayModeData* data;
	DEVMODEA devmode;
	HDC hdc;

	devmode.dmSize = sizeof(devmode);
	devmode.dmDriverExtra = 0;
	if (!EnumDisplaySettingsA(deviceName, index, &devmode))
	{
		return false;
	}

	data = VE_NEW VeDisplayModeData;
	VE_ASSERT(data);
	data->DeviceMode = devmode;
	data->DeviceMode.dmFields =
		(DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY |
			DM_DISPLAYFLAGS);
	data->ScaleX = 1.0f;
	data->ScaleY = 1.0f;

	mode->m_u32Format = VE_PIXELFORMAT_UNKNOWN;
	mode->m_i32Width = devmode.dmPelsWidth;
	mode->m_i32Height = devmode.dmPelsHeight;
	mode->m_i32RefreshRate = devmode.dmDisplayFrequency;
	mode->m_spDriverData = data;

	if (index == ENUM_CURRENT_SETTINGS
		&& (hdc = CreateDCA(deviceName, NULL, NULL, NULL)) != NULL)
	{
		char bmi_data[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)];
		LPBITMAPINFO bmi;
		HBITMAP hbm;
		int logical_width = GetDeviceCaps(hdc, HORZRES);
		int logical_height = GetDeviceCaps(hdc, VERTRES);

		data->ScaleX = (float)logical_width / devmode.dmPelsWidth;
		data->ScaleY = (float)logical_height / devmode.dmPelsHeight;
		mode->m_i32Width = logical_width;
		mode->m_i32Height = logical_height;

		VeZeroMemory(bmi_data, sizeof(bmi_data));
		bmi = (LPBITMAPINFO)bmi_data;
		bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

		hbm = CreateCompatibleBitmap(hdc, 1, 1);
		GetDIBits(hdc, hbm, 0, 1, NULL, bmi, DIB_RGB_COLORS);
		GetDIBits(hdc, hbm, 0, 1, NULL, bmi, DIB_RGB_COLORS);
		DeleteObject(hbm);
		DeleteDC(hdc);
		if (bmi->bmiHeader.biCompression == BI_BITFIELDS)
		{
			switch (*(VeUInt32 *)bmi->bmiColors)
			{
			case 0x00FF0000:
				mode->m_u32Format = VE_PIXELFORMAT_RGB888;
				break;
			case 0x000000FF:
				mode->m_u32Format = VE_PIXELFORMAT_BGR888;
				break;
			case 0xF800:
				mode->m_u32Format = VE_PIXELFORMAT_RGB565;
				break;
			case 0x7C00:
				mode->m_u32Format = VE_PIXELFORMAT_RGB555;
				break;
			}
		}
		else if (bmi->bmiHeader.biBitCount == 8)
		{
			mode->m_u32Format = VE_PIXELFORMAT_INDEX8;
		}
		else if (bmi->bmiHeader.biBitCount == 4)
		{
			mode->m_u32Format = VE_PIXELFORMAT_INDEX4LSB;
		}
	}
	else
	{
		if ((devmode.dmFields & DM_BITSPERPEL) == DM_BITSPERPEL)
		{
			switch (devmode.dmBitsPerPel)
			{
			case 32:
				mode->m_u32Format = VE_PIXELFORMAT_RGB888;
				break;
			case 24:
				mode->m_u32Format = VE_PIXELFORMAT_RGB24;
				break;
			case 16:
				mode->m_u32Format = VE_PIXELFORMAT_RGB565;
				break;
			case 15:
				mode->m_u32Format = VE_PIXELFORMAT_RGB555;
				break;
			case 8:
				mode->m_u32Format = VE_PIXELFORMAT_INDEX8;
				break;
			case 4:
				mode->m_u32Format = VE_PIXELFORMAT_INDEX4LSB;
				break;
			}
		}
	}
	return true;
}
//--------------------------------------------------------------------------
bool WindowsVideoDevice::AddDisplay(const VeChar8* pcDeviceName) noexcept
{
	VeDebugOutputCore("Display: %s\n", pcDeviceName);

	VeDisplayMode kMode;
	
	if (!GetDisplayMode(pcDeviceName, ENUM_CURRENT_SETTINGS, &kMode))
	{
		return false;
	}

	m_kDisplayList.resize(m_kDisplayList.size() + 1);
	VeVideoDisplay& kDisplay = m_kDisplayList.back();

	VeDisplayData* pkDisplayData = VE_NEW VeDisplayData;
	VE_ASSERT(pkDisplayData);
	DISPLAY_DEVICEA device;
	device.cb = sizeof(device);
	if (EnumDisplayDevicesA(pcDeviceName, 0, &device, 0))
	{
		kDisplay.m_kName = device.DeviceString;
	}

	kDisplay.m_kDesktopMode = kMode;
	kDisplay.m_kCurrentMode = kMode;
	kDisplay.m_spDriverData = pkDisplayData;
	return true;
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::InitModes() noexcept
{
	VeInt32 pass;
	DWORD i, j, count;
	DISPLAY_DEVICEA device;

	device.cb = sizeof(device);

	for (pass = 0; pass < 2; ++pass)
	{
		for (i = 0;; ++i)
		{
			CHAR DeviceName[32];

			if (!EnumDisplayDevicesA(NULL, i, &device, 0))
			{
				break;
			}
			if (!(device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))
			{
				continue;
			}
			if (pass == 0)
			{
				if (!(device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE))
				{
					continue;
				}
			}
			else
			{
				if (device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
				{
					continue;
				}
			}
			VeMemoryCopy(DeviceName, device.DeviceName, sizeof(DeviceName));

			VeDebugOutputCore("Device: %s\n", DeviceName);

			count = 0;
			for (j = 0;; ++j)
			{
				if (!EnumDisplayDevicesA(DeviceName, j, &device, 0))
				{
					break;
				}
				if (!(device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP))
				{
					continue;
				}
				if (pass == 0)
				{
					if (!(device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE))
					{
						continue;
					}
				}
				else {
					if (device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
					{
						continue;
					}
				}
				count += AddDisplay(device.DeviceName);
			}
			if (count == 0)
			{
				AddDisplay(DeviceName);
			}
		}
	}

	VE_ASSERT(m_kDisplayList.size());
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::TermModes() noexcept
{
	m_kDisplayList.clear();
}
//--------------------------------------------------------------------------
bool WindowsVideoDevice::GetDisplayBounds(VeRect* pkRect,
	VeVideoDisplay* pkDisplay) noexcept
{
	VE_ASSERT(pkDisplay);
	VeDisplayModeData* pkData = (VeDisplayModeData*)pkDisplay->m_kCurrentMode.m_spDriverData;

	pkRect->x = (VeInt32)VeCeilf(pkData->DeviceMode.dmPosition.x * pkData->ScaleX);
	pkRect->y = (VeInt32)VeCeilf(pkData->DeviceMode.dmPosition.y * pkData->ScaleY);
	pkRect->w = (VeInt32)VeCeilf(pkData->DeviceMode.dmPelsWidth * pkData->ScaleX);
	pkRect->h = (VeInt32)VeCeilf(pkData->DeviceMode.dmPelsHeight * pkData->ScaleY);
	return true;
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::GetDisplayModes(VeVideoDisplay* pkDisplay) noexcept
{
	VeDisplayData* data = (VeDisplayData*)pkDisplay->m_spDriverData;
	VeDisplayMode mode;

	for (VeUInt32 i = 0;; ++i)
	{
		if (!GetDisplayMode(data->DeviceName, i, &mode))
		{
			break;
		}
		if (VE_ISPIXELFORMAT_INDEXED(mode.m_u32Format))
		{
			continue;
		}
		if (mode.m_u32Format != VE_PIXELFORMAT_UNKNOWN)
		{
			pkDisplay->m_kDisplayModes.push_back(mode);
		}
	}
}
//--------------------------------------------------------------------------
bool WindowsVideoDevice::SetDisplayMode(VeVideoDisplay* pkDisplay,
	VeDisplayMode* pkMode) noexcept
{
	VeDisplayData* pkDisplayData = (VeDisplayData*)pkDisplay->m_spDriverData;
	VeDisplayModeData* pkData = (VeDisplayModeData*)pkMode->m_spDriverData;
	LONG status;

	status = ChangeDisplaySettingsExA(pkDisplayData->DeviceName,
		&pkData->DeviceMode, NULL, CDS_FULLSCREEN, NULL);
	if (status != DISP_CHANGE_SUCCESSFUL)
	{
		const VeChar8* pcReason = "Unknown reason";
		switch (status) {
		case DISP_CHANGE_BADFLAGS:
			pcReason = "DISP_CHANGE_BADFLAGS";
			break;
		case DISP_CHANGE_BADMODE:
			pcReason = "DISP_CHANGE_BADMODE";
			break;
		case DISP_CHANGE_BADPARAM:
			pcReason = "DISP_CHANGE_BADPARAM";
			break;
		case DISP_CHANGE_FAILED:
			pcReason = "DISP_CHANGE_FAILED";
			break;
		}
		VeDebugOutputCore("ChangeDisplaySettingsEx() failed: %s", pcReason);
		return false;
	}
	EnumDisplaySettingsA(pkDisplayData->DeviceName, ENUM_CURRENT_SETTINGS, &pkData->DeviceMode);
	return true;
}
//--------------------------------------------------------------------------
