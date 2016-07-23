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

#include "stdafx.h"
#include "WindowsVideo.h"
#include "WindowsMouse.h"
#include <shellapi.h>

//--------------------------------------------------------------------------
static std::map<HWND, VeWindowData*> s_kWindowDataMap;
//--------------------------------------------------------------------------
static const VeScancode windows_scancode_table[] =
{
	/*	0						1							2							3							4						5							6							7 */
	/*	8						9							A							B							C						D							E							F */
	VE_SCANCODE_UNKNOWN,		VE_SCANCODE_ESCAPE,		VE_SCANCODE_1,				VE_SCANCODE_2,				VE_SCANCODE_3,			VE_SCANCODE_4,				VE_SCANCODE_5,				VE_SCANCODE_6,			/* 0 */
	VE_SCANCODE_7,				VE_SCANCODE_8,				VE_SCANCODE_9,				VE_SCANCODE_0,				VE_SCANCODE_MINUS,		VE_SCANCODE_EQUALS,		VE_SCANCODE_BACKSPACE,		VE_SCANCODE_TAB,		/* 0 */

	VE_SCANCODE_Q,				VE_SCANCODE_W,				VE_SCANCODE_E,				VE_SCANCODE_R,				VE_SCANCODE_T,			VE_SCANCODE_Y,				VE_SCANCODE_U,				VE_SCANCODE_I,			/* 1 */
	VE_SCANCODE_O,				VE_SCANCODE_P,				VE_SCANCODE_LEFTBRACKET,	VE_SCANCODE_RIGHTBRACKET,	VE_SCANCODE_RETURN,	VE_SCANCODE_LCTRL,			VE_SCANCODE_A,				VE_SCANCODE_S,			/* 1 */

	VE_SCANCODE_D,				VE_SCANCODE_F,				VE_SCANCODE_G,				VE_SCANCODE_H,				VE_SCANCODE_J,			VE_SCANCODE_K,				VE_SCANCODE_L,				VE_SCANCODE_SEMICOLON,	/* 2 */
	VE_SCANCODE_APOSTROPHE,	VE_SCANCODE_GRAVE,			VE_SCANCODE_LSHIFT,		VE_SCANCODE_BACKSLASH,		VE_SCANCODE_Z,			VE_SCANCODE_X,				VE_SCANCODE_C,				VE_SCANCODE_V,			/* 2 */

	VE_SCANCODE_B,				VE_SCANCODE_N,				VE_SCANCODE_M,				VE_SCANCODE_COMMA,			VE_SCANCODE_PERIOD,	VE_SCANCODE_SLASH,			VE_SCANCODE_RSHIFT,		VE_SCANCODE_PRINTSCREEN,/* 3 */
	VE_SCANCODE_LALT,			VE_SCANCODE_SPACE,			VE_SCANCODE_CAPSLOCK,		VE_SCANCODE_F1,			VE_SCANCODE_F2,		VE_SCANCODE_F3,			VE_SCANCODE_F4,			VE_SCANCODE_F5,		/* 3 */

	VE_SCANCODE_F6,			VE_SCANCODE_F7,			VE_SCANCODE_F8,			VE_SCANCODE_F9,			VE_SCANCODE_F10,		VE_SCANCODE_NUMLOCKCLEAR,	VE_SCANCODE_SCROLLLOCK,	VE_SCANCODE_HOME,		/* 4 */
	VE_SCANCODE_UP,			VE_SCANCODE_PAGEUP,		VE_SCANCODE_KP_MINUS,		VE_SCANCODE_LEFT,			VE_SCANCODE_KP_5,		VE_SCANCODE_RIGHT,			VE_SCANCODE_KP_PLUS,		VE_SCANCODE_END,		/* 4 */

	VE_SCANCODE_DOWN,			VE_SCANCODE_PAGEDOWN,		VE_SCANCODE_INSERT,		VE_SCANCODE_DELETE,		VE_SCANCODE_UNKNOWN,	VE_SCANCODE_UNKNOWN,		VE_SCANCODE_NONUSBACKSLASH,VE_SCANCODE_F11,		/* 5 */
	VE_SCANCODE_F12,			VE_SCANCODE_PAUSE,			VE_SCANCODE_UNKNOWN,		VE_SCANCODE_LGUI,			VE_SCANCODE_RGUI,		VE_SCANCODE_APPLICATION,	VE_SCANCODE_UNKNOWN,		VE_SCANCODE_UNKNOWN,	/* 5 */

	VE_SCANCODE_UNKNOWN,		VE_SCANCODE_UNKNOWN,		VE_SCANCODE_UNKNOWN,		VE_SCANCODE_UNKNOWN,		VE_SCANCODE_F13,		VE_SCANCODE_F14,			VE_SCANCODE_F15,			VE_SCANCODE_F16,		/* 6 */
	VE_SCANCODE_F17,			VE_SCANCODE_F18,			VE_SCANCODE_F19,			VE_SCANCODE_UNKNOWN,		VE_SCANCODE_UNKNOWN,	VE_SCANCODE_UNKNOWN,		VE_SCANCODE_UNKNOWN,		VE_SCANCODE_UNKNOWN,	/* 6 */

	VE_SCANCODE_UNKNOWN,		VE_SCANCODE_UNKNOWN,		VE_SCANCODE_UNKNOWN,		VE_SCANCODE_UNKNOWN,		VE_SCANCODE_UNKNOWN,	VE_SCANCODE_UNKNOWN,		VE_SCANCODE_UNKNOWN,		VE_SCANCODE_UNKNOWN,	/* 7 */
	VE_SCANCODE_UNKNOWN,		VE_SCANCODE_UNKNOWN,		VE_SCANCODE_UNKNOWN,		VE_SCANCODE_UNKNOWN,		VE_SCANCODE_UNKNOWN,	VE_SCANCODE_UNKNOWN,		VE_SCANCODE_UNKNOWN,		VE_SCANCODE_UNKNOWN	/* 7 */
};
//--------------------------------------------------------------------------
VeScancode WindowsScanCodeToSDLScanCode(LPARAM lParam, WPARAM wParam) noexcept
{
	VeScancode code;
	char bIsExtended;
	int32_t nScanCode = (lParam >> 16) & 0xFF;

	if (nScanCode == 0 || nScanCode == 0x45)
	{
		switch (wParam)
		{
		case VK_CLEAR: return VE_SCANCODE_CLEAR;
		case VK_MODECHANGE: return VE_SCANCODE_MODE;
		case VK_SELECT: return VE_SCANCODE_SELECT;
		case VK_EXECUTE: return VE_SCANCODE_EXECUTE;
		case VK_HELP: return VE_SCANCODE_HELP;
		case VK_PAUSE: return VE_SCANCODE_PAUSE;
		case VK_NUMLOCK: return VE_SCANCODE_NUMLOCKCLEAR;

		case VK_F13: return VE_SCANCODE_F13;
		case VK_F14: return VE_SCANCODE_F14;
		case VK_F15: return VE_SCANCODE_F15;
		case VK_F16: return VE_SCANCODE_F16;
		case VK_F17: return VE_SCANCODE_F17;
		case VK_F18: return VE_SCANCODE_F18;
		case VK_F19: return VE_SCANCODE_F19;
		case VK_F20: return VE_SCANCODE_F20;
		case VK_F21: return VE_SCANCODE_F21;
		case VK_F22: return VE_SCANCODE_F22;
		case VK_F23: return VE_SCANCODE_F23;
		case VK_F24: return VE_SCANCODE_F24;

		case VK_OEM_NEC_EQUAL: return VE_SCANCODE_KP_EQUALS;
		case VK_BROWSER_BACK: return VE_SCANCODE_AC_BACK;
		case VK_BROWSER_FORWARD: return VE_SCANCODE_AC_FORWARD;
		case VK_BROWSER_REFRESH: return VE_SCANCODE_AC_REFRESH;
		case VK_BROWSER_STOP: return VE_SCANCODE_AC_STOP;
		case VK_BROWSER_SEARCH: return VE_SCANCODE_AC_SEARCH;
		case VK_BROWSER_FAVORITES: return VE_SCANCODE_AC_BOOKMARKS;
		case VK_BROWSER_HOME: return VE_SCANCODE_AC_HOME;
		case VK_VOLUME_MUTE: return VE_SCANCODE_AUDIOMUTE;
		case VK_VOLUME_DOWN: return VE_SCANCODE_VOLUMEDOWN;
		case VK_VOLUME_UP: return VE_SCANCODE_VOLUMEUP;

		case VK_MEDIA_NEXT_TRACK: return VE_SCANCODE_AUDIONEXT;
		case VK_MEDIA_PREV_TRACK: return VE_SCANCODE_AUDIOPREV;
		case VK_MEDIA_STOP: return VE_SCANCODE_AUDIOSTOP;
		case VK_MEDIA_PLAY_PAUSE: return VE_SCANCODE_AUDIOPLAY;
		case VK_LAUNCH_MAIL: return VE_SCANCODE_MAIL;
		case VK_LAUNCH_MEDIA_SELECT: return VE_SCANCODE_MEDIASELECT;

		case VK_OEM_102: return VE_SCANCODE_NONUSBACKSLASH;

		case VK_ATTN: return VE_SCANCODE_SYSREQ;
		case VK_CRSEL: return VE_SCANCODE_CRSEL;
		case VK_EXSEL: return VE_SCANCODE_EXSEL;
		case VK_OEM_CLEAR: return VE_SCANCODE_CLEAR;

		case VK_LAUNCH_APP1: return VE_SCANCODE_APP1;
		case VK_LAUNCH_APP2: return VE_SCANCODE_APP2;

		default: return VE_SCANCODE_UNKNOWN;
		}
	}

	if (nScanCode > 127)
		return VE_SCANCODE_UNKNOWN;

	code = windows_scancode_table[nScanCode];

	bIsExtended = (lParam & (1 << 24)) != 0;
	if (!bIsExtended)
	{
		switch (code)
		{
		case VE_SCANCODE_HOME:
			return VE_SCANCODE_KP_7;
		case VE_SCANCODE_UP:
			return VE_SCANCODE_KP_8;
		case VE_SCANCODE_PAGEUP:
			return VE_SCANCODE_KP_9;
		case VE_SCANCODE_LEFT:
			return VE_SCANCODE_KP_4;
		case VE_SCANCODE_RIGHT:
			return VE_SCANCODE_KP_6;
		case VE_SCANCODE_END:
			return VE_SCANCODE_KP_1;
		case VE_SCANCODE_DOWN:
			return VE_SCANCODE_KP_2;
		case VE_SCANCODE_PAGEDOWN:
			return VE_SCANCODE_KP_3;
		case VE_SCANCODE_INSERT:
			return VE_SCANCODE_KP_0;
		case VE_SCANCODE_DELETE:
			return VE_SCANCODE_KP_PERIOD;
		case VE_SCANCODE_PRINTSCREEN:
			return VE_SCANCODE_KP_MULTIPLY;
		default:
			break;
		}
	}
	else
	{
		switch (code)
		{
		case VE_SCANCODE_RETURN:
			return VE_SCANCODE_KP_ENTER;
		case VE_SCANCODE_LALT:
			return VE_SCANCODE_RALT;
		case VE_SCANCODE_LCTRL:
			return VE_SCANCODE_RCTRL;
		case VE_SCANCODE_SLASH:
			return VE_SCANCODE_KP_DIVIDE;
		case VE_SCANCODE_CAPSLOCK:
			return VE_SCANCODE_KP_PLUS;
		default:
			break;
		}
	}

	return code;
}
//--------------------------------------------------------------------------
struct TempTextA
{
	TempTextA(size_t stLen) noexcept
	{
		m_lpstrText = VeStackAlloc(CHAR, stLen);
		assert(m_lpstrText);
	}

