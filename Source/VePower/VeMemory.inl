////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeMemory.inl
//  Created:     2015/02/19 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
#ifndef VE_MEM_DEBUG
//--------------------------------------------------------------------------
inline void* _VeMalloc(VeSizeT stSizeInBytes) noexcept
{
	return VeExternalMalloc(stSizeInBytes);
}
//--------------------------------------------------------------------------
inline void* _VeAlignedMalloc(VeSizeT stSizeInBytes,
	VeSizeT stAlignment) noexcept
{
	return VeExternalAlignedMalloc(stSizeInBytes, stAlignment);
}
//--------------------------------------------------------------------------
inline void* _VeRealloc(void* pvMemblock, VeSizeT stSizeInBytes) noexcept
{
	return VeExternalRealloc(pvMemblock, stSizeInBytes);
}
//--------------------------------------------------------------------------
inline void _VeFree(void* pvMemory) noexcept
{
	return VeExternalFree(pvMemory);
}
//--------------------------------------------------------------------------
inline void _VeAlignedFree(void* pvMemory) noexcept
{
	return VeExternalAlignedFree(pvMemory);
}
//--------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------
