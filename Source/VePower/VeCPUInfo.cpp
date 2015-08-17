////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Source File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeCPUInfo.cpp
//  Created:     2015/08/17 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#include "VePowerPch.h"
#if defined(_MSC_VER) && (_MSC_VER >= 1500) && (defined(_M_IX86) || defined(_M_X64))
#include <intrin.h>
#ifndef _WIN64
#define __MMX__
#define __3dNOW__
#endif
#define __SSE__
#define __SSE2__
#elif defined(__MINGW64_VERSION_MAJOR)
#include <intrin.h>
#else
#ifdef __ALTIVEC__
#if HAVE_ALTIVEC_H && !defined(__APPLE_ALTIVEC__)
#include <altivec.h>
#undef pixel
#endif
#endif
#ifdef __MMX__
#include <mmintrin.h>
#endif
#ifdef __3dNOW__
#include <mm3dnow.h>
#endif
#ifdef __SSE__
#include <xmmintrin.h>
#endif
#ifdef __SSE2__
#include <emmintrin.h>
#endif
#endif

//--------------------------------------------------------------------------
#define CPU_HAS_RDTSC   0x00000001
#define CPU_HAS_ALTIVEC 0x00000002
#define CPU_HAS_MMX     0x00000004
#define CPU_HAS_3DNOW   0x00000008
#define CPU_HAS_SSE     0x00000010
#define CPU_HAS_SSE2    0x00000020
#define CPU_HAS_SSE3    0x00000040
#define CPU_HAS_SSE41   0x00000100
#define CPU_HAS_SSE42   0x00000200
#define CPU_HAS_AVX     0x00000400
//--------------------------------------------------------------------------
static inline VE_BOOL CPU_haveCPUID() noexcept
{
	VE_BOOL has_CPUID = 0;
#if defined(__GNUC__) && defined(i386)
	__asm__(
		"        pushfl                      # Get original EFLAGS             \n"
		"        popl    %%eax                                                 \n"
		"        movl    %%eax,%%ecx                                           \n"
		"        xorl    $0x200000,%%eax     # Flip ID bit in EFLAGS           \n"
		"        pushl   %%eax               # Save new EFLAGS value on stack  \n"
		"        popfl                       # Replace current EFLAGS value    \n"
		"        pushfl                      # Get new EFLAGS                  \n"
		"        popl    %%eax               # Store new EFLAGS in EAX         \n"
		"        xorl    %%ecx,%%eax         # Can not toggle ID bit,          \n"
		"        jz      1f                  # Processor=80486                 \n"
		"        movl    $1,%0               # We have CPUID support           \n"
		"1:                                                                    \n"
		: "=m" (has_CPUID)
		:
		: "%eax", "%ecx"
		);
#elif defined(__GNUC__) && defined(__x86_64__)
	__asm__(
		"        pushfq                      # Get original EFLAGS             \n"
		"        popq    %%rax                                                 \n"
		"        movq    %%rax,%%rcx                                           \n"
		"        xorl    $0x200000,%%eax     # Flip ID bit in EFLAGS           \n"
		"        pushq   %%rax               # Save new EFLAGS value on stack  \n"
		"        popfq                       # Replace current EFLAGS value    \n"
		"        pushfq                      # Get new EFLAGS                  \n"
		"        popq    %%rax               # Store new EFLAGS in EAX         \n"
		"        xorl    %%ecx,%%eax         # Can not toggle ID bit,          \n"
		"        jz      1f                  # Processor=80486                 \n"
		"        movl    $1,%0               # We have CPUID support           \n"
		"1:                                                                    \n"
		: "=m" (has_CPUID)
		:
		: "%rax", "%rcx"
		);
#elif (defined(_MSC_VER) && defined(_M_IX86)) || defined(__WATCOMC__)
	__asm {
		pushfd; Get original EFLAGS
			pop     eax
			mov     ecx, eax
			xor     eax, 200000h; Flip ID bit in EFLAGS
			push    eax; Save new EFLAGS value on stack
			popfd; Replace current EFLAGS value
			pushfd; Get new EFLAGS
			pop     eax; Store new EFLAGS in EAX
			xor     eax, ecx; Can not toggle ID bit,
			jz      done; Processor = 80486
			mov     has_CPUID, 1; We have CPUID support
			done :
	}
#elif defined(_MSC_VER) && defined(_M_X64)
	has_CPUID = 1;
#elif defined(__sun) && defined(__i386)
	__asm (
	"       pushfl                 \n"
		"       popl    %eax           \n"
		"       movl    %eax,%ecx      \n"
		"       xorl    $0x200000,%eax \n"
		"       pushl   %eax           \n"
		"       popfl                  \n"
		"       pushfl                 \n"
		"       popl    %eax           \n"
		"       xorl    %ecx,%eax      \n"
		"       jz      1f             \n"
		"       movl    $1,-8(%ebp)    \n"
		"1:                            \n"
		);
#elif defined(__sun) && defined(__amd64)
	__asm (
	"       pushfq                 \n"
		"       popq    %rax           \n"
		"       movq    %rax,%rcx      \n"
		"       xorl    $0x200000,%eax \n"
		"       pushq   %rax           \n"
		"       popfq                  \n"
		"       pushfq                 \n"
		"       popq    %rax           \n"
		"       xorl    %ecx,%eax      \n"
		"       jz      1f             \n"
		"       movl    $1,-8(%rbp)    \n"
		"1:                            \n"
		);
#endif
	return has_CPUID;
}
//--------------------------------------------------------------------------
#if defined(__GNUC__) && defined(i386)
#define cpuid(func, a, b, c, d) \
    __asm__ __volatile__ ( \
"        pushl %%ebx        \n" \
"        cpuid              \n" \
"        movl %%ebx, %%esi  \n" \
"        popl %%ebx         \n" : \
            "=a" (a), "=S" (b), "=c" (c), "=d" (d) : "a" (func))
