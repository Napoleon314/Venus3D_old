/**********************************************************************
 *<
	FILE: gfloat

	DESCRIPTION: Single Precision Floating Point Routines

	CREATED BY: Don Brittain & Dan Silva

	HISTORY:

 *>	Copyright (c) 1994, All Rights Reserved.
 **********************************************************************/

#pragma once

// if on Intel, and not compiling managed code, use assembly language
#undef __USE_ASM_CODE_
#include <math.h>

#ifndef _MANAGED
	#ifdef _M_IX86
		#define __USE_ASM_CODE_
	#else
		#include <math.h>
	#endif
#endif

//-------------------------------------------------------------------------------
// Single precision floating point stuff...
//
/*! \remarks Returns both the sine and cosine of the specified angle as floats. This function uses the assembly language on Intel CPUs. */ 
inline void SinCos (float angle, float *sine, float *cosine) 
{
#ifdef __USE_ASM_CODE_
	__asm {
		push		ecx
        fld         dword ptr angle
        fsincos
		mov 		ecx, dword ptr[cosine]
        fstp        dword ptr [ecx]
		mov 		ecx, dword ptr[sine]
        fstp        dword ptr [ecx]
		pop			ecx
    }
#else
    *sine   = (float)sin (angle);
    *cosine = (float)cos (angle);
#endif
}

/*! \remarks Returns the sine of the specified angle as a float. This function uses the assembly language on Intel CPUs. */ 
inline float Sin(float angle)
{
#ifdef __USE_ASM_CODE_
	float s, c;
	SinCos(angle, &s, &c);
	return s;
#else
	return (float)sin((double)angle);
#endif
}

/*! \remarks Returns the cosine of the specified angle as a float. This function uses the assembly language on Intel CPUs. */
inline float Cos(float angle)
{
#ifdef __USE_ASM_CODE_
	float s, c;
	SinCos(angle, &s, &c);
	return c;
#else
	return (float)cos((double)angle);
#endif
}

/*! \remarks Returns the square root of the specified argument as a float. This function uses the assembly language on Intel CPUs. */ 
inline float Sqrt(float arg)
{
#ifdef __USE_ASM_CODE_
	float ans;
    __asm {
        fld         dword ptr arg
        fsqrt
        fstp        dword ptr [ans]
    	}
	return ans;
#else
	return (float)sqrt((double)arg);
#endif
}


