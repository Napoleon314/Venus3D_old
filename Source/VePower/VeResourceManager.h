////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeResourceManager.h
//  Created:     2015/03/10 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

class VE_POWER_API VeResourceManager : public VeRefObject
{
	VeNoCopy(VeResourceManager);
public:
	enum Task
	{
		TASK_,
		TASK_FILE,
		TASK_PROCESS,
		TASK_NUM
	};

	typedef std::function<VeDirectoryPtr(const VeChar8*, bool)>
		DirCreator;

	typedef std::function<VeBinaryIStreamPtr(const VeChar8*)>
		StreamCreator;

	class FileCache;
	typedef VePointer<FileCache> FileCachePtr;

	typedef std::function<void(FileCachePtr)> FileCallback;


	class VE_POWER_API FileCache : public VeRefObject
	{
	public:
		FileCache(VeResourceManager* pkParent, const VeChar8* pcPath,
			const VeChar8* pcName, VeResourceGroupPtr& spGroup) noexcept;

		virtual ~FileCache() noexcept;

		void Enter() noexcept;

		void Leave() noexcept;

	protected:
		friend class VeResourceManager;
		VeRefNode<FileCache*> m_kNode;
		VeResourceManager* m_pkParent;
		VeFixedString m_kFullPath;
		VeFixedString m_kFileName;
		VeResourceGroupPtr m_spGroup;
		VeDirectoryPtr m_spDir;
		VeMemoryIStreamPtr m_spData;
		VeList<FileCallback> m_kCallbackList;
		VeUInt32 m_u32FreeTime = 0;
		VeUInt32 m_u32EnterCount = 0;

	};

	VeResourceManager() noexcept;

	virtual ~VeResourceManager() noexcept;

	inline VeTaskQueue& GetTaskQueue(Task eTask) noexcept;

	inline void SetFileFreeTime(VeUInt32 u32TimeUInt) noexcept;

	inline VeUInt32 GetFileFreeTime() noexcept;

	bool HasTask() noexcept;

	void Update() noexcept;

	void SetDefaultDirCreator(DirCreator kCreator) noexcept;

	void RegistDirCreator(const VeChar8* pcName,
		DirCreator kCreator) noexcept;

	void UnregistDirCreator(const VeChar8* pcName) noexcept;

	void SetDefaultStreamCreator(StreamCreator kCreator) noexcept;

	void RegistStreamCreator(const VeChar8* pcName,
		StreamCreator kCreator) noexcept;

	void UnregistStreamCreator(const VeChar8* pcName) noexcept;

	template <class _Ty>
	void RegistDirectory(bool bDefault = false) noexcept
	{
		RegistDirCreator(_Ty::GetTypeName(), &_Ty::Create);
		RegistStreamCreator(_Ty::GetTypeName(), &_Ty::CreateStream);
		if (bDefault)
		{
			SetDefaultDirCreator(&_Ty::Create);
			SetDefaultStreamCreator(&_Ty::CreateStream);
		}
	}

	template <class _Ty>
	void UnregistDirectory() noexcept
	{
		UnregistDirCreator(_Ty::GetTypeName());
		UnregistStreamCreator(_Ty::GetTypeName());
	}

	VeDirectoryPtr CreateDir(const VeChar8* pcPath,
		bool bTryCreate = true) noexcept;

	VeBinaryIStreamPtr CreateStream(const VeChar8* pcPath) noexcept;

	void SetupGroupFromJSON(const VeChar8* pcText) noexcept;

	void SetupGroup(const VeJSONValue& kObj) noexcept;

	VeResourceGroupPtr GetGroup(const VeChar8* pcName, bool bCreate = true) noexcept;

	void SetDefaultGroup(const VeResourceGroupPtr& spGroup) noexcept;

	void CacheFile(const VeChar8* pcPath, FileCallback kCallback) noexcept;
protected:
	VeTaskQueue m_akTaskQueues[TASK_NUM];
	DirCreator m_kDefaultDirCreator;
	VeStringMap<DirCreator> m_kDirCreatorMap;
	StreamCreator m_kDefaultStreamCreator;
	VeStringMap<StreamCreator> m_kStreamCreatorMap;
	VeStringMap<VeResourceGroupPtr> m_kGroupMap;
	VeResourceGroupPtr m_spDefaultGroup;
	VeRefList<FileCache*> m_kLoadingFileList;
	VeRefList<FileCache*> m_kUsingFileList;
	VeRefList<FileCache*> m_kFreeFileList;
	VeStringMap<FileCachePtr> m_kFileCacheMap;
	VeUInt32 m_u32FileCacheFreeTime = 1000;

};

VeSmartPointer(VeResourceManager);

#include "VeResourceManager.inl"
