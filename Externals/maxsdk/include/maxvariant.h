//**************************************************************************/
// Copyright (c) 1998-2011 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Max VARIANT handling
// AUTHOR:      Richard Houle
// DATE:        Fev 3 2011
//***************************************************************************/
#pragma once

#include "maxstring.h"
#include <PropIdl.h>
#include <vector>

class CStr;
class WStr;

namespace MaxSDK { namespace Util {

/**
 * Convert a variant into a string.
 *
 * For flags, see VariantConvertType inside MSDN for a list of possible values.
 */
UtilExport MaxString VariantToString(const PROPVARIANT*, UINT encoding = CP_ACP, USHORT flags = 0);

/**
 * Determine if a variant is already a string.
 */
UtilExport bool VariantIsString(const PROPVARIANT*);

/**
 * Determine if a variant is a vector of string.
 */
UtilExport bool VariantIsStringVector(const PROPVARIANT*);

/**
 * Convert a variant into a std::vector.
 */
template <typename T>
size_t VariantToStringVector(const PROPVARIANT*, std::vector<T>&, UINT encoding = CP_ACP);

template UtilExport size_t VariantToStringVector<MaxString>(const PROPVARIANT*, std::vector<MaxString>&, UINT encoding);
template UtilExport size_t VariantToStringVector<CStr>(const PROPVARIANT*, std::vector<CStr>&, UINT encoding);
template UtilExport size_t VariantToStringVector<WStr>(const PROPVARIANT*, std::vector<WStr>&, UINT encoding);

/**
 * Sets a string inside a variant.
 *
 * @param str    String. Copied inside this object.
 * @param clear  If true, will perform a VariantClear on the variant.
 */
UtilExport bool SetStringToVariant(PROPVARIANT*, const char* str, bool clear = true);

/**
 * Sets a string inside a variant.
 *
 * @param str    String. Copied inside this object.
 * @param clear  If true, will perform a VariantClear on the variant.
 */
UtilExport bool SetStringToVariant(PROPVARIANT*, const wchar_t* str, bool clear = true);

/**
 * Add a string entry at the end of a variant vector.
 *
 * Supported variant types are:
 *  * VT_VECTOR | VT_LPSTR
 *  * VT_VECTOR | VT_LPWSTR
 *  * VT_VECTOR | VT_VARIANT
 */
UtilExport bool VariantAddToVector(PROPVARIANT* v, const char*, UINT encoding = CP_ACP);

/**
 * Add a string entry at the end of a variant vector.
 *
 * Supported variant types are:
 *  * VT_VECTOR | VT_LPSTR
 *  * VT_VECTOR | VT_LPWSTR
 *  * VT_VECTOR | VT_VARIANT
 */
UtilExport bool VariantAddToVector(PROPVARIANT* v, const wchar_t*, UINT encoding = CP_ACP);

/**
 * Add an entry at the end of a variant vector.
 *
 * Supported variant types are:
 *  * VT_VECTOR | VT_LPSTR
 *  * VT_VECTOR | VT_LPWSTR
 *  * VT_VECTOR | VT_VARIANT
 */
UtilExport bool VariantAddToVector(PROPVARIANT* v, const PROPVARIANT*, bool give = false, UINT encoding = CP_ACP);

/**
 * Determine if a variant contains wide-chars.
 */
UtilExport bool DoesVariantContainWideChars(const PROPVARIANT* variant);

/**
 * Convert a variant to UTF16.
 *
 * No matter what happens, the source variant is left intact. And you have the
 * responsability to free the destination variant.
 * @param destVar The destination variant.
 * @param sourceVar The source variant.
 * @param codePage Any data stored in "sourceVar" is considered in that codepage.
 */
UtilExport bool VariantConvertToUTF16(PROPVARIANT* destVar, const PROPVARIANT* sourceVar, UINT codePage = CP_ACP);

/**
 * Convert a variant to MBCS.
 *
 * No matter what happens, the source variant is left intact. And you have the
 * responsability to free the destination variant.
 * @param destVar The destination variant.
 * @param sourceVar The source variant.
 * @param codePage Any data stored in "sourceVar" is considered to be in ACP. Codepage serves as a destination
 *                 codepage. All ACP data will be converted to that codepage.
 */
UtilExport bool VariantConvertToMBCS(PROPVARIANT* destVar, const PROPVARIANT* sourceVar, UINT codePage = CP_ACP);

inline MaxString VariantToString(const VARIANT*     v, UINT encoding = CP_ACP, USHORT flags = 0) { return VariantToString((PROPVARIANT*)v,  encoding, flags); }
inline MaxString VariantToString(const PROPVARIANT& v, UINT encoding = CP_ACP, USHORT flags = 0) { return VariantToString(&v,               encoding, flags); }
inline MaxString VariantToString(const VARIANT&     v, UINT encoding = CP_ACP, USHORT flags = 0) { return VariantToString((PROPVARIANT*)&v, encoding, flags); }

inline bool VariantIsStringVector(const VARIANT*     v) { return VariantIsStringVector((PROPVARIANT*)v); }
inline bool VariantIsStringVector(const PROPVARIANT& v) { return VariantIsStringVector(&v); }
inline bool VariantIsStringVector(const VARIANT&     v) { return VariantIsStringVector((PROPVARIANT*)&v); }

inline size_t VariantToStringVector(const VARIANT*     v, std::vector<MaxString>& ov, UINT encoding = CP_ACP) { return VariantToStringVector((PROPVARIANT*)v,  ov, encoding); }
inline size_t VariantToStringVector(const PROPVARIANT& v, std::vector<MaxString>& ov, UINT encoding = CP_ACP) { return VariantToStringVector(&v,               ov, encoding); }
inline size_t VariantToStringVector(const VARIANT&     v, std::vector<MaxString>& ov, UINT encoding = CP_ACP) { return VariantToStringVector((PROPVARIANT*)&v, ov, encoding); }

}}


