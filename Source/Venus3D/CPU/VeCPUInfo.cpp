////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//  Copyright (c) 2016 Venus3D
// -------------------------------------------------------------------------
//  Module:      Venus3D
//  File name:   VeCPUInfo.cpp
//  Created:     2016/07/01 by Albert
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

//--------------------------------------------------------------------------
static bool HaveCPUID() noexcept
{
	int iHasCPUID = 0;
	/* *INDENT-OFF* */
#ifndef SDL_CPUINFO_DISABLED
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
		: "=m" (iHasCPUID)
		:
		: "%eax", "%ecx"
	);
#elif defined(__GNUC__) && defined(__x86_64__)
	/* Technically, if this is being compiled under __x86_64__ then it has
	CPUid by definition.  But it's nice to be able to prove it.  :)      */
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
		: "=m" (iHasCPUID)
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
		mov     iHasCPUID, 1; We have CPUID support
		done :
	}
#elif defined(_MSC_VER) && defined(_M_X64)
	iHasCPUID = 1;
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
#endif
	/* *INDENT-ON* */
	return iHasCPUID ? true : false;
}
//--------------------------------------------------------------------------
#if defined(__GNUC__) && defined(i386)
#define cpuid(func, a, b, c, d) \
    __asm__ __volatile__ ( \
"        pushl %%ebx        \n" \
"        xorl %%ecx,%%ecx   \n" \
"        cpuid              \n" \
"        movl %%ebx, %%esi  \n" \
"        popl %%ebx         \n" : \
            "=a" (a), "=S" (b), "=c" (c), "=d" (d) : "a" (func))
#elif defined(__GNUC__) && defined(__x86_64__)
#define cpuid(func, a, b, c, d) \
    __asm__ __volatile__ ( \
"        pushq %%rbx        \n" \
"        xorq %%rcx,%%rcx   \n" \
"        cpuid              \n" \
"        movq %%rbx, %%rsi  \n" \
"        popq %%rbx         \n" : \
            "=a" (a), "=S" (b), "=c" (c), "=d" (d) : "a" (func))