	~TempTextA() noexcept
	{
		VeStackFree(m_lpstrText);
	}

	LPSTR m_lpstrText = nullptr;
};
//--------------------------------------------------------------------------
struct TempTextW
{
	TempTextW(size_t stLen) noexcept
	{
		m_lpwstrText = VeStackAlloc(WCHAR, stLen);
		assert(m_lpwstrText);
	}

	~TempTextW() noexcept
	{
		VeStackFree(m_lpwstrText);
	}

	LPWSTR m_lpwstrText = nullptr;
};
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
	assert_eq(RegisterApp(nullptr, 0, nullptr), true);
}
//--------------------------------------------------------------------------
WindowsVideoDevice::~WindowsVideoDevice() noexcept
{
	UnregisterApp();
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::_Init() noexcept
{
	InitModes();
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::_Term() noexcept
{
	TermModes();
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::UpdateClipCursor(VeWindow::Data* pkWindow) noexcept
{
	assert(pkWindow);
	VeWindowData* pkData = (VeWindowData*)pkWindow->m_spDriverdata;

	if (pkData->m_bInTitleClick || pkData->m_bInModalLoop)
	{
		ClipCursor(nullptr);
		return;
	}

	assert(Venus3D::Ref().GetMouse());
	WindowsMouse* pkMouse = (WindowsMouse*)(Venus3D::Ref().GetMouse());

	if ((pkMouse->m_bRelativeMode
		|| (pkWindow->m_u32Flags & VE_WINDOW_INPUT_GRABBED))
		&& (pkWindow->m_u32Flags & VE_WINDOW_INPUT_FOCUS))
	{
		if (pkMouse->m_bRelativeMode && !pkMouse->m_bRelativeModeWarp)
		{
			LONG cx, cy;
			RECT rect;
			GetWindowRect(pkData->m_hWnd, &rect);

			cx = (rect.left + rect.right) >> 1;
			cy = (rect.top + rect.bottom) >> 1;

			rect.left = cx - 1;
			rect.right = cx + 1;
			rect.top = cy - 1;
			rect.bottom = cy + 1;

			ClipCursor(&rect);
		}
		else
		{
			RECT rect;
			if (GetClientRect(pkData->m_hWnd, &rect) && !IsRectEmpty(&rect))
			{
				ClientToScreen(pkData->m_hWnd, (LPPOINT)&rect);
				ClientToScreen(pkData->m_hWnd, (LPPOINT)&rect + 1);
				ClipCursor(&rect);
			}
		}
	}
	else
	{
		ClipCursor(nullptr);
	}
}
//--------------------------------------------------------------------------
#ifndef GET_X_LPARAM
#	define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#	define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
#endif
//--------------------------------------------------------------------------
LRESULT CALLBACK WindowsVideoDevice::WindowProc(HWND hwnd, UINT msg,
	WPARAM wParam, LPARAM lParam) noexcept
{
	LRESULT lReturnCode = -1;

	auto it = s_kWindowDataMap.find(hwnd);
	if (it == s_kWindowDataMap.end())
	{
		return CallWindowProc(DefWindowProc, hwnd, msg, wParam, lParam);
	}

	VeWindowData* pkData = it->second;

	switch (msg)
	{
	case WM_SHOWWINDOW:
		if (wParam)
		{
			((WindowsVideoDevice*)Venus3D::Ref().GetVideoDevice())->SendWindowEvent(
				pkData->m_pkWindow, VE_WINDOWEVENT_SHOWN, 0, 0);
		}
		else
		{
			((WindowsVideoDevice*)Venus3D::Ref().GetVideoDevice())->SendWindowEvent(
				pkData->m_pkWindow, VE_WINDOWEVENT_HIDDEN, 0, 0);
		}
		break;
	case WM_ACTIVATE:
		{
			BOOL minimized;

			minimized = HIWORD(wParam);
			if (!minimized && (LOWORD(wParam) != WA_INACTIVE))
			{
				((WindowsVideoDevice*)Venus3D::Ref().GetVideoDevice())->SendWindowEvent(
					pkData->m_pkWindow, VE_WINDOWEVENT_SHOWN, 0, 0);

				if (Venus3D::Ref().GetKeyboard())
				{
					if (GetKeyboardFocus() != pkData->m_pkWindow)
					{
						Venus3D::Ref().GetKeyboard()->SetFocus(pkData->m_pkWindow);
					}
				}

				WindowsVideoDevice::UpdateClipCursor(pkData->m_pkWindow);
				((WindowsVideoDevice*)Venus3D::Ref().GetVideoDevice())->CheckAsyncMouseRelease(pkData);
			}
			else
			{
				if (Venus3D::Ref().GetKeyboard())
				{
					if (Venus3D::Ref().GetKeyboard()->GetFocus() == VeWindow::Cast(pkData->m_pkWindow))
					{
						Venus3D::Ref().GetKeyboard()->SetFocus(nullptr);
					}
				}
				ClipCursor(nullptr);
			}
		}
		lReturnCode = 0;
		break;
	case WM_MOUSEMOVE:
		{
			WindowsMouse* pkMouse = (WindowsMouse*)(Venus3D::Ref().GetMouse());
			if (!pkMouse->m_bRelativeMode || pkMouse->m_bRelativeModeWarp)
			{
				pkMouse->SendMotion(pkData->m_pkWindow, 0, 0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			}
		}
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	case WM_XBUTTONUP:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_XBUTTONDOWN:
		{
			WindowsMouse* pkMouse = (WindowsMouse*)(Venus3D::Ref().GetMouse());
			if (!pkMouse->m_bRelativeMode || pkMouse->m_bRelativeModeWarp)
			{
				((WindowsVideoDevice*)Venus3D::Ref().GetVideoDevice())->CheckWParamMouseButtons(wParam, pkData);
			}
		}
		break;
	case WM_INPUT:
		{
			WindowsMouse* pkMouse = (WindowsMouse*)(Venus3D::Ref().GetMouse());
			HRAWINPUT hRawInput = (HRAWINPUT)lParam;
			RAWINPUT inp;
			UINT size = sizeof(inp);

			if (!pkMouse->m_bRelativeMode || pkMouse->m_bRelativeModeWarp
				|| pkMouse->m_pkFocus != pkData->m_pkWindow)
			{
				break;
			}

			GetRawInputData(hRawInput, RID_INPUT, &inp, &size, sizeof(RAWINPUTHEADER));

			if (inp.header.dwType == RIM_TYPEMOUSE)
			{
				RAWMOUSE* mouse = &inp.data.mouse;

				if ((mouse->usFlags & 0x01) == MOUSE_MOVE_RELATIVE)
				{
					pkMouse->SendMotion(pkData->m_pkWindow, 0, 1,
						pkMouse->m_i32LastX, pkMouse->m_i32LastY);
				}
				else
				{
					static VePoint initialMousePoint;
					if (initialMousePoint.x == 0 && initialMousePoint.y == 0)
					{
						initialMousePoint.x = pkMouse->m_i32LastX;
						initialMousePoint.y = pkMouse->m_i32LastY;
					}

					pkMouse->SendMotion(pkData->m_pkWindow, 0, 1,
						(pkMouse->m_i32LastX - initialMousePoint.x),
						(pkMouse->m_i32LastY - initialMousePoint.y));

					initialMousePoint.x = mouse->lLastX;
					initialMousePoint.y = mouse->lLastY;
				}
				((WindowsVideoDevice*)Venus3D::Ref().GetVideoDevice())->CheckRawMouseButtons(
					mouse->usButtonFlags, pkData);
			}
		}
		break;
	case WM_MOUSEWHEEL:
		{
			static short s_AccumulatedMotion;
			WindowsMouse* pkMouse = (WindowsMouse*)(Venus3D::Ref().GetMouse());
			s_AccumulatedMotion += GET_WHEEL_DELTA_WPARAM(wParam);
			if (s_AccumulatedMotion > 0)
			{
				while (s_AccumulatedMotion >= WHEEL_DELTA)
				{
					pkMouse->SendWheel(pkData->m_pkWindow, 0, 0, 1);
					s_AccumulatedMotion -= WHEEL_DELTA;
				}
			}
			else
			{
				while (s_AccumulatedMotion <= -WHEEL_DELTA)
				{
					pkMouse->SendWheel(pkData->m_pkWindow, 0, 0, -1);
					s_AccumulatedMotion += WHEEL_DELTA;
				}
			}
		}
		break;
	case WM_MOUSEHWHEEL:
		{
			static short s_AccumulatedMotion;
			WindowsMouse* pkMouse = (WindowsMouse*)(Venus3D::Ref().GetMouse());
			s_AccumulatedMotion += GET_WHEEL_DELTA_WPARAM(wParam);
			if (s_AccumulatedMotion > 0)
			{
				while (s_AccumulatedMotion >= WHEEL_DELTA)
				{
					pkMouse->SendWheel(pkData->m_pkWindow, 0, 1, 0);
					s_AccumulatedMotion -= WHEEL_DELTA;
				}
			}
			else
			{
				while (s_AccumulatedMotion <= -WHEEL_DELTA)
				{
					pkMouse->SendWheel(pkData->m_pkWindow, 0, -1, 0);
					s_AccumulatedMotion += WHEEL_DELTA;
				}
			}
		}
		break;
#	ifdef WM_MOUSELEAVE
	case WM_MOUSELEAVE:
		{
			WindowsMouse* pkMouse = (WindowsMouse*)(Venus3D::Ref().GetMouse());
			if (pkMouse->m_pkFocus == pkData->m_pkWindow && !pkMouse->m_bRelativeMode)
			{
				if (!IsIconic(hwnd))
				{
					POINT cursorPos;
					GetCursorPos(&cursorPos);
					ScreenToClient(hwnd, &cursorPos);
					pkMouse->SendMotion(pkData->m_pkWindow, 0, 0, cursorPos.x, cursorPos.y);
				}
				pkMouse->SetFocus(nullptr);
			}
			lReturnCode = 0;
		}
		break;
#	endif

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		{
			VeScancode code = WindowsScanCodeToSDLScanCode(lParam, wParam);
			if (code != VE_SCANCODE_UNKNOWN)
			{
				Venus3D::Ref().GetKeyboard()->SendKey(VE_PRESSED, code);
			}
		}
		/*if (msg == WM_KEYDOWN)
		{
			BYTE keyboardState[256];
			char text[5];
			UINT32 utf32 = 0;

			GetKeyboardState(keyboardState);
			if (ToUnicode(wParam, (lParam >> 16) & 0xff, keyboardState,
				(LPWSTR)&utf32, 1, 0) > 0)
			{
				WORD repetition;
				for (repetition = lParam & 0xffff; repetition > 0; repetition--) {
					WIN_ConvertUTF32toUTF8(utf32, text);
					ve_keyboard_ptr->SendText(text);
				}
			}
		}*/
		lReturnCode = 0;
		break;

	case WM_SYSKEYUP:
	case WM_KEYUP:
		{
			VeScancode code = WindowsScanCodeToSDLScanCode(lParam, wParam);
			const uint8_t* keyboardState = Venus3D::Ref().GetKeyboard()->GetState();

			if (keyboardState[VE_SCANCODE_LALT] == VE_PRESSED
				|| keyboardState[VE_SCANCODE_RALT] == VE_PRESSED)
			{
				if (code == VE_SCANCODE_F4)
				{
					((WindowsVideoDevice*)Venus3D::Ref().GetVideoDevice())->SendWindowEvent(
						pkData->m_pkWindow, VE_WINDOWEVENT_CLOSE, 0, 0);
				}
			}

			if (code != VE_SCANCODE_UNKNOWN)
			{
				if (code == VE_SCANCODE_PRINTSCREEN &&
					keyboardState[code] == VE_RELEASED)
				{
					Venus3D::Ref().GetKeyboard()->SendKey(VE_PRESSED, code);
				}
				Venus3D::Ref().GetKeyboard()->SendKey(VE_RELEASED, code);
			}
		}
		lReturnCode = 0;
		break;

	case WM_NCLBUTTONDOWN:
		{
			pkData->m_bInTitleClick = VE_TRUE;
			WindowsVideoDevice::UpdateClipCursor(pkData->m_pkWindow);
		}
		break;

	case WM_NCMOUSELEAVE:
		{
			pkData->m_bInTitleClick = VE_FALSE;
			WindowsVideoDevice::UpdateClipCursor(pkData->m_pkWindow);
		}
		break;
	case WM_ENTERSIZEMOVE:
	case WM_ENTERMENULOOP:
		{
			pkData->m_bInModalLoop = VE_TRUE;
			WindowsVideoDevice::UpdateClipCursor(pkData->m_pkWindow);
		}
		break;

	case WM_EXITSIZEMOVE:
	case WM_EXITMENULOOP:
		{
			pkData->m_bInModalLoop = VE_FALSE;
			WindowsVideoDevice::UpdateClipCursor(pkData->m_pkWindow);
			((WindowsVideoDevice*)Venus3D::Ref().GetVideoDevice())->CheckAsyncMouseRelease(pkData);
		}
		break;

	case WM_WINDOWPOSCHANGED:
		{
			RECT rect;
			int x, y;
			int w, h;

			if (!GetClientRect(hwnd, &rect) || IsRectEmpty(&rect))
			{
				break;
			}
			ClientToScreen(hwnd, (LPPOINT)& rect);
			ClientToScreen(hwnd, (LPPOINT)& rect + 1);

			WindowsVideoDevice::UpdateClipCursor(pkData->m_pkWindow);

			x = rect.left;
			y = rect.top;
			((WindowsVideoDevice*)Venus3D::Ref().GetVideoDevice())->SendWindowEvent(
				pkData->m_pkWindow, VE_WINDOWEVENT_MOVED, x, y);

			w = rect.right - rect.left;
			h = rect.bottom - rect.top;
			((WindowsVideoDevice*)Venus3D::Ref().GetVideoDevice())->SendWindowEvent(
				pkData->m_pkWindow, VE_WINDOWEVENT_RESIZED, w, h);
		}
		break;

	case WM_SIZE:
		{
			switch (wParam)
			{
			case SIZE_MAXIMIZED:
				((WindowsVideoDevice*)Venus3D::Ref().GetVideoDevice())->SendWindowEvent(
					pkData->m_pkWindow, VE_WINDOWEVENT_MAXIMIZED, 0, 0);
				break;
			case SIZE_MINIMIZED:
				((WindowsVideoDevice*)Venus3D::Ref().GetVideoDevice())->SendWindowEvent(
					pkData->m_pkWindow, VE_WINDOWEVENT_MINIMIZED, 0, 0);
				break;
			default:
				((WindowsVideoDevice*)Venus3D::Ref().GetVideoDevice())->SendWindowEvent(
					pkData->m_pkWindow, VE_WINDOWEVENT_RESTORED, 0, 0);
				break;
			}
		}
		break;

	case WM_SETCURSOR:
		{
			WindowsMouse* pkMouse = (WindowsMouse*)(Venus3D::Ref().GetMouse());
			uint16_t hittest;

			hittest = LOWORD(lParam);
			if (hittest == HTCLIENT)
			{
				SetCursor(pkMouse->m_hCursor);
				lReturnCode = TRUE;
			}
		}
		break;

	case WM_PAINT:
		{
			RECT rect;
			if (GetUpdateRect(hwnd, &rect, FALSE))
			{
				ValidateRect(hwnd, nullptr);
				((WindowsVideoDevice*)Venus3D::Ref().GetVideoDevice())->SendWindowEvent(
					pkData->m_pkWindow, VE_WINDOWEVENT_EXPOSED, 0, 0);
			}
		}
		lReturnCode = 0;
		break;

	case WM_ERASEBKGND:
		return (1);
	
	case WM_CLOSE:
		((WindowsVideoDevice*)Venus3D::Ref().GetVideoDevice())->SendWindowEvent(
			pkData->m_pkWindow, VE_WINDOWEVENT_CLOSE, 0, 0);
		return 0;
	default:
		break;
	}

	if (pkData->m_pfuncWndProc)
	{
		return CallWindowProc(pkData->m_pfuncWndProc, hwnd, msg, wParam, lParam);
	}
	else if (lReturnCode >= 0)
	{
		return lReturnCode;
	}
	else
	{
		return CallWindowProc(DefWindowProc, hwnd, msg, wParam, lParam);
	}
}
//--------------------------------------------------------------------------
bool WindowsVideoDevice::RegisterApp(const char* pcName,
	uint32_t u32Style, HINSTANCE hInst) noexcept
{
	if (m_i32AppRegistered)
	{
		++m_i32AppRegistered;
		return true;
	}

	if (!pcName)
	{
		pcName = Venus3D::Ref().GetPakName();
#if defined(CS_BYTEALIGNCLIENT) || defined(CS_OWNDC)
		m_u32AppStyle = (CS_BYTEALIGNCLIENT | CS_OWNDC);
#endif
	}

	assert(pcName);
	m_kAppName = pcName;
	m_u32AppStyle = u32Style;
	m_hInstance = hInst ? hInst : GetModuleHandle(nullptr);

	LPTSTR lptstrAppName = nullptr;
#	ifdef _UNICODE
	int32_t i32Num = MultiByteToWideChar(CP_UTF8, 0, m_kAppName,
		-1, nullptr, 0);
	TempTextW kAppName((size_t)i32Num + 1);
	kAppName.m_lpwstrText[i32Num] = 0;
	if (i32Num)
	{
		MultiByteToWideChar(CP_UTF8, 0, m_kAppName,
			-1, kAppName.m_lpwstrText, i32Num);
	}
	lptstrAppName = kAppName.m_lpwstrText;
#	else
	lptstrAppName = m_kAppName;
#	endif

	WNDCLASS kClass;
	kClass.hCursor = nullptr;
	kClass.hIcon = nullptr;
	kClass.lpszMenuName = nullptr;
	kClass.lpszClassName = lptstrAppName;
	kClass.hbrBackground = nullptr;
	kClass.hInstance = m_hInstance;
	kClass.style = m_u32AppStyle;
	kClass.lpfnWndProc = WindowProc;
	kClass.cbWndExtra = 0;
	kClass.cbClsExtra = 0;

	if (!RegisterClass(&kClass))
	{
		VeCoreLogE("Couldn't register application class");
		return false;
	}
	m_i32AppRegistered = 1;
	return true;
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::UnregisterApp() noexcept
{
	if (!m_i32AppRegistered)
		return;
	--m_i32AppRegistered;
	if (m_i32AppRegistered <= 0)
	{
		LPTSTR lptstrAppName = nullptr;
#		ifdef _UNICODE
		int32_t i32Num = MultiByteToWideChar(CP_UTF8, 0, m_kAppName,
			-1, nullptr, 0);
		TempTextW kAppName((size_t)i32Num + 1);
		kAppName.m_lpwstrText[i32Num] = 0;
		if (i32Num)
		{
			MultiByteToWideChar(CP_UTF8, 0, m_kAppName,
				-1, kAppName.m_lpwstrText, i32Num);
		}
		lptstrAppName = kAppName.m_lpwstrText;
#		else
		lptstrAppName = m_kAppName;
#		endif

		WNDCLASS kClass;
		if (GetClassInfo(m_hInstance, lptstrAppName, &kClass))
		{
			UnregisterClass(lptstrAppName, m_hInstance);
		}
		m_i32AppRegistered = 0;
		m_kAppName = "";
		m_u32AppStyle = 0;
		m_hInstance = nullptr;
	}
}
//--------------------------------------------------------------------------
static bool GetDisplayMode(LPCTSTR deviceName, DWORD index,
	VeDisplayMode* mode) noexcept
{
	VeDisplayModeData* data;
	DEVMODE devmode;
	HDC hdc;

	devmode.dmSize = sizeof(devmode);
	devmode.dmDriverExtra = 0;
	if (!EnumDisplaySettings(deviceName, index, &devmode))
	{
		return false;
	}

	data = VE_NEW VeDisplayModeData;
	assert(data);
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
		&& (hdc = CreateDC(deviceName, nullptr, nullptr, nullptr)) != nullptr)
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

		memset(bmi_data, 0, sizeof(bmi_data));
		bmi = (LPBITMAPINFO)bmi_data;
		bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

		hbm = CreateCompatibleBitmap(hdc, 1, 1);
		GetDIBits(hdc, hbm, 0, 1, nullptr, bmi, DIB_RGB_COLORS);
		GetDIBits(hdc, hbm, 0, 1, nullptr, bmi, DIB_RGB_COLORS);
		DeleteObject(hbm);
		DeleteDC(hdc);
		if (bmi->bmiHeader.biCompression == BI_BITFIELDS)
		{
			switch (*(uint32_t *)bmi->bmiColors)
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
bool WindowsVideoDevice::AddDisplay(LPCTSTR DeviceName) noexcept
{
	VeDisplayMode kMode;
	
	if (!GetDisplayMode(DeviceName, ENUM_CURRENT_SETTINGS, &kMode))
	{
		return false;
	}

	m_kDisplayList.resize(m_kDisplayList.size() + 1);
	VeVideoDisplay& kDisplay = m_kDisplayList.back();

	VeDisplayData* pkDisplayData = VE_NEW VeDisplayData;
	assert(pkDisplayData);
	DISPLAY_DEVICE device;
	device.cb = sizeof(device);
	if (EnumDisplayDevices(DeviceName, 0, &device, 0))
	{
#		ifdef _UNICODE
		int32_t i32Num = WideCharToMultiByte(CP_UTF8, 0,
			device.DeviceString, -1, 0, 0, 0, FALSE);
		TempTextA kDisplayName((size_t)i32Num + 1);
		kDisplayName.m_lpstrText[i32Num] = 0;
		if (i32Num)
		{
			WideCharToMultiByte(CP_UTF8, 0, device.DeviceString,
				-1, kDisplayName.m_lpstrText, i32Num, 0, FALSE);
		}
		kDisplay.m_kName = kDisplayName.m_lpstrText;
#		else
		kDisplay.m_kName = device.DeviceString;
#		endif
	}

	kDisplay.m_kDesktopMode = kMode;
	kDisplay.m_kCurrentMode = kMode;
	kDisplay.m_spDriverData = pkDisplayData;
	return true;
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::InitModes() noexcept
{
	int32_t pass;
	DWORD i, j, count;
	DISPLAY_DEVICE device;

	device.cb = sizeof(device);

	for (pass = 0; pass < 2; ++pass)
	{
		for (i = 0;; ++i)
		{
			TCHAR DeviceName[32];

			if (!EnumDisplayDevices(nullptr, i, &device, 0))
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
			memcpy(DeviceName, device.DeviceName, sizeof(DeviceName));

			count = 0;
			for (j = 0;; ++j)
			{
				if (!EnumDisplayDevices(DeviceName, j, &device, 0))
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

	assert(m_kDisplayList.size());
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::TermModes() noexcept
{
	m_kDisplayList.clear();
}
//--------------------------------------------------------------------------
bool WindowsVideoDevice::_GetDisplayBounds(VeVideoDisplay* pkDisplay,
	VeRect* pkRect) noexcept
{
	assert(pkDisplay);
	VeDisplayModeData* pkData = (VeDisplayModeData*)pkDisplay->m_kCurrentMode.m_spDriverData;

	pkRect->x = (int32_t)ceilf(pkData->DeviceMode.dmPosition.x * pkData->ScaleX);
	pkRect->y = (int32_t)ceilf(pkData->DeviceMode.dmPosition.y * pkData->ScaleY);
	pkRect->w = (int32_t)ceilf(pkData->DeviceMode.dmPelsWidth * pkData->ScaleX);
	pkRect->h = (int32_t)ceilf(pkData->DeviceMode.dmPelsHeight * pkData->ScaleY);
	return true;
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::_GetDisplayModes(VeVideoDisplay* pkDisplay) noexcept
{
	VeDisplayData* data = (VeDisplayData*)pkDisplay->m_spDriverData;
	VeDisplayMode mode;

	for (uint32_t i = 0;; ++i)
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
bool WindowsVideoDevice::_SetDisplayMode(VeVideoDisplay* pkDisplay,
	VeDisplayMode* pkMode) noexcept
{
	VeDisplayData* pkDisplayData = (VeDisplayData*)pkDisplay->m_spDriverData;
	VeDisplayModeData* pkData = (VeDisplayModeData*)pkMode->m_spDriverData;
	LONG status;

	status = ChangeDisplaySettingsEx(pkDisplayData->DeviceName,
		&pkData->DeviceMode, nullptr, CDS_FULLSCREEN, nullptr);
	if (status != DISP_CHANGE_SUCCESSFUL)
	{
		const char* pcReason = "Unknown reason";
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
		VeCoreLogE("ChangeDisplaySettingsEx() failed: %s", pcReason);
		return false;
	}
	EnumDisplaySettings(pkDisplayData->DeviceName, ENUM_CURRENT_SETTINGS, &pkData->DeviceMode);
	return true;
}
//--------------------------------------------------------------------------
#define TICKS_PASSED(A, B)  ((int32_t)((B) - (A)) <= 0)
//--------------------------------------------------------------------------
void WindowsVideoDevice::_PumpEvents() noexcept
{
	MSG msg;
	DWORD dwStartTicks = GetTickCount();
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (TICKS_PASSED(msg.time, dwStartTicks))
		{
			break;
		}
	}
}
//--------------------------------------------------------------------------
#define STYLE_BASIC         (WS_CLIPSIBLINGS | WS_CLIPCHILDREN)
#define STYLE_FULLSCREEN    (WS_POPUP)
#define STYLE_BORDERLESS    (WS_POPUP)
#define STYLE_NORMAL        (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX)
#define STYLE_RESIZABLE     (WS_THICKFRAME | WS_MAXIMIZEBOX)
#define STYLE_MASK          (STYLE_FULLSCREEN | STYLE_BORDERLESS | STYLE_NORMAL | STYLE_RESIZABLE)
//--------------------------------------------------------------------------
static DWORD GetWindowStyle(VeWindow::Data* pkWindow) noexcept
{
	DWORD style = 0;

	if (pkWindow->m_u32Flags & VE_WINDOW_FULLSCREEN)
	{
		style |= STYLE_FULLSCREEN;
	}
	else
	{
		if (pkWindow->m_u32Flags & VE_WINDOW_BORDERLESS)
		{
			style |= STYLE_BORDERLESS;
		}
		else {
			style |= STYLE_NORMAL;
		}
		if (pkWindow->m_u32Flags & VE_WINDOW_RESIZABLE)
		{
			style |= STYLE_RESIZABLE;
		}
	}
	return style;
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::SetWindowPositionInternal(
	VeWindow::Data* pkWindow, uint32_t u32Flags) noexcept
{
	VeWindowData* pkData = (VeWindowData*)pkWindow->m_spDriverdata;
	HWND hwnd = pkData->m_hWnd;
	RECT rect;
	DWORD style;
	HWND top;
	BOOL menu;
	int32_t x, y;
	int32_t w, h;

	if ((pkWindow->m_u32Flags & (VE_WINDOW_FULLSCREEN | VE_WINDOW_INPUT_FOCUS))
		== (VE_WINDOW_FULLSCREEN | VE_WINDOW_INPUT_FOCUS))
	{
		top = HWND_TOPMOST;
	}
	else
	{
		top = HWND_NOTOPMOST;
	}

	style = GetWindowLong(hwnd, GWL_STYLE);
	rect.left = 0;
	rect.top = 0;
	rect.right = pkWindow->w;
	rect.bottom = pkWindow->h;
	menu = (style & WS_CHILDWINDOW) ? FALSE : (GetMenu(hwnd) != nullptr);
	AdjustWindowRectEx(&rect, style, menu, 0);
	w = (rect.right - rect.left);
	h = (rect.bottom - rect.top);
	x = pkWindow->x + rect.left;
	y = pkWindow->y + rect.top;

	pkData->m_bExpectedResize = TRUE;
	SetWindowPos(hwnd, top, x, y, w, h, u32Flags);
	pkData->m_bExpectedResize = FALSE;
}
//--------------------------------------------------------------------------
bool WindowsVideoDevice::SetupWindowData(VeWindow::Data* pkWindow,
	HWND hWnd, VE_BOOL bCreated) noexcept
{
	assert(pkWindow);
	VeWindowData* pkData = VE_NEW VeWindowData;
	assert(pkData);
	pkData->m_pkWindow = pkWindow;
	pkData->m_hWnd = hWnd;
	pkData->m_hDc = GetDC(hWnd);
	pkData->m_bCreated = bCreated;
	pkData->m_u32MouseButtonFlags = 0;
	pkWindow->m_spDriverdata = pkData;

	s_kWindowDataMap[hWnd] = pkData;

	pkData->m_pfuncWndProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC);
	if (pkData->m_pfuncWndProc == WindowProc)
	{
		pkData->m_pfuncWndProc = nullptr;
	}
	else
	{
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WindowProc);
	}

	{
		RECT rect;
		if (GetClientRect(hWnd, &rect))
		{
			int32_t w = rect.right;
			int32_t h = rect.bottom;
			if ((pkWindow->w && pkWindow->w != w) || (pkWindow->h && pkWindow->h != h))
			{
				SetWindowPositionInternal(pkWindow,
					SWP_NOCOPYBITS | SWP_NOZORDER | SWP_NOACTIVATE);
			}
			else
			{
				pkWindow->w = w;
				pkWindow->h = h;
			}
		}
	}

	{
		POINT point;
		point.x = 0;
		point.y = 0;
		if (ClientToScreen(hWnd, &point))
		{
			pkWindow->x = point.x;
			pkWindow->y = point.y;
		}
	}

	{
		DWORD style = GetWindowLong(hWnd, GWL_STYLE);
		if (style & WS_VISIBLE)
		{
			pkWindow->m_u32Flags |= VE_WINDOW_SHOWN;
		}
		else
		{
			pkWindow->m_u32Flags &= ~VE_WINDOW_SHOWN;
		}
		if (style & (WS_BORDER | WS_THICKFRAME))
		{
			pkWindow->m_u32Flags &= ~VE_WINDOW_BORDERLESS;
		}
		else
		{
			pkWindow->m_u32Flags |= VE_WINDOW_BORDERLESS;
		}
		if (style & WS_THICKFRAME)
		{
			pkWindow->m_u32Flags |= VE_WINDOW_RESIZABLE;
		}
		else
		{
			pkWindow->m_u32Flags &= ~VE_WINDOW_RESIZABLE;
		}
#		ifdef WS_MAXIMIZE
		if (style & WS_MAXIMIZE)
		{
			pkWindow->m_u32Flags |= VE_WINDOW_MAXIMIZED;
		}
		else
#		endif
		{
			pkWindow->m_u32Flags &= ~VE_WINDOW_MAXIMIZED;
		}
#		ifdef WS_MINIMIZE
		if (style & WS_MINIMIZE)
		{
			pkWindow->m_u32Flags |= VE_WINDOW_MINIMIZED;
		}
		else
#		endif
		{
			pkWindow->m_u32Flags &= ~VE_WINDOW_MINIMIZED;
		}
	}

	if (GetFocus() == hWnd)
	{
		pkWindow->m_u32Flags |= VE_WINDOW_INPUT_FOCUS;
		if(Venus3D::Ref().GetKeyboard()) Venus3D::Ref().GetKeyboard()->SetFocus(pkData->m_pkWindow);

		if (pkWindow->m_u32Flags & VE_WINDOW_INPUT_GRABBED)
		{
			RECT rect;
			GetClientRect(hWnd, &rect);
			ClientToScreen(hWnd, (LPPOINT)& rect);
			ClientToScreen(hWnd, (LPPOINT)& rect + 1);
			ClipCursor(&rect);
		}
	}

	/*if (videodata->RegisterTouchWindow)
	{
		videodata->RegisterTouchWindow(hwnd, (TWF_FINETOUCH | TWF_WANTPALM));
	}*/

	DragAcceptFiles(hWnd, TRUE);

	return true;
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::CheckWParamMouseButton(VE_BOOL bwParamMousePressed,
	VE_BOOL bMousePressed, VeWindowData* pkData, uint8_t u8Button) noexcept
{
	if (bwParamMousePressed && !bMousePressed)
	{
		Venus3D::Ref().GetMouse()->SendButton(pkData->m_pkWindow, 0, VE_PRESSED, u8Button);
	}
	else if (!bwParamMousePressed && bMousePressed)
	{
		Venus3D::Ref().GetMouse()->SendButton(pkData->m_pkWindow, 0, VE_RELEASED, u8Button);
	}
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::CheckWParamMouseButtons(WPARAM wParam,
	VeWindowData* pkData) noexcept
{
	assert(Venus3D::Ref().GetMouse());
	if (wParam != pkData->m_u32MouseButtonFlags)
	{
		uint32_t u32MouseFlags = Venus3D::Ref().GetMouse()->GetButtonState();
		CheckWParamMouseButton((wParam & MK_LBUTTON), (u32MouseFlags & VE_BUTTON_LMASK), pkData, VE_BUTTON_LEFT);
		CheckWParamMouseButton((wParam & MK_MBUTTON), (u32MouseFlags & VE_BUTTON_MMASK), pkData, VE_BUTTON_MIDDLE);
		CheckWParamMouseButton((wParam & MK_RBUTTON), (u32MouseFlags & VE_BUTTON_RMASK), pkData, VE_BUTTON_RIGHT);
		CheckWParamMouseButton((wParam & MK_XBUTTON1), (u32MouseFlags & VE_BUTTON_X1MASK), pkData, VE_BUTTON_X1);
		CheckWParamMouseButton((wParam & MK_XBUTTON2), (u32MouseFlags & VE_BUTTON_X2MASK), pkData, VE_BUTTON_X2);
		pkData->m_u32MouseButtonFlags = wParam;
	}
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::CheckRawMouseButtons(ULONG rawButtons,
	VeWindowData* pkData) noexcept
{
	if (rawButtons != pkData->m_u32MouseButtonFlags)
	{
		uint32_t u32MouseFlags = Venus3D::Ref().GetMouse()->GetButtonState();
		if ((rawButtons & RI_MOUSE_BUTTON_1_DOWN))
			CheckWParamMouseButton((rawButtons & RI_MOUSE_BUTTON_1_DOWN), (u32MouseFlags & VE_BUTTON_LMASK), pkData, VE_BUTTON_LEFT);
		if ((rawButtons & RI_MOUSE_BUTTON_1_UP))
			CheckWParamMouseButton(!(rawButtons & RI_MOUSE_BUTTON_1_UP), (u32MouseFlags & VE_BUTTON_LMASK), pkData, VE_BUTTON_LEFT);
		if ((rawButtons & RI_MOUSE_BUTTON_2_DOWN))
			CheckWParamMouseButton((rawButtons & RI_MOUSE_BUTTON_2_DOWN), (u32MouseFlags & VE_BUTTON_RMASK), pkData, VE_BUTTON_RIGHT);
		if ((rawButtons & RI_MOUSE_BUTTON_2_UP))
			CheckWParamMouseButton(!(rawButtons & RI_MOUSE_BUTTON_2_UP), (u32MouseFlags & VE_BUTTON_RMASK), pkData, VE_BUTTON_RIGHT);
		if ((rawButtons & RI_MOUSE_BUTTON_3_DOWN))
			CheckWParamMouseButton((rawButtons & RI_MOUSE_BUTTON_3_DOWN), (u32MouseFlags & VE_BUTTON_MMASK), pkData, VE_BUTTON_MIDDLE);
		if ((rawButtons & RI_MOUSE_BUTTON_3_UP))
			CheckWParamMouseButton(!(rawButtons & RI_MOUSE_BUTTON_3_UP), (u32MouseFlags & VE_BUTTON_MMASK), pkData, VE_BUTTON_MIDDLE);
		if ((rawButtons & RI_MOUSE_BUTTON_4_DOWN))
			CheckWParamMouseButton((rawButtons & RI_MOUSE_BUTTON_4_DOWN), (u32MouseFlags & VE_BUTTON_X1MASK), pkData, VE_BUTTON_X1);
		if ((rawButtons & RI_MOUSE_BUTTON_4_UP))
			CheckWParamMouseButton(!(rawButtons & RI_MOUSE_BUTTON_4_UP), (u32MouseFlags & VE_BUTTON_X1MASK), pkData, VE_BUTTON_X1);
		if ((rawButtons & RI_MOUSE_BUTTON_5_DOWN))
			CheckWParamMouseButton((rawButtons & RI_MOUSE_BUTTON_5_DOWN), (u32MouseFlags & VE_BUTTON_X2MASK), pkData, VE_BUTTON_X2);
		if ((rawButtons & RI_MOUSE_BUTTON_5_UP))
			CheckWParamMouseButton(!(rawButtons & RI_MOUSE_BUTTON_5_UP), (u32MouseFlags & VE_BUTTON_X2MASK), pkData, VE_BUTTON_X2);
		pkData->m_u32MouseButtonFlags = rawButtons;
	}
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::CheckAsyncMouseRelease(
	VeWindowData* pkData) noexcept
{
	uint32_t u32MouseFlags = Venus3D::Ref().GetMouse()->GetButtonState();
	SHORT keyState;
	keyState = GetAsyncKeyState(VK_LBUTTON);
	CheckWParamMouseButton((keyState & 0x8000), (u32MouseFlags & VE_BUTTON_LMASK), pkData, VE_BUTTON_LEFT);
	keyState = GetAsyncKeyState(VK_RBUTTON);
	CheckWParamMouseButton((keyState & 0x8000), (u32MouseFlags & VE_BUTTON_RMASK), pkData, VE_BUTTON_RIGHT);
	keyState = GetAsyncKeyState(VK_MBUTTON);
	CheckWParamMouseButton((keyState & 0x8000), (u32MouseFlags & VE_BUTTON_MMASK), pkData, VE_BUTTON_MIDDLE);
	keyState = GetAsyncKeyState(VK_XBUTTON1);
	CheckWParamMouseButton((keyState & 0x8000), (u32MouseFlags & VE_BUTTON_X1MASK), pkData, VE_BUTTON_X1);
	keyState = GetAsyncKeyState(VK_XBUTTON2);
	CheckWParamMouseButton((keyState & 0x8000), (u32MouseFlags & VE_BUTTON_X2MASK), pkData, VE_BUTTON_X2);
	pkData->m_u32MouseButtonFlags = 0;
}
//--------------------------------------------------------------------------
bool WindowsVideoDevice::_CreateWindow(VeWindow::Data* pkWindow) noexcept
{
	assert(pkWindow);
	HWND hwnd;
	RECT rect;
	DWORD style = STYLE_BASIC;
	int32_t x, y;
	int32_t w, h;

	style |= GetWindowStyle(pkWindow);

	rect.left = pkWindow->x;
	rect.top = pkWindow->y;
	rect.right = pkWindow->x + pkWindow->w;
	rect.bottom = pkWindow->y + pkWindow->h;
	AdjustWindowRectEx(&rect, style, FALSE, 0);
	x = rect.left;
	y = rect.top;
	w = (rect.right - rect.left);
	h = (rect.bottom - rect.top);

	LPTSTR lptstrAppName = nullptr;
#	ifdef _UNICODE
	int32_t i32Num = MultiByteToWideChar(CP_UTF8, 0, m_kAppName,
		-1, nullptr, 0);
	TempTextW kAppName((size_t)i32Num + 1);
	kAppName.m_lpwstrText[i32Num] = 0;
	if (i32Num)
	{
		MultiByteToWideChar(CP_UTF8, 0, m_kAppName,
			-1, kAppName.m_lpwstrText, i32Num);
	}
	lptstrAppName = kAppName.m_lpwstrText;
#	else
	lptstrAppName = m_kAppName;
#	endif

	hwnd = CreateWindow(lptstrAppName, L"", style, x, y, w, h,
		0, 0, m_hInstance, 0);
	if (!hwnd)
	{
		return false;
	}

	SetCursor(LoadCursor(NULL, IDC_ARROW));

	_PumpEvents();

	if (!SetupWindowData(pkWindow, hwnd, VE_TRUE))
	{
		::DestroyWindow(hwnd);
		return false;
	}

	return true;
}
//--------------------------------------------------------------------------
bool WindowsVideoDevice::_CreateWindowFrom(VeWindow::Data* pkWindow,
	const void* pvData) noexcept
{
	assert(pkWindow && pvData);
	HWND hWnd = (HWND)pvData;

	int32_t i32TitleLen = GetWindowTextLength(hWnd);
	if (i32TitleLen > 0)
	{
		LPTSTR lpTitle = VeStackAlloc(TCHAR, i32TitleLen + 1);
		assert(lpTitle);
		lpTitle[i32TitleLen] = 0;
		i32TitleLen = GetWindowText(hWnd, lpTitle, i32TitleLen);
		assert(i32TitleLen > 0);
#		ifdef _UNICODE
		int32_t i32Num = WideCharToMultiByte(CP_UTF8, 0,
			lpTitle, -1, 0, 0, 0, FALSE);
		TempTextA kTitle((size_t)i32Num + 1);
		kTitle.m_lpstrText[i32Num] = 0;
		if (i32Num)
		{
			WideCharToMultiByte(CP_UTF8, 0, lpTitle,
				-1, kTitle.m_lpstrText, i32Num, 0, FALSE);
		}
		pkWindow->m_kTitle = kTitle.m_lpstrText;
#		else
		pkWindow->m_kTitle = lpTitle;
#		endif
		VeStackFree(lpTitle);
	}

	if (!SetupWindowData(pkWindow, hWnd, VE_FALSE))
	{
		return false;
	}

	return true;
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::_SetWindowTitle(VeWindow::Data* pkWindow) noexcept
{
	assert(pkWindow);
	HWND hWnd = ((VeWindowData*)pkWindow->m_spDriverdata)->m_hWnd;

	LPTSTR lptstrTitleAppName = nullptr;
#	ifdef _UNICODE
	int32_t i32Num = MultiByteToWideChar(CP_UTF8, 0, pkWindow->m_kTitle,
		-1, nullptr, 0);
	TempTextW kTitle((size_t)i32Num + 1);
	kTitle.m_lpwstrText[i32Num] = 0;
	if (i32Num)
	{
		MultiByteToWideChar(CP_UTF8, 0, pkWindow->m_kTitle,
			-1, kTitle.m_lpwstrText, i32Num);
	}
	lptstrTitleAppName = kTitle.m_lpwstrText;
#	else
	lptstrTitleAppName = pkWindow->m_kTitle;
#	endif
	SetWindowText(hWnd, lptstrTitleAppName);
}
//--------------------------------------------------------------------------
#define WRITE_BYTE_BUF(buf, t,val) (*(t*)buf) = (val); buf += sizeof(val)
//--------------------------------------------------------------------------
void WindowsVideoDevice::_SetWindowIcon(VeWindow::Data* pkWindow,
	const VeSurfacePtr& spIcon) noexcept
{
	assert(pkWindow && spIcon);
	HWND hWnd = ((VeWindowData*)pkWindow->m_spDriverdata)->m_hWnd;

	int32_t i32IconLen = 40 + spIcon->GetHeight() * spIcon->GetWidth() * 4;
	BYTE* pbyIconBmp = VeStackAlloc(BYTE, i32IconLen);
	
	BYTE* pbyPointer(pbyIconBmp);

	WRITE_BYTE_BUF(pbyPointer, uint32_t, 40);
	WRITE_BYTE_BUF(pbyPointer, uint32_t, spIcon->GetWidth());
	WRITE_BYTE_BUF(pbyPointer, uint32_t, spIcon->GetHeight() * 2);
	WRITE_BYTE_BUF(pbyPointer, uint16_t, 1);
	WRITE_BYTE_BUF(pbyPointer, uint16_t, 32);
	WRITE_BYTE_BUF(pbyPointer, uint32_t, BI_RGB);
	WRITE_BYTE_BUF(pbyPointer, uint32_t, spIcon->GetWidth() * spIcon->GetHeight() * 4);
	WRITE_BYTE_BUF(pbyPointer, uint32_t, 0);
	WRITE_BYTE_BUF(pbyPointer, uint32_t, 0);
	WRITE_BYTE_BUF(pbyPointer, uint32_t, 0);
	WRITE_BYTE_BUF(pbyPointer, uint32_t, 0);
	
	assert(spIcon->GetFormat()->m_u32Format == VE_PIXELFORMAT_ARGB8888);
	int32_t y = spIcon->GetHeight();
	while (y--)
	{
		uint8_t* pu8Src = (uint8_t*)spIcon->GetBuffer() + y * spIcon->GetPitch();
		memcpy(pbyPointer, pu8Src, spIcon->GetPitch());
	}

	HICON hIcon = CreateIconFromResource(pbyIconBmp, i32IconLen, TRUE, 0x00030000);
	
	VeStackFree(pbyIconBmp);

	SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

	SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::_SetWindowPosition(
	VeWindow::Data* pkWindow) noexcept
{
	SetWindowPositionInternal(pkWindow, SWP_NOCOPYBITS | SWP_NOSIZE | SWP_NOACTIVATE);
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::_SetWindowSize(VeWindow::Data* pkWindow) noexcept
{
	SetWindowPositionInternal(pkWindow, SWP_NOCOPYBITS | SWP_NOSIZE | SWP_NOACTIVATE);
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::_ShowWindow(VeWindow::Data* pkWindow) noexcept
{
	assert(pkWindow);
	HWND hWnd = ((VeWindowData*)pkWindow->m_spDriverdata)->m_hWnd;
	::ShowWindow(hWnd, SW_SHOW);
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::_HideWindow(VeWindow::Data* pkWindow) noexcept
{
	assert(pkWindow);
	HWND hWnd = ((VeWindowData*)pkWindow->m_spDriverdata)->m_hWnd;
	::ShowWindow(hWnd, SW_HIDE);
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::_RaiseWindow(VeWindow::Data* pkWindow) noexcept
{
	SetWindowPositionInternal(pkWindow, SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOSIZE);
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::_MaximizeWindow(VeWindow::Data* pkWindow) noexcept
{
	assert(pkWindow);
	VeWindowData* pkData = (VeWindowData*)pkWindow->m_spDriverdata;
	HWND hWnd = pkData->m_hWnd;
	pkData->m_bExpectedResize = TRUE;
	::ShowWindow(hWnd, SW_MAXIMIZE);
	pkData->m_bExpectedResize = FALSE;
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::_MinimizeWindow(VeWindow::Data* pkWindow) noexcept
{
	assert(pkWindow);
	HWND hWnd = ((VeWindowData*)pkWindow->m_spDriverdata)->m_hWnd;
	::ShowWindow(hWnd, SW_MINIMIZE);
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::_RestoreWindow(VeWindow::Data* pkWindow) noexcept
{
	assert(pkWindow);
	VeWindowData* pkData = (VeWindowData*)pkWindow->m_spDriverdata;
	HWND hWnd = pkData->m_hWnd;
	pkData->m_bExpectedResize = TRUE;
	::ShowWindow(hWnd, SW_RESTORE);
	pkData->m_bExpectedResize = FALSE;
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::_SetWindowBordered(VeWindow::Data* pkWindow,
	VE_BOOL bBordered) noexcept
{
	assert(pkWindow);
	HWND hWnd = ((VeWindowData*)pkWindow->m_spDriverdata)->m_hWnd;
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);

	if (bBordered)
	{
		dwStyle &= ~STYLE_BORDERLESS;
		dwStyle |= STYLE_NORMAL;
	}
	else {
		dwStyle &= ~STYLE_NORMAL;
		dwStyle |= STYLE_BORDERLESS;
	}

	SetWindowLong(hWnd, GWL_STYLE, dwStyle);
	SetWindowPositionInternal(pkWindow, SWP_NOCOPYBITS | SWP_FRAMECHANGED
		| SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::_SetWindowFullscreen(VeWindow::Data* pkWindow,
	VeVideoDisplay* pkDisplay, VE_BOOL bFullscreen) noexcept
{
	assert(pkWindow);
	VeWindowData* pkData = (VeWindowData*)pkWindow->m_spDriverdata;
	HWND hWnd = pkData->m_hWnd;
	RECT kRect;
	VeRect kBounds;
	DWORD dwStyle;
	HWND hTop;
	BOOL bMenu;
	int32_t x, y;
	int32_t w, h;

	if ((pkWindow->m_u32Flags & (VE_WINDOW_FULLSCREEN | VE_WINDOW_INPUT_FOCUS))
		== (VE_WINDOW_FULLSCREEN | VE_WINDOW_INPUT_FOCUS))
	{
		hTop = HWND_TOPMOST;
	}
	else
	{
		hTop = HWND_NOTOPMOST;
	}

	dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	dwStyle &= ~STYLE_MASK;
	dwStyle |= GetWindowStyle(pkWindow);

	_GetDisplayBounds(pkDisplay, &kBounds);

	if (bFullscreen)
	{
		x = kBounds.x;
		y = kBounds.y;
		w = kBounds.w;
		h = kBounds.h;
	}
	else
	{
		kRect.left = 0;
		kRect.top = 0;
		kRect.right = pkWindow->m_kWindowed.w;
		kRect.bottom = pkWindow->m_kWindowed.h;
		bMenu = (dwStyle & WS_CHILDWINDOW) ? FALSE : (GetMenu(hWnd) != nullptr);
		AdjustWindowRectEx(&kRect, dwStyle, bMenu, 0);
		w = (kRect.right - kRect.left);
		h = (kRect.bottom - kRect.top);
		x = pkWindow->m_kWindowed.x + kRect.left;
		y = pkWindow->m_kWindowed.y + kRect.top;
	}
	SetWindowLong(hWnd, GWL_STYLE, dwStyle);
	pkData->m_bExpectedResize = TRUE;
	SetWindowPos(hWnd, hTop, x, y, w, h, SWP_NOCOPYBITS | SWP_NOACTIVATE);
	pkData->m_bExpectedResize = FALSE;
}
//--------------------------------------------------------------------------
bool WindowsVideoDevice::_SetWindowGammaRamp(VeWindow::Data* pkWindow,
	const uint16_t* pu16Ramp) noexcept
{
	assert(pkWindow);
	VeVideoDisplay* pkDisplay = GetDisplayForWindow(pkWindow);
	VeDisplayData* pkData = (VeDisplayData*)pkDisplay->m_spDriverData;
	
	BOOL bSucceeded = FALSE;

	HDC hDc = CreateDC(pkData->DeviceName, nullptr, nullptr, nullptr);
	if (hDc)
	{
		bSucceeded = SetDeviceGammaRamp(hDc, (LPVOID)pu16Ramp);
		if (!bSucceeded)
		{
			VeCoreLogE("SetDeviceGammaRamp()");
		}
		DeleteDC(hDc);
	}
	return bSucceeded ? true : false;
}
//--------------------------------------------------------------------------
bool WindowsVideoDevice::_GetWindowGammaRamp(VeWindow::Data* pkWindow,
	uint16_t* pu16Ramp) noexcept
{
	assert(pkWindow);
	VeVideoDisplay* pkDisplay = GetDisplayForWindow(pkWindow);
	VeDisplayData* pkData = (VeDisplayData*)pkDisplay->m_spDriverData;
	
	BOOL bSucceeded = FALSE;

	HDC hDc = CreateDC(pkData->DeviceName, nullptr, nullptr, nullptr);
	if (hDc)
	{
		bSucceeded = GetDeviceGammaRamp(hDc, (LPVOID)pu16Ramp);
		if (!bSucceeded)
		{
			VeCoreLogE("GetDeviceGammaRamp()");
		}
		DeleteDC(hDc);
	}
	return bSucceeded ? true : false;
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::_SetWindowGrab(VeWindow::Data* pkWindow,
	VE_BOOL) noexcept
{
	UpdateClipCursor(pkWindow);

	if (pkWindow->m_u32Flags & VE_WINDOW_FULLSCREEN)
	{
		UINT flags = SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOSIZE;

		if (!(pkWindow->m_u32Flags & VE_WINDOW_SHOWN))
		{
			flags |= SWP_NOACTIVATE;
		}
		SetWindowPositionInternal(pkWindow, flags);
	}
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::_DestroyWindow(VeWindow::Data* pkWindow) noexcept
{
	assert(pkWindow);
	VeWindowData* pkData = (VeWindowData*)pkWindow->m_spDriverdata;

	if (pkData)
	{
		ReleaseDC(pkData->m_hWnd, pkData->m_hDc);
		s_kWindowDataMap.erase(pkData->m_hWnd);
		if (pkData->m_bCreated)
		{
			::DestroyWindow(pkData->m_hWnd);
		}
		else
		{
			if (pkData->m_pfuncWndProc)
			{
				SetWindowLongPtr(pkData->m_hWnd, GWLP_WNDPROC,
					(LONG_PTR)pkData->m_pfuncWndProc);
			}
		}
		pkWindow->m_spDriverdata = nullptr;
	}
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::_OnWindowEnter(VeWindow::Data* pkWindow) noexcept
{
#	ifdef WM_MOUSELEAVE
	VeWindowData* pkData = (VeWindowData*)pkWindow->m_spDriverdata;
	TRACKMOUSEEVENT trackMouseEvent;

	if (!pkData || !pkData->m_hWnd)
	{
		return;
	}

	trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
	trackMouseEvent.dwFlags = TME_LEAVE;
	trackMouseEvent.hwndTrack = pkData->m_hWnd;

	TrackMouseEvent(&trackMouseEvent);
#	endif
}
//--------------------------------------------------------------------------
void WindowsVideoDevice::_GetWindowWMInfo(VeWindow::Data* pkWindow,
	VeSysWMInfo* pkInfo) noexcept
{
	assert(pkWindow && pkInfo);
	HWND hwnd = ((VeWindowData*)pkWindow->m_spDriverdata)->m_hWnd;
	pkInfo->m_eType = VE_SYSWM_WINDOWS;
	pkInfo->win.window = hwnd;
}
//--------------------------------------------------------------------------
