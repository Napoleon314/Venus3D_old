////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeKeyboard.cpp
//  Created:     2015/08/07 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"

//--------------------------------------------------------------------------
#ifdef VE_ENABLE_KEYBOARD
//--------------------------------------------------------------------------
VeKeyboardPtr CreateKeyboard() noexcept
{
	return VE_NEW VeKeyboard();
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
static const VeKeycode s_ai32DefaultKeymap[VE_NUM_SCANCODES] =
{
	0, 0, 0, 0,
	'a',
	'b',
	'c',
	'd',
	'e',
	'f',
	'g',
	'h',
	'i',
	'j',
	'k',
	'l',
	'm',
	'n',
	'o',
	'p',
	'q',
	'r',
	's',
	't',
	'u',
	'v',
	'w',
	'x',
	'y',
	'z',
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	'0',
	VE_K_RETURN,
	VE_K_ESCAPE,
	VE_K_BACKSPACE,
	VE_K_TAB,
	VE_K_SPACE,
	'-',
	'=',
	'[',
	']',
	'\\',
	'#',
	';',
	'\'',
	'`',
	',',
	'.',
	'/',
	VE_K_CAPSLOCK,
	VE_K_F1,
	VE_K_F2,
	VE_K_F3,
	VE_K_F4,
	VE_K_F5,
	VE_K_F6,
	VE_K_F7,
	VE_K_F8,
	VE_K_F9,
	VE_K_F10,
	VE_K_F11,
	VE_K_F12,
	VE_K_PRINTSCREEN,
	VE_K_SCROLLLOCK,
	VE_K_PAUSE,
	VE_K_INSERT,
	VE_K_HOME,
	VE_K_PAGEUP,
	VE_K_DELETE,
	VE_K_END,
	VE_K_PAGEDOWN,
	VE_K_RIGHT,
	VE_K_LEFT,
	VE_K_DOWN,
	VE_K_UP,
	VE_K_NUMLOCKCLEAR,
	VE_K_KP_DIVIDE,
	VE_K_KP_MULTIPLY,
	VE_K_KP_MINUS,
	VE_K_KP_PLUS,
	VE_K_KP_ENTER,
	VE_K_KP_1,
	VE_K_KP_2,
	VE_K_KP_3,
	VE_K_KP_4,
	VE_K_KP_5,
	VE_K_KP_6,
	VE_K_KP_7,
	VE_K_KP_8,
	VE_K_KP_9,
	VE_K_KP_0,
	VE_K_KP_PERIOD,
	0,
	VE_K_APPLICATION,
	VE_K_POWER,
	VE_K_KP_EQUALS,
	VE_K_F13,
	VE_K_F14,
	VE_K_F15,
	VE_K_F16,
	VE_K_F17,
	VE_K_F18,
	VE_K_F19,
	VE_K_F20,
	VE_K_F21,
	VE_K_F22,
	VE_K_F23,
	VE_K_F24,
	VE_K_EXECUTE,
	VE_K_HELP,
	VE_K_MENU,
	VE_K_SELECT,
	VE_K_STOP,
	VE_K_AGAIN,
	VE_K_UNDO,
	VE_K_CUT,
	VE_K_COPY,
	VE_K_PASTE,
	VE_K_FIND,
	VE_K_MUTE,
	VE_K_VOLUMEUP,
	VE_K_VOLUMEDOWN,
	0, 0, 0,
	VE_K_KP_COMMA,
	VE_K_KP_EQUALSAS400,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	VE_K_ALTERASE,
	VE_K_SYSREQ,
	VE_K_CANCEL,
	VE_K_CLEAR,
	VE_K_PRIOR,
	VE_K_RETURN2,
	VE_K_SEPARATOR,
	VE_K_OUT,
	VE_K_OPER,
	VE_K_CLEARAGAIN,
	VE_K_CRSEL,
	VE_K_EXSEL,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	VE_K_KP_00,
	VE_K_KP_000,
	VE_K_THOUSANDSSEPARATOR,
	VE_K_DECIMALSEPARATOR,
	VE_K_CURRENCYUNIT,
	VE_K_CURRENCYSUBUNIT,
	VE_K_KP_LEFTPAREN,
	VE_K_KP_RIGHTPAREN,
	VE_K_KP_LEFTBRACE,
	VE_K_KP_RIGHTBRACE,
	VE_K_KP_TAB,
	VE_K_KP_BACKSPACE,
	VE_K_KP_A,
	VE_K_KP_B,
	VE_K_KP_C,
	VE_K_KP_D,
	VE_K_KP_E,
	VE_K_KP_F,
	VE_K_KP_XOR,
	VE_K_KP_POWER,
	VE_K_KP_PERCENT,
	VE_K_KP_LESS,
	VE_K_KP_GREATER,
	VE_K_KP_AMPERSAND,
	VE_K_KP_DBLAMPERSAND,
	VE_K_KP_VERTICALBAR,
	VE_K_KP_DBLVERTICALBAR,
	VE_K_KP_COLON,
	VE_K_KP_HASH,
	VE_K_KP_SPACE,
	VE_K_KP_AT,
	VE_K_KP_EXCLAM,
	VE_K_KP_MEMSTORE,
	VE_K_KP_MEMRECALL,
	VE_K_KP_MEMCLEAR,
	VE_K_KP_MEMADD,
	VE_K_KP_MEMSUBTRACT,
	VE_K_KP_MEMMULTIPLY,
	VE_K_KP_MEMDIVIDE,
	VE_K_KP_PLUSMINUS,
	VE_K_KP_CLEAR,
	VE_K_KP_CLEARENTRY,
	VE_K_KP_BINARY,
	VE_K_KP_OCTAL,
	VE_K_KP_DECIMAL,
	VE_K_KP_HEXADECIMAL,
	0, 0,
	VE_K_LCTRL,
	VE_K_LSHIFT,
	VE_K_LALT,
	VE_K_LGUI,
	VE_K_RCTRL,
	VE_K_RSHIFT,
	VE_K_RALT,
	VE_K_RGUI,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	VE_K_MODE,
	VE_K_AUDIONEXT,
	VE_K_AUDIOPREV,
	VE_K_AUDIOSTOP,
	VE_K_AUDIOPLAY,
	VE_K_AUDIOMUTE,
	VE_K_MEDIASELECT,
	VE_K_WWW,
	VE_K_MAIL,
	VE_K_CALCULATOR,
	VE_K_COMPUTER,
	VE_K_AC_SEARCH,
	VE_K_AC_HOME,
	VE_K_AC_BACK,
	VE_K_AC_FORWARD,
	VE_K_AC_STOP,
	VE_K_AC_REFRESH,
	VE_K_AC_BOOKMARKS,
	VE_K_BRIGHTNESSDOWN,
	VE_K_BRIGHTNESSUP,
	VE_K_DISPLAYSWITCH,
	VE_K_KBDILLUMTOGGLE,
	VE_K_KBDILLUMDOWN,
	VE_K_KBDILLUMUP,
	VE_K_EJECT,
	VE_K_SLEEP,
};
//--------------------------------------------------------------------------
static const VeChar8* s_ac8ScancodeNames[VE_NUM_SCANCODES] =
{
	nullptr, nullptr, nullptr, nullptr,
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"0",
	"Return",
	"Escape",
	"Backspace",
	"Tab",
	"Space",
	"-",
	"=",
	"[",
	"]",
	"\\",
	"#",
	";",
	"'",
	"`",
	",",
	".",
	"/",
	"CapsLock",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"PrintScreen",
	"ScrollLock",
	"Pause",
	"Insert",
	"Home",
	"PageUp",
	"Delete",
	"End",
	"PageDown",
	"Right",
	"Left",
	"Down",
	"Up",
	"Numlock",
	"Keypad /",
	"Keypad *",
	"Keypad -",
	"Keypad +",
	"Keypad Enter",
	"Keypad 1",
	"Keypad 2",
	"Keypad 3",
	"Keypad 4",
	"Keypad 5",
	"Keypad 6",
	"Keypad 7",
	"Keypad 8",
	"Keypad 9",
	"Keypad 0",
	"Keypad .",
	nullptr,
	"Application",
	"Power",
	"Keypad =",
	"F13",
	"F14",
	"F15",
	"F16",
	"F17",
	"F18",
	"F19",
	"F20",
	"F21",
	"F22",
	"F23",
	"F24",
	"Execute",
	"Help",
	"Menu",
	"Select",
	"Stop",
	"Again",
	"Undo",
	"Cut",
	"Copy",
	"Paste",
	"Find",
	"Mute",
	"VolumeUp",
	"VolumeDown",
	nullptr, nullptr, nullptr,
	"Keypad ,",
	"Keypad = (AS400)",
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	"AltErase",
	"SysReq",
	"Cancel",
	"Clear",
	"Prior",
	"Return",
	"Separator",
	"Out",
	"Oper",
	"Clear / Again",
	"CrSel",
	"ExSel",
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	"Keypad 00",
	"Keypad 000",
	"ThousandsSeparator",
	"DecimalSeparator",
	"CurrencyUnit",
	"CurrencySubUnit",
	"Keypad (",
	"Keypad )",
	"Keypad {",
	"Keypad }",
	"Keypad Tab",
	"Keypad Backspace",
	"Keypad A",
	"Keypad B",
	"Keypad C",
	"Keypad D",
	"Keypad E",
	"Keypad F",
	"Keypad XOR",
	"Keypad ^",
	"Keypad %",
	"Keypad <",
	"Keypad >",
	"Keypad &",
	"Keypad &&",
	"Keypad |",
	"Keypad ||",
	"Keypad :",
	"Keypad #",
	"Keypad Space",
	"Keypad @",
	"Keypad !",
	"Keypad MemStore",
	"Keypad MemRecall",
	"Keypad MemClear",
	"Keypad MemAdd",
	"Keypad MemSubtract",
	"Keypad MemMultiply",
	"Keypad MemDivide",
	"Keypad +/-",
	"Keypad Clear",
	"Keypad ClearEntry",
	"Keypad Binary",
	"Keypad Octal",
	"Keypad Decimal",
	"Keypad Hexadecimal",
	nullptr, nullptr,
	"Left Ctrl",
	"Left Shift",
	"Left Alt",
	"Left GUI",
	"Right Ctrl",
	"Right Shift",
	"Right Alt",
	"Right GUI",
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr,
	"ModeSwitch",
	"AudioNext",
	"AudioPrev",
	"AudioStop",
	"AudioPlay",
	"AudioMute",
	"MediaSelect",
	"WWW",
	"Mail",
	"Calculator",
	"Computer",
	"AC Search",
	"AC Home",
	"AC Back",
	"AC Forward",
	"AC Stop",
	"AC Refresh",
	"AC Bookmarks",
	"BrightnessDown",
	"BrightnessUp",
	"DisplaySwitch",
	"KBDIllumToggle",
	"KBDIllumDown",
	"KBDIllumUp",
	"Eject",
	"Sleep",
};
//--------------------------------------------------------------------------
VeKeyboard::VeKeyboard() noexcept
{
	
}
//--------------------------------------------------------------------------
VeKeyboard::~VeKeyboard() noexcept
{

}
//--------------------------------------------------------------------------
void VeKeyboard::Init() noexcept
{
	VeZeroMemory(m_au8KeyState, sizeof(m_au8KeyState));
	VeMemoryCopy(m_i32KeyMap, s_ai32DefaultKeymap, sizeof(s_ai32DefaultKeymap));
}
//--------------------------------------------------------------------------
void VeKeyboard::Term() noexcept
{

}
//--------------------------------------------------------------------------
void VeKeyboard::Reset() noexcept
{
	for (VeInt32 i(0); i < VE_NUM_SCANCODES; ++i)
	{
		if (m_au8KeyState[i] == VE_RELEASED)
		{
			SendKey(VE_RELEASED, (VeScancode)i);
		}
	}
}
//--------------------------------------------------------------------------
void VeKeyboard::SetKeymap(VeUInt32 u32Start, VeKeycode* pkKeys,
	VeUInt32 u32Lenth) noexcept
{
	VE_ASSERT(u32Start + u32Lenth <= VE_NUM_SCANCODES);
	VeMemoryCopy(&m_i32KeyMap[u32Start], pkKeys, sizeof(VeKeycode) * u32Lenth);
}
//--------------------------------------------------------------------------
void VeKeyboard::SetScancodeName(VeScancode eScancode,
	const VeChar8* pcName) noexcept
{
	s_ac8ScancodeNames[eScancode] = pcName;
}
//--------------------------------------------------------------------------
void VeKeyboard::SetFocus(VeWindow::Data* pkWindow) noexcept
{
	VE_ASSERT(ve_video_ptr);
	if (m_pkFocus && !pkWindow)
	{
		Reset();
	}

	if (m_pkFocus && m_pkFocus != pkWindow)
	{
		ve_video_ptr->SendWindowEvent(m_pkFocus,
			VE_WINDOWEVENT_FOCUS_LOST, 0, 0);

		/*if (SDL_EventState(SDL_TEXTINPUT, SDL_QUERY)) {
			SDL_VideoDevice *video = SDL_GetVideoDevice();
			if (video && video->StopTextInput) {
				video->StopTextInput(video);
			}
		}*/
	}

	m_pkFocus = pkWindow;

	if (m_pkFocus)
	{
		ve_video_ptr->SendWindowEvent(m_pkFocus,
			VE_WINDOWEVENT_FOCUS_GAINED, 0, 0);

		/*if (SDL_EventState(SDL_TEXTINPUT, SDL_QUERY)) {
			SDL_VideoDevice *video = SDL_GetVideoDevice();
			if (video && video->StartTextInput) {
				video->StartTextInput(video);
			}
		}*/
	}
}
//--------------------------------------------------------------------------
void VeKeyboard::SendKey(VeUInt8 u8State,
	VeScancode eScancode) noexcept
{
	VeUInt16 modstate;
	VeUInt32 type;
	VeUInt8 repeat;

	if (!eScancode) return;

	if (eScancode == VE_PRESSED)
	{
		modstate = m_u16ModState;
		switch (eScancode)
		{
		case VE_SCANCODE_NUMLOCKCLEAR:
			m_u16ModState ^= VE_KMOD_NUM;
			break;
		case VE_SCANCODE_CAPSLOCK:
			m_u16ModState ^= VE_KMOD_CAPS;
			break;
		case VE_SCANCODE_LCTRL:
			m_u16ModState |= VE_KMOD_LCTRL;
			break;
		case VE_SCANCODE_RCTRL:
			m_u16ModState |= VE_KMOD_RCTRL;
			break;
		case VE_SCANCODE_LSHIFT:
			m_u16ModState |= VE_KMOD_LSHIFT;
			break;
		case VE_SCANCODE_RSHIFT:
			m_u16ModState |= VE_KMOD_RSHIFT;
			break;
		case VE_SCANCODE_LALT:
			m_u16ModState |= VE_KMOD_LALT;
			break;
		case VE_SCANCODE_RALT:
			m_u16ModState |= VE_KMOD_RALT;
			break;
		case VE_SCANCODE_LGUI:
			m_u16ModState |= VE_KMOD_LGUI;
			break;
		case VE_SCANCODE_RGUI:
			m_u16ModState |= VE_KMOD_RGUI;
			break;
		case VE_SCANCODE_MODE:
			m_u16ModState |= VE_KMOD_MODE;
			break;
		default:
			break;
		}
	}
	else
	{
		switch (eScancode)
		{
		case VE_SCANCODE_NUMLOCKCLEAR:
		case VE_SCANCODE_CAPSLOCK:
			break;
		case VE_SCANCODE_LCTRL:
			m_u16ModState &= ~VE_KMOD_LCTRL;
			break;
		case VE_SCANCODE_RCTRL:
			m_u16ModState &= ~VE_KMOD_RCTRL;
			break;
		case VE_SCANCODE_LSHIFT:
			m_u16ModState &= ~VE_KMOD_LSHIFT;
			break;
		case VE_SCANCODE_RSHIFT:
			m_u16ModState &= ~VE_KMOD_RSHIFT;
			break;
		case VE_SCANCODE_LALT:
			m_u16ModState &= ~VE_KMOD_LALT;
			break;
		case VE_SCANCODE_RALT:
			m_u16ModState &= ~VE_KMOD_RALT;
			break;
		case VE_SCANCODE_LGUI:
			m_u16ModState &= ~VE_KMOD_LGUI;
			break;
		case VE_SCANCODE_RGUI:
			m_u16ModState &= ~VE_KMOD_RGUI;
			break;
		case VE_SCANCODE_MODE:
			m_u16ModState &= ~VE_KMOD_MODE;
			break;
		default:
			break;
		}
		modstate = m_u16ModState;
	}

	switch (u8State)
	{
	case VE_PRESSED:
		type = VE_KEYDOWN;
		break;
	case VE_RELEASED:
		type = VE_KEYUP;
		break;
	default:
		return;
	}

	repeat = (u8State && m_au8KeyState[eScancode]);
	if (m_au8KeyState[eScancode] == u8State && !repeat)
	{
		return;
	}

	m_au8KeyState[eScancode] = u8State;

	VE_ASSERT(ve_event_queue_ptr);
	if (ve_event_queue_ptr->IsEventTypeEnable(type))
	{
		VeEvent* pkEvent = ve_event_queue_ptr->AddEvent();
		pkEvent->m_kKey.m_u32Type = type;
		pkEvent->m_kKey.m_u8State = u8State;
		pkEvent->m_kKey.m_u8Repeat = repeat;
		pkEvent->m_kKey.m_kKeysym.m_eScancode = eScancode;
		pkEvent->m_kKey.m_kKeysym.m_i32Sym = m_i32KeyMap[eScancode];
		pkEvent->m_kKey.m_kKeysym.m_u16Mod = modstate;
		pkEvent->m_kKey.m_u32WindowID = m_pkFocus ? m_pkFocus->m_u32Id : 0;
	}
}
//--------------------------------------------------------------------------
void VeKeyboard::GetDefaultKeymap(VeKeycode* pkKeymap) noexcept
{
	VE_ASSERT(pkKeymap);
	VeMemoryCopy(pkKeymap, s_ai32DefaultKeymap, sizeof(s_ai32DefaultKeymap));
}
//--------------------------------------------------------------------------