#elif (defined(_MSC_VER) && defined(_M_IX86)) || defined(__WATCOMC__)
#define cpuid(func, a, b, c, d) \
    __asm { \
        __asm mov eax, func \
        __asm xor ecx, ecx \
        __asm cpuid \
        __asm mov a, eax \
        __asm mov b, ebx \
        __asm mov c, ecx \
        __asm mov d, edx \
}
#elif defined(_MSC_VER) && defined(_M_X64)
#define cpuid(func, a, b, c, d) \
{ \
    int CPUInfo[4]; \
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
static int GetCPUIDFeatures() noexcept
{
	int iFeatures = 0;
	int a, b, c, d;
	cpuid(0, a, b, c, d);
	if (a >= 1) {
		cpuid(1, a, b, c, d);
		iFeatures = d;
	}
	return iFeatures;
}
//--------------------------------------------------------------------------
static bool OSSavesYMM() noexcept
{
	int a, b, c, d;

	/* Check to make sure we can call xgetbv */
	cpuid(0, a, b, c, d);
	if (a < 1) {
		return false;
	}
	cpuid(1, a, b, c, d);
	if (!(c & 0x08000000)) {
		return false;
	}

	/* Call xgetbv to see if YMM register state is saved */
	a = 0;
#if defined(__GNUC__) && (defined(i386) || defined(__x86_64__))
	asm(".byte 0x0f, 0x01, 0xd0" : "=a" (a) : "c" (0) : "%edx");
#elif defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64)) && (_MSC_FULL_VER >= 160040219) /* VS2010 SP1 */
	a = (int)_xgetbv(0);
#elif (defined(_MSC_VER) && defined(_M_IX86)) || defined(__WATCOMC__)
	__asm
	{
		xor ecx, ecx
		_asm _emit 0x0f _asm _emit 0x01 _asm _emit 0xd0
		mov a, eax
	}
#endif
	return ((a & 6) == 6) ? true : false;
}
//--------------------------------------------------------------------------
static int HaveRDTSC() noexcept
{
	if (HaveCPUID()) {
		return (GetCPUIDFeatures() & 0x00000010);
	}
	return 0;
}
//--------------------------------------------------------------------------
static int HaveAltiVec() noexcept
{
	volatile int iAltiVec = 0;
#ifndef SDL_CPUINFO_DISABLED
#if (defined(__MACOSX__) && (defined(__ppc__) || defined(__ppc64__))) || (defined(__OpenBSD__) && defined(__powerpc__))
#ifdef __OpenBSD__
	int selectors[2] = { CTL_MACHDEP, CPU_ALTIVEC };
#else
	int selectors[2] = { CTL_HW, HW_VECTORUNIT };
#endif
	int hasVectorUnit = 0;
	size_t length = sizeof(hasVectorUnit);
	int error = sysctl(selectors, 2, &hasVectorUnit, &length, NULL, 0);
	if (0 == error)
		iAltiVec = (hasVectorUnit != 0);
#elif SDL_ALTIVEC_BLITTERS && HAVE_SETJMP
	void(*handler) (int sig);
	handler = signal(SIGILL, illegal_instruction);
	if (setjmp(jmpbuf) == 0) {
		asm volatile ("mtspr 256, %0\n\t" "vand %%v0, %%v0, %%v0"::"r" (-1));
		iAltiVec = 1;
	}
	signal(SIGILL, handler);
#endif
#endif
	return iAltiVec;
}
//--------------------------------------------------------------------------
static int HaveMMX() noexcept
{
	if (HaveCPUID()) {
		return (GetCPUIDFeatures() & 0x00800000);
	}
	return 0;
}
//--------------------------------------------------------------------------
static int Have3DNow() noexcept
{
	if (HaveCPUID()) {
		unsigned int a, b, c, d;

		cpuid(0x80000000, a, b, c, d);
		if (a >= 0x80000001) {
			cpuid(0x80000001, a, b, c, d);
			return (d & 0x80000000);
		}
	}
	return 0;
}
//--------------------------------------------------------------------------
static int HaveSSE() noexcept
{
	if (HaveCPUID()) {
		return (GetCPUIDFeatures() & 0x02000000);
	}
	return 0;
}
//--------------------------------------------------------------------------
static int HaveSSE2() noexcept
{
	if (HaveCPUID()) {
		return (GetCPUIDFeatures() & 0x04000000);
	}
	return 0;
}
//--------------------------------------------------------------------------
static int HaveSSE3() noexcept
{
	if (HaveCPUID()) {
		int a, b, c, d;

		cpuid(0, a, b, c, d);
		if (a >= 1) {
			cpuid(1, a, b, c, d);
			return (c & 0x00000001);
		}
	}
	return 0;
}
//--------------------------------------------------------------------------
static int HaveSSE41() noexcept
{
	if (HaveCPUID()) {
		int a, b, c, d;

		cpuid(0, a, b, c, d);
		if (a >= 1) {
			cpuid(1, a, b, c, d);
			return (c & 0x00080000);
		}
	}
	return 0;
}
//--------------------------------------------------------------------------
static int HaveSSE42() noexcept
{
	if (HaveCPUID()) {
		int a, b, c, d;

		cpuid(0, a, b, c, d);
		if (a >= 1) {
			cpuid(1, a, b, c, d);
			return (c & 0x00100000);
		}
	}
	return 0;
}
//--------------------------------------------------------------------------
static int HaveAVX() noexcept
{
	if (HaveCPUID() && OSSavesYMM()) {
		int a, b, c, d;

		cpuid(0, a, b, c, d);
		if (a >= 1) {
			cpuid(1, a, b, c, d);
			return (c & 0x10000000);
		}
	}
	return 0;
}
//--------------------------------------------------------------------------
static int HaveAVX2() noexcept
{
	if (HaveCPUID() && OSSavesYMM()) {
		int a, b, c, d;

		cpuid(0, a, b, c, d);
		if (a >= 7) {
			cpuid(7, a, b, c, d);
			return (b & 0x00000020);
		}
	}
	return 0;
}
//--------------------------------------------------------------------------
static int HaveF16C() noexcept
{
	if (HaveCPUID() && OSSavesYMM()) {
		int a, b, c, d;

		cpuid(0, a, b, c, d);
		if (a >= 1) {
			cpuid(1, a, b, c, d);
			return VE_MASK_HAS_ALL(c, 0x38080000);
		}
	}
	return 0;
}
//--------------------------------------------------------------------------
static const char * GetCPUType(void)
{
	static char acCPUType[13];

	if (!acCPUType[0]) {
		int i = 0;

		if (HaveCPUID()) {
			int a, b, c, d;
			cpuid(0x00000000, a, b, c, d);
			(void)a;
			acCPUType[i++] = (char)(b & 0xff); b >>= 8;
			acCPUType[i++] = (char)(b & 0xff); b >>= 8;
			acCPUType[i++] = (char)(b & 0xff); b >>= 8;
			acCPUType[i++] = (char)(b & 0xff);

			acCPUType[i++] = (char)(d & 0xff); d >>= 8;
			acCPUType[i++] = (char)(d & 0xff); d >>= 8;
			acCPUType[i++] = (char)(d & 0xff); d >>= 8;
			acCPUType[i++] = (char)(d & 0xff);

			acCPUType[i++] = (char)(c & 0xff); c >>= 8;
			acCPUType[i++] = (char)(c & 0xff); c >>= 8;
			acCPUType[i++] = (char)(c & 0xff); c >>= 8;
			acCPUType[i++] = (char)(c & 0xff);
		}
		if (!acCPUType[0]) {
			strcpy(acCPUType, "Unknown");
		}
	}
	return acCPUType;
}
//--------------------------------------------------------------------------
static unsigned int s_uiCPUCount = 0;
//--------------------------------------------------------------------------
unsigned int VeGetCPUCount() noexcept
{
	return std::thread::hardware_concurrency();
}
//--------------------------------------------------------------------------
constexpr unsigned int s_uiDefaultCacheLineSize = 128;
//--------------------------------------------------------------------------
unsigned int VeGetCPUCacheLineSize() noexcept
{
	const char * strCPUType = GetCPUType();
	int a, b, c, d;
	(void)a; (void)b; (void)c; (void)d;
	if (strcmp(strCPUType, "GenuineIntel") == 0) {
		cpuid(0x00000001, a, b, c, d);
		return (((b >> 8) & 0xff) * 8);
	}
	else if (strcmp(strCPUType, "AuthenticAMD") == 0) {
		cpuid(0x80000005, a, b, c, d);
		return (c & 0xff);
	}
	else {
		/* Just make a guess here... */
		return s_uiDefaultCacheLineSize;
	}
}
//--------------------------------------------------------------------------
static unsigned int s_uiCPUFeatures = 0xFFFFFFFF;
//--------------------------------------------------------------------------
unsigned int VeGetCPUFeatures() noexcept
{
	if (s_uiCPUFeatures == 0xFFFFFFFF) {
		s_uiCPUFeatures = 0;
		if (HaveRDTSC()) {
			s_uiCPUFeatures |= VE_CPU_HAS_RDTSC;
		}
		if (HaveAltiVec()) {
			s_uiCPUFeatures |= VE_CPU_HAS_ALTIVEC;
		}
		if (HaveMMX()) {
			s_uiCPUFeatures |= VE_CPU_HAS_MMX;
		}
		if (Have3DNow()) {
			s_uiCPUFeatures |= VE_CPU_HAS_3DNOW;
		}
		if (HaveSSE()) {
			s_uiCPUFeatures |= VE_CPU_HAS_SSE;
		}
		if (HaveSSE2()) {
			s_uiCPUFeatures |= VE_CPU_HAS_SSE2;
		}
		if (HaveSSE3()) {
			s_uiCPUFeatures |= VE_CPU_HAS_SSE3;
		}
		if (HaveSSE41()) {
			s_uiCPUFeatures |= VE_CPU_HAS_SSE41;
		}
		if (HaveSSE42()) {
			s_uiCPUFeatures |= VE_CPU_HAS_SSE42;
		}
		if (HaveAVX()) {
			s_uiCPUFeatures |= VE_CPU_HAS_AVX;
		}
		if (HaveAVX2()) {
			s_uiCPUFeatures |= VE_CPU_HAS_AVX2;
		}
		if (HaveF16C()) {
			s_uiCPUFeatures |= VE_CPU_HAS_F16C;
		}
	}
	return s_uiCPUFeatures;
}
//--------------------------------------------------------------------------
static unsigned int s_uiSystemRAM = 0;
//--------------------------------------------------------------------------
unsigned int VeGetSystemRAM() noexcept
{
	if (!s_uiSystemRAM) {
#if defined(HAVE_SYSCONF) && defined(_SC_PHYS_PAGES) && defined(_SC_PAGESIZE)
		if (s_uiSystemRAM <= 0) {
			s_uiSystemRAM = (int)((Sint64)sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGESIZE) / (1024 * 1024));
		}
#endif
#ifdef HAVE_SYSCTLBYNAME
		if (s_uiSystemRAM <= 0) {
#if defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__NetBSD__)
#ifdef HW_REALMEM
			int mib[2] = { CTL_HW, HW_REALMEM };
#else
			/* might only report up to 2 GiB */
			int mib[2] = { CTL_HW, HW_PHYSMEM };
#endif /* HW_REALMEM */
#else
			int mib[2] = { CTL_HW, HW_MEMSIZE };
#endif /* __FreeBSD__ || __FreeBSD_kernel__ */
			Uint64 memsize = 0;
			size_t len = sizeof(memsize);

			if (sysctl(mib, 2, &memsize, &len, NULL, 0) == 0) {
				s_uiSystemRAM = (int)(memsize / (1024 * 1024));
			}
		}
#endif
#ifdef WIN32
		if (s_uiSystemRAM <= 0) {
			MEMORYSTATUSEX stat;
			stat.dwLength = sizeof(stat);
			if (GlobalMemoryStatusEx(&stat)) {
				s_uiSystemRAM = (int)(stat.ullTotalPhys / (1024 * 1024));
			}
		}
#endif
	}
	return s_uiSystemRAM;
}
//--------------------------------------------------------------------------
