////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeResourceGroup.inl
//  Created:     2015/08/23 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline const VeChar8* VeResourceGroup::GetName() const noexcept
{
	return m_kName;
}
//--------------------------------------------------------------------------
inline VeUInt32 VeResourceGroup::GetReadDirNum() const noexcept
{
	return (VeUInt32)m_kReadDirList.size();
}
//--------------------------------------------------------------------------
inline const VeList<VeDirectoryPtr>& VeResourceGroup::GetReadDirList() const noexcept
{
	return m_kReadDirList;
}
//--------------------------------------------------------------------------
inline const VeDirectoryPtr& VeResourceGroup::GetWriteDir() const noexcept
{
	return m_spWriteDir;
}
//--------------------------------------------------------------------------
inline bool VeResourceGroup::CanWrite() noexcept
{
	return m_spWriteDir ? true : false;
}
//--------------------------------------------------------------------------
