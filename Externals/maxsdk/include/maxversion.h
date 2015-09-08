#pragma once

#include "buildnumber.h" // defines VERSION_INT

// VERSION_INT is defined in buildnumber.h and is written to by the builder, inserting the current build number.
// VERSION_INT is used for the 3rd component eg. 4.1.1.[36]

// The product and file version could be different.
// For example, VIZ 4.0 works with max files version 4.2

// 3ds Max internal version number is managed using the following scheme:
// Major = Main release number (ex. 3ds Max 2013 (SimCity) = 15).
// Minor = Product Update; PUs would typically be starting at 1
// Point = Build Number (this always increments for a major release
//
// Example for Phoenix = 18:
//			First Release Service Pack would be:	18.1.<build>.0
//			Second Release Service Pack would be:	18.2.<build>.0
//			Etc.
//
// Hot fixes have the minor number of the next unreleased SP.

// MAX File version:
#ifndef MAX_VERSION_MAJOR
#define MAX_VERSION_MAJOR 18
#endif

#ifndef MAX_VERSION_MINOR
#define MAX_VERSION_MINOR 0
#endif

#ifndef MAX_VERSION_POINT
#define MAX_VERSION_POINT VERSION_INT
#endif

// MAX Product version
#ifndef MAX_PRODUCT_VERSION_MAJOR
#define MAX_PRODUCT_VERSION_MAJOR 18
#endif

#ifndef MAX_PRODUCT_VERSION_MINOR
#define MAX_PRODUCT_VERSION_MINOR 0
#endif

#ifndef MAX_PRODUCT_VERSION_POINT
#define MAX_PRODUCT_VERSION_POINT VERSION_INT
#endif

#ifndef MAX_PRODUCT_YEAR_NUMBER
#define MAX_PRODUCT_YEAR_NUMBER 2016
#endif

// MAX_RELEASE_EXTERNAL is an alternative for MAX_RELEASE (plugapi.h)
// Define it when you want the "branded" (UI) version number to be different 
// from the internal one.
//#define MAX_RELEASE_EXTERNAL	10900

// This should be left blank for the main release up until the first Service Pack.
// Service Pack 1 would then need to be labeled "SP1" etc.
// Hot fixes are labeled HF
#ifndef MAX_PRODUCT_VERSION_SP
#define MAX_PRODUCT_VERSION_SP "\0"
#endif

// HF number is only informative, that's the last number in the version info. It is always 0 for public releases.
#ifndef MAX_HF_NUMBER
#define MAX_HF_NUMBER 0
#endif

#define _MAX_VERSION(a, b, c,d) a##b##c##d
#define MAX_VERSION _MAX_VERSION(MAX_VERSION_MAJOR, MAX_VERSION_MINOR, VERSION_INT, MAX_HF_NUMBER)
