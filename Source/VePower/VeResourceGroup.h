////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeResourceGroup.h
//  Created:     2015/08/23 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_POWER_API VeResourceGroup : public VeRefObject
{
	VeNoCopy(VeResourceGroup);
public:
	VeResourceGroup(const VeChar8* pcName) noexcept;

	virtual ~VeResourceGroup() noexcept;

	inline const VeChar8* GetName() const noexcept;

	inline VeUInt32 GetReadDirNum() const noexcept;

	inline const VeList<VeDirectoryPtr>& GetReadDirList() const noexcept;

	inline const VeDirectoryPtr& GetWriteDir() const noexcept;

	inline bool CanWrite() noexcept;

	void PushReadDirBack(const VeDirectoryPtr& spDir) noexcept;

	void PushReadDirFront(const VeDirectoryPtr& spDir) noexcept;

	void PopReadDirBack() noexcept;

	void PopReadDirFront() noexcept;

	void SetWriteDirectory(const VeDirectoryPtr& spDir) noexcept;

	void Clear() noexcept;

	VeDirectoryPtr GetDir(const VeChar8* pcFileName) noexcept;
	
protected:
	const VeFixedString m_kName;
	VeList<VeDirectoryPtr> m_kReadDirList;
	VeDirectoryPtr m_spWriteDir;

};

VeSmartPointer(VeResourceGroup);

#include "VeResourceGroup.inl"
