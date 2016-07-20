#include "stdafx.h"

int SDLCALL SDL_Init(Uint32 flags)
{
	return flags;
}


/* Get the name of the platform */
const char *
SDL_GetPlatform()
{
#if __AIX__
	return "AIX";
#elif __ANDROID__
	return "Android";
#elif __BSDI__
	return "BSDI";
#elif __DREAMCAST__
	return "Dreamcast";
#elif __EMSCRIPTEN__
	return "Emscripten";
#elif __FREEBSD__
	return "FreeBSD";
#elif __HAIKU__
	return "Haiku";
#elif __HPUX__
	return "HP-UX";
#elif __IRIX__
	return "Irix";
#elif __LINUX__
	return "Linux";
#elif __MINT__
	return "Atari MiNT";
#elif __MACOS__
	return "MacOS Classic";
#elif __MACOSX__
	return "Mac OS X";
#elif __NACL__
	return "NaCl";
#elif __NETBSD__
	return "NetBSD";
#elif __OPENBSD__
	return "OpenBSD";
#elif __OS2__
	return "OS/2";
#elif __OSF__
	return "OSF/1";
#elif __QNXNTO__
	return "QNX Neutrino";
#elif __RISCOS__
	return "RISC OS";
#elif __SOLARIS__
	return "Solaris";
#elif __WIN32__
	return "Windows";
#elif __WINRT__
	return "WinRT";
#elif __IPHONEOS__
	return "iOS";
#elif __PSP__
	return "PlayStation Portable";
#else
	return "Unknown (see SDL_platform.h)";
#endif
}
