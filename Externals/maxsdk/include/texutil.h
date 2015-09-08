/**********************************************************************
 *<
	FILE: texutil.h

	DESCRIPTION:

	CREATED BY: Dan Silva

	HISTORY:

 *>	Copyright (c) 1994, All Rights Reserved.
 **********************************************************************/
#pragma once

#include "coreexp.h"
#include "point2.h"
#include "point3.h"
#include "acolor.h"

//
//	Misc. noise functions from Texturing and Modeling A Procedural Approach
//  Perlin, Musgrave...
//

/*! \remarks This function performs a mapping across the unit interval [0, 1] where the result is within the unit interval. If b is within the interval 0 to 1, the result of bias() is within the interval 0 to 1. This function is defined such that bias(a, 0.5)=a. The function is defined as follows: \n\n
\code
float bias(float a, float b) { 
return (float)pow(a, log(b) / log(0.5f)); 
} \endcode 
 */
CoreExport float bias(float a, float b);
/*! \remarks This function performs a mapping across the unit interval [0, 1] where the result is within the unit interval. If b is within the interval 0 to 1, the result of gain() is within the interval 0 to 1. This function is defined such that gain(a, 0.5)=a. Above and below 0.5, this function consists of two scaled down bias() curves forming an S-shaped curve. The function is defined as follows: \n\n
\code
float gain(float a, float b)
{ 
float p = (float)log(1.0f - b) / (float)log(0.5f); 
if (a < .001f) 
return 0.f; 
else if (a > .999f) 
return 1.0f; 
if (a < 0.5f) 
return (float)pow(2 * a, p) / 2; 
else 
return 1.0f - (float)pow(2.0 * (1. - a), (double)p) / 2; 
} \endcode 
 */
CoreExport float gain(float a, float b);
/*! \remarks The clamp() function is defined as follows: \n\n
\code
float clamp( float x, float a, float b) { 
return (x < a ? a : (x > b ? b : x)); 
} \endcode 
\param x A floating point value.
\param a A floating point value.
\param b A floating point value.
\return Returns "a" when "x" is less than "a", the value of "x" when "x" is between "a" and "b", and the value "b" when "x" is greater than "b".
 */
CoreExport float clamp(float x, float a, float b);
/*! \remarks This function is a smoother version of step() using a linear ramp. The boxstep() function is defined as follows:\n\n
\code
float boxstep(float a, float b, float x) { 
return clamp((x-a)/(b-a),0.0f, 1.0f); 
} \endcode \n\n
For comparison, note that step() returns values of 0 when x is less than a and 1 when x is greater than or equal to a. This function is not a part of the SDK, but step() is defined as follows:\n\n
\code
float step(float a, float x) { 
return (float)(x >= a); 
} \endcode \n\n
\param a The limit for the x value where the function will return 0.
\param b The limit for the x value where the function will return 1.
\param x A floating point value.
\return 0 if "x" is less than "a", a linear interpolation between 0 and 1 if "x" is greater than or equal to "a" and less than or equal to "b", and 1 if "x" is greater than "b".
 */
CoreExport float boxstep(float a, float b, float x); // linear from (a,0) to (b,1)
/*! \remarks This function is similar to step(), but instead of a sharp transition from 0 to 1 at a specified threshold, it makes a gradual transition from 0 to 1 beginning at threshold a and ending at threshold b. To do this, this function uses a cubic function whose slope is 0 at "a" and "b", and whose value is 0 at "a" and 1 at "b". This function provides a still smoother version of step() using a cubic spline. The smoothstep() function is used (instead of step()) in many procedural textures because sharp transitions often result in artifacts.
\param a The limit for the x value where the function will return 0.
\param b The limit for the x value where the function will return 1.
\param x A floating point value.
 */
CoreExport float smoothstep(float a, float b, float x);  // Hermite cubic from (a,0) to (b,1)
/*! \remarks This function returns the mathematical modulo operation and handles negatives correctly. The standard math functions fmod() and fmodf() as well as the operator % return negative results if the first operand is negative. This function always returns a positive remainder. */
CoreExport float mod(float x, float m); // returns x Mod m, handles negatives correctly
/*! \remarks This function returns the mathematical modulo operation and handles negatives correctly. The standard math functions fmod() and fmodf() as well as the operator % return negative results if the first operand is negative. This function always returns a positive remainder. */
CoreExport int mod(int x, int m); // returns x Mod m, handles negatives correctly
/*! \remarks This function makes a type of straight-segment S curve. The parameters are x, a, b, and d. The algorithm is as follows:\n\n
for a+d < x < b-d sramp(x) = x \n\n
for a-d < x < a+d sramp() makes a smooth transition (parabolic) from sramp' = 0 to sramp' = 1 \n\n
for b-d < x < b+d sramp() makes a smooth transition (parabolic) from sramp' = 1 to sramp' = 0 \n\n
 */
CoreExport float sramp(float x,float a, float b, float d);

