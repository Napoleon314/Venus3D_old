////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeAsset.h
//  Created:     2015/03/12 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef VE_PLATFORM_ANDROID

VE_POWER_API void SetAssetManager(AAssetManager* pkManager);

class VE_POWER_API VeAssetIStream : public VeBinaryIStream
{
	VeRTTIDecl(VeAssetIStream, VeBinaryIStream);
public:
	VeAssetIStream(const VeChar8* pcAssetName) noexcept;

	virtual ~VeAssetIStream() noexcept;

	virtual VeSizeT Read(void* pvBuffer, VeSizeT stBytes) noexcept;

	virtual VeResult Seek(VePtrDiff pdOffset, VeWhence eWhence) noexcept;

	virtual VeSizeT Tell() noexcept;

	virtual void* Skip(VeSizeT stBytes) noexcept;

	virtual void Restart() noexcept;

	virtual VeSizeT RemainingLength() const noexcept;

	virtual VeChar8 Peek() noexcept;

	virtual void Finish() noexcept;

private:
	AAsset* m_pkAsset = nullptr;

};

class VE_POWER_API VeAssetPath : public VeDirectory
{
	VeRTTIDecl(VeAssetPath, VeDirectory);
public:
	class VE_POWER_API ReadTask : public VeRefObject
	{
	public:
		ReadTask(const VeChar8* pcFullPath,
			VeRefNode<ReadCallback>& kCallback) noexcept;

	protected:
		VeFixedString m_kFullPath;
		VeRefNode<VeRefObject*> m_kNode;
		VeRefList<ReadCallback> m_kCallback;
		VeRefNode<VeTaskQueue::DoTask> m_kTask;
		VeMemoryIStreamPtr m_spData;

	};

	VeAssetPath(const VeChar8* pcPath = "") noexcept;

	virtual ~VeAssetPath() noexcept;

	static const VeChar8* GetTypeName() noexcept;

	virtual const VeChar8* GetType() noexcept;

	virtual const VeChar8* GetName() noexcept;

	virtual bool Access(VeUInt32 u32Flag) const noexcept;

	virtual bool HasFile(const VeChar8* pcFile) const noexcept;

	virtual bool HasSubDir(const VeChar8* pcDir) const noexcept;

	virtual void FindFileList(const VeChar8* pcDesc,
		VeVector<VeFixedString>& kOut) const noexcept;

	virtual VeDirectoryPtr Open(const VeChar8* pcPath,
		bool bTryCreate = true) const noexcept;

	virtual VeBinaryIStreamPtr OpenIStream(
		const VeChar8* pcFile) const noexcept;

	virtual void ReadAsync(const VeChar8* pcFile,
		VeRefNode<ReadCallback>& kCallback) const noexcept;

	virtual void WriteAsync(const VeChar8* pcFileName,
		const VeMemoryOStreamPtr& spContent,
		VeRefNode<WriteCallback>& kDelegate,
		bool bAppend = false) const noexcept;

	static VeDirectoryPtr Create(const VeChar8* pcPath,
		bool bTryCreate = true) noexcept;

	static VeBinaryIStreamPtr CreateStream(const VeChar8* pcPath) noexcept;

private:
	VeFixedString m_kPath;
	static const VeChar8 * const ms_pcType;

};

#endif