#elif defined(__GNUC__) && defined(__x86_64__)
#define cpuid(func, a, b, c, d) \
    __asm__ __volatile__ ( \
"        pushq %%rbx        \n" \
"        cpuid              \n" \
"        movq %%rbx, %%rsi  \n" \
"        popq %%rbx         \n" : \
            "=a" (a), "=S" (b), "=c" (c), "=d" (d) : "a" (func))
#elif (defined(_MSC_VER) && defined(_M_IX86)) || defined(__WATCOMC__)
#define cpuid(func, a, b, c, d) \
    __asm { \
        __asm mov eax, func \
        __asm cpuid \
        __asm mov a, eax \
        __asm mov b, ebx \
        __asm mov c, ecx \
        __asm mov d, edx \
}
#elif defined(_MSC_VER) && defined(_M_X64)
#define cpuid(func, a, b, c, d) \
{ \
    VeInt32 CPUInfo[4]; \
    __cpuid(CPUInfo, func); \
    a = CPUInfo[0]; \
    b = CPUInfo[1]; \
    c = CPUInfo[2]; \
    d = CPUInfo[3]; \
}
#else
#define cpuid(func, a, b, c, d) \
    a = b = c = d = 0
#endif
//--------------------------------------------------------------------------
static inline VeInt32 CPU_getCPUIDFeatures() noexcept
{
	VeInt32 features = 0;
	VeInt32 a, b, c, d;

	cpuid(0, a, b, c, d);
	if (a >= 1) {
		cpuid(1, a, b, c, d);
		features = d;
	}
	return features;
}
//--------------------------------------------------------------------------
static inline VeInt32 CPU_haveRDTSC() noexcept
{
	if (CPU_haveCPUID())
	{
		return (CPU_getCPUIDFeatures() & 0x00000010);
	}
	return 0;
}
//--------------------------------------------------------------------------
static inline VeInt32 CPU_haveAltiVec() noexcept
{
	return 0;
}
//--------------------------------------------------------------------------
static inline VeInt32 CPU_haveMMX() noexcept
{
	if (CPU_haveCPUID())
	{
		return (CPU_getCPUIDFeatures() & 0x00800000);
	}
	return 0;
}
//--------------------------------------------------------------------------
static inline VeInt32 CPU_have3DNow() noexcept
{
	if (CPU_haveCPUID())
	{
		VeInt32 a, b, c, d;
		cpuid(0x80000000, a, b, c, d);
		if (a >= 0x80000001)
		{
			cpuid(0x80000001, a, b, c, d);
			return (d & 0x80000000);
		}
	}
	return 0;
}
//--------------------------------------------------------------------------
static inline VeInt32 CPU_haveSSE() noexcept
{
	if (CPU_haveCPUID())
	{
		return (CPU_getCPUIDFeatures() & 0x02000000);
	}
	return 0;
}
//--------------------------------------------------------------------------
static inline VeInt32 CPU_haveSSE2() noexcept
{
	if (CPU_haveCPUID())
	{
		return (CPU_getCPUIDFeatures() & 0x04000000);
	}
	return 0;
}
//--------------------------------------------------------------------------
static inline VeInt32 CPU_haveSSE3() noexcept
{
	if (CPU_haveCPUID())
	{
		VeInt32 a, b, c, d;

		cpuid(0, a, b, c, d);
		if (a >= 1)
		{
			cpuid(1, a, b, c, d);
			return (c & 0x00000001);
		}
	}
	return 0;
}
//--------------------------------------------------------------------------
static inline VeInt32 CPU_haveSSE41() noexcept
{
	if (CPU_haveCPUID())
	{
		VeInt32 a, b, c, d;

		cpuid(1, a, b, c, d);
		if (a >= 1)
		{
			cpuid(1, a, b, c, d);
			return (c & 0x00080000);
		}
	}
	return 0;
}
//--------------------------------------------------------------------------
static inline VeInt32 CPU_haveSSE42() noexcept
{
	if (CPU_haveCPUID())
	{
		VeInt32 a, b, c, d;

		cpuid(1, a, b, c, d);
		if (a >= 1)
		{
			cpuid(1, a, b, c, d);
			return (c & 0x00100000);
		}
	}
	return 0;
}
//--------------------------------------------------------------------------
static inline VeInt32 CPU_haveAVX() noexcept
{
	if (CPU_haveCPUID())
	{
		VeInt32 a, b, c, d;

		cpuid(1, a, b, c, d);
		if (a >= 1)
		{
			cpuid(1, a, b, c, d);
			return (c & 0x10000000);
		}
	}
	return 0;
}
//--------------------------------------------------------------------------
VeInt32 VeCPUInfo::ms_i32CPUCount = 0;
//--------------------------------------------------------------------------
VeInt32 VeCPUInfo::ms_i32CPUFeatures = 0xFFFFFFFF;
//--------------------------------------------------------------------------
VeInt32 VeCPUInfo::ms_i32SystemRAM = 0;
//--------------------------------------------------------------------------
VeInt32 VeCPUInfo::GetCount() noexcept
{
	if (!ms_i32CPUCount)
	{
#		ifdef VE_PLATFORM_WIN
		if (ms_i32CPUCount <= 0)
		{
			SYSTEM_INFO kInfo;
			GetSystemInfo(&kInfo);
			ms_i32CPUCount = kInfo.dwNumberOfProcessors;
		}
#		endif

		if (ms_i32CPUCount <= 0)
		{
			ms_i32CPUCount = 1;
		}
	}
	return ms_i32CPUCount;
}
//--------------------------------------------------------------------------
static inline const VeChar8* GetCPUType() noexcept
{
	static VeChar8 s_acCPUType[13];

	if (!s_acCPUType[0])
	{
		VeInt32 i = 0;
		VeInt32 a, b, c, d;

		if (CPU_haveCPUID())
		{
			cpuid(0x00000000, a, b, c, d);
			s_acCPUType[i++] = (char)(b & 0xff); b >>= 8;
			s_acCPUType[i++] = (char)(b & 0xff); b >>= 8;
			s_acCPUType[i++] = (char)(b & 0xff); b >>= 8;
			s_acCPUType[i++] = (char)(b & 0xff); b >>= 8;
			s_acCPUType[i++] = (char)(d & 0xff); d >>= 8;
			s_acCPUType[i++] = (char)(d & 0xff); d >>= 8;
			s_acCPUType[i++] = (char)(d & 0xff); d >>= 8;
			s_acCPUType[i++] = (char)(d & 0xff); d >>= 8;
			s_acCPUType[i++] = (char)(c & 0xff); c >>= 8;
			s_acCPUType[i++] = (char)(c & 0xff); c >>= 8;
			s_acCPUType[i++] = (char)(c & 0xff); c >>= 8;
			s_acCPUType[i++] = (char)(c & 0xff); c >>= 8;
		}
		if (!s_acCPUType[0])
		{
			VeStrcpy(s_acCPUType, "Unknown");
		}
	}
	return s_acCPUType;
}
//--------------------------------------------------------------------------
VeInt32 VeCPUInfo::GetCacheLineSize() noexcept
{
	const VeChar8* cpuType = GetCPUType();

	if (VeStrcmp(cpuType, "GenuineIntel") == 0)
	{
		VeInt32 a, b, c, d;

		cpuid(0x00000001, a, b, c, d);
		return (((b >> 8) & 0xff) * 8);
	}
	else if (VeStrcmp(cpuType, "AuthenticAMD") == 0)
	{
		int a, b, c, d;

		cpuid(0x80000005, a, b, c, d);
		return (c & 0xff);
	}
	else
	{
		return VE_CACHELINE_SIZE;
	}

	return 0;
}
//--------------------------------------------------------------------------
VeUInt32 VeCPUInfo::GetCPUFeatures() noexcept
{
	if (ms_i32CPUFeatures == 0xFFFFFFFF)
	{
		ms_i32CPUFeatures = 0;
		if (CPU_haveRDTSC())
		{
			ms_i32CPUFeatures |= CPU_HAS_RDTSC;
		}
		if (CPU_haveMMX())
		{
			ms_i32CPUFeatures |= CPU_HAS_MMX;
		}
		if (CPU_have3DNow())
		{
			ms_i32CPUFeatures |= CPU_HAS_3DNOW;
		}
		if (CPU_haveSSE())
		{
			ms_i32CPUFeatures |= CPU_HAS_SSE;
		}
		if (CPU_haveSSE2())
		{
			ms_i32CPUFeatures |= CPU_HAS_SSE2;
		}
		if (CPU_haveSSE3())
		{
			ms_i32CPUFeatures |= CPU_HAS_SSE3;
		}
		if (CPU_haveSSE41())
		{
			ms_i32CPUFeatures |= CPU_HAS_SSE41;
		}
		if (CPU_haveSSE42())
		{
			ms_i32CPUFeatures |= CPU_HAS_SSE42;
		}
		if (CPU_haveAVX())
		{
			ms_i32CPUFeatures |= CPU_HAS_AVX;
		}
	}
	return ms_i32CPUFeatures;
}
//--------------------------------------------------------------------------
bool VeCPUInfo::HasRDTSC() noexcept
{
	return GetCPUFeatures() & CPU_HAS_RDTSC;
}
//--------------------------------------------------------------------------
bool VeCPUInfo::HasMMX() noexcept
{
	return GetCPUFeatures() & CPU_HAS_MMX;
}
//--------------------------------------------------------------------------
bool VeCPUInfo::Has3DNow() noexcept
{
	return GetCPUFeatures() & CPU_HAS_3DNOW;
}
//--------------------------------------------------------------------------
bool VeCPUInfo::HasSSE() noexcept
{
	return GetCPUFeatures() & CPU_HAS_SSE;
}
//--------------------------------------------------------------------------
bool VeCPUInfo::HasSSE2() noexcept
{
	return GetCPUFeatures() & CPU_HAS_SSE2;
}
//--------------------------------------------------------------------------
bool VeCPUInfo::HasSSE3() noexcept
{
	return GetCPUFeatures() & CPU_HAS_SSE3;
}
//--------------------------------------------------------------------------
bool VeCPUInfo::HasSSE41() noexcept
{
	return GetCPUFeatures() & CPU_HAS_SSE41;
}
//--------------------------------------------------------------------------
bool VeCPUInfo::HasSSE42() noexcept
{
	return GetCPUFeatures() & CPU_HAS_SSE42;
}
//--------------------------------------------------------------------------
bool VeCPUInfo::HasAVX() noexcept
{
	return GetCPUFeatures() & CPU_HAS_AVX;
}
//--------------------------------------------------------------------------
VeInt32 VeCPUInfo::GetSystemRAM() noexcept
{
	if (!ms_i32SystemRAM)
	{
#		ifdef VE_PLATFORM_WIN
		if (ms_i32SystemRAM <= 0)
		{
			MEMORYSTATUSEX stat;
			stat.dwLength = sizeof(stat);
			if (GlobalMemoryStatusEx(&stat))
			{
				ms_i32SystemRAM = (VeInt32)(stat.ullTotalPhys / (1024 * 1024));
			}
		}
#		endif
	}
	return ms_i32SystemRAM;
}
//--------------------------------------------------------------------------