// returns 0 if x<a, 1 if x>b otherwise x.
/*! \remarks Returns 0 if "x" is less than "a", 1 if "x" is greater than "b", otherwise it returns "x". */
CoreExport float threshold(float x,float a, float b);

CoreExport void  setdebug(int i);
/*! \remarks An approximation of white noise blurred to dampen frequencies beyond some value. */
CoreExport float noise1(float arg);
/*! \remarks An approximation of white noise blurred to dampen frequencies beyond some value in two dimensions. */
CoreExport float noise2(Point2 p);
/*! \remarks A noise function in three dimensions implemented by a pseudo-random tricubic spline. This function is an approximation of white noise blurred to dampen frequencies beyond some value. */
CoreExport float noise3(Point3 p);
/*! \remarks This function is an approximation of white noise blurred to dampen frequencies beyond some value in three dimensions, and time. */
CoreExport float noise4(Point3 p,float time);

// This is 3DStudio's Noise function: its only slightly different from noise3:
//  scaled up by factor of 1.65 and clamped to -1,+1.
/*! \remarks A noise function in three dimensions implemented by a pseudo-random tricubic spline. This is the same as noise3() scaled up by factor of 1.65 and clamped to -1,+1. The macro NOISE can be used to map the value returned from the noise3DS() function into interval [0,1]. */
CoreExport float noise3DS(Point3 p);
/*! \remarks This turbulence function is a simple fractal generating loop built on top of the noise function. It is used to make marble, clouds, explosions, and so on. It returns a value in the range [0, 1]. */
CoreExport float turbulence(Point3& p, float freq);
/*! \remarks Takes the low 9 bits of an input value and returns a number in that range (0-512). */
CoreExport int Perm(int v);

#define MAX_OCTAVES	50
/*! \remarks A fractional Brownian motion fractal (or fBm for short) that returns a floating point value. This version of the fBm is said to be "homogeneous" (the same everywhere) and "isotropic" (the same in all directions). There are versions of this function that accept an input floating point, Point2, and Point3 value. */
CoreExport float fBm1(float  point, float H, float lacunarity, float octaves);
CoreExport float fBm1(Point2 point, float H, float lacunarity, float octaves);
CoreExport float fBm1(Point3 point, float H, float lacunarity, float octaves);
/*! \remarks A one-dimensional Catmull-Rom interpolating spline through a set of knot values. The spline() function used to map a number into another number. The parameter of the spline is a floating point value. If x is 0, the result is the second knot value. If x is 1, the result is the final knot value. For values between 0 and 1, the value interpolates smoothly between the values of the knots from the second knot to the second to last knot. The first and last knot values determine the derivatives of the spline at the endpoint. */
CoreExport float spline(float x, int nknots, float *knot);
/*! \remarks A variant of the spline() function for mapping colors. */
CoreExport Color color_spline(float x, int nknots, Color *knot);


// faster version of floor
/*! \remarks A fast version of the standard C function floor(). It returns a floating-point value representing the largest integer that is less than or equal to x. */
inline int FLOOR( float x) { return ((int)(x) - ((int)(x)>(x)? 1:0)); }
/*! \remarks Returns the fraction (non-integer) part of the value passed. This is defined as x - (float)FLOOR(x). */
inline float frac(float x) { return x - (float)FLOOR(x); }
/*! \remarks Returns the larger of two values (returns the second value in the case of equality). */
inline float fmax(float x, float y) { return x>y?x:y;	}
/*! \remarks Returns x if it is less than y. Otherwise, y. */
inline float fmin(float x, float y) { return x<y?x:y;	}

// Macro to map it into interval [0,1]
#define NOISE(p) ((1.0f+noise3DS(p))*.5f)

// alpha-composite ctop on top of cbot, assuming pre-multiplied alpha
/*! \remarks Performs an alpha-composite of one color (ctop) on top of another (cbot), assuming pre-multiplied alpha. */
inline AColor AComp(AColor cbot, AColor ctop) {
	float ia = 1.0f - ctop.a;
	return (ctop + ia*cbot);
	}


//-----------------------------------------------------------
// Based on: A Cellular Basis Function
//                   Steven Worley
//
// SIGGRAPH 1996 Conference Procedings
//
#define MAX_CELL_LEVELS	20
/*! \remarks This is the noise function used by the 3ds Max Cellular texture. The idea is that there is a set of cells randomly distributed through space. This function returns the distances to the closest cells. Developers using this function can refer to: A Cellular Basis Function by Steven Worley, and published in the SIGGRAPH 1996 Conference Procedings.
 */
CoreExport void CellFunction(Point3 v,int n,float *dist,int *celIDs=NULL,Point3 *grads=NULL,float gradSmooth=0.0f);
/*! \remarks A fractal version of CellFunction(). It has additional parameters for iterations and lacunariy. */
CoreExport void FractalCellFunction(Point3 v,float iterations, float lacunarity,int n,float *dist,int *celIDs=NULL,Point3 *grads=NULL,float gradSmooth=0.0f);
/*! \remarks Returns a random number in the range 0.0 to 1.0 based on a cell identifier. */
CoreExport float RandFromCellID(int id);

