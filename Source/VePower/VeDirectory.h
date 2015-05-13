////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeDirectory.h
//  Created:     2015/03/10 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

VE_POWER_API bool VeWildNameMatch(const VeChar8* pcName,
	const VeChar8* pcPattern) noexcept;

VeSmartPointer(VeDirectory);

class VE_POWER_API VeDirectory : public VeRefObject
{
	VeRTTIDecl(VeDirectory);
public:
	enum AccessMask
	{
		ACCESS_F_OK = 0x0,
		ACCESS_W_OK = 0x2,
		ACCESS_R_OK = 0x4
	};

	typedef std::function<void(const VeMemoryIStreamPtr&)> ReadCallback;
	typedef std::function<void(VeResult)> WriteCallback;

	VeDirectory() noexcept;

	virtual ~VeDirectory() noexcept;

	virtual const VeChar8* GetType() noexcept = 0;

	virtual const VeChar8* GetName() noexcept = 0;

	virtual bool Access(VeUInt32 u32Flag) const noexcept = 0;

	virtual bool HasFile(const VeChar8* pcFile) const noexcept = 0;

	virtual bool HasSubDir(const VeChar8* pcDir) const noexcept = 0;

	virtual void FindFileList(const VeChar8* pcDesc,
		VeVector<VeFixedString>& kOut) const noexcept = 0;

	virtual VeDirectoryPtr Open(const VeChar8* pcPath,
		bool bTryCreate = true) const noexcept = 0;

	virtual VeBinaryIStreamPtr OpenSync(
		const VeChar8* pcFile) const noexcept = 0;

	virtual void ReadAsync(const VeChar8* pcFile,
		VeRefNode<ReadCallback>& kCallback) const noexcept = 0;

	virtual void WriteAsync(const VeChar8* pcFileName,
		const VeMemoryOStreamPtr& spContent,
		VeRefNode<WriteCallback>& kDelegate,
		bool bAppend = false) const noexcept = 0;

};
