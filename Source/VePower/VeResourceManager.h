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

class VE_POWER_API VeResourceManager : public VeSingleton<VeResourceManager>
{
	VeNoCopy(VeResourceManager);
public:
	enum Task
	{
		TASK_QUERY,
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

	class JSONCache;
	typedef VePointer<JSONCache> JSONCachePtr;

	typedef std::function<void(FileCachePtr)> FileCallback;
	typedef FileCallback FileCreator;

	typedef std::function<void(JSONCachePtr,VeJSONValue&)> JSONCreator;

	typedef std::function<VeResourcePtr(const VeChar8*,const VeChar8*)> ResCreator;

	class VE_POWER_API FileCache : public VeRefObject
	{
	public:
		FileCache(const VeChar8* pcPath, const VeChar8* pcName,
			const VeResourceGroupPtr& spGroup) noexcept;

		virtual ~FileCache() noexcept;

		inline const VeChar8* GetFullPath() const noexcept;

		inline const VeChar8* GetFileName() const noexcept;

		inline const VeChar8* GetGroupName() const noexcept;

		inline const VeResourceGroupPtr& GetGroup() const noexcept;

		inline const VeMemoryIStreamPtr& GetData() noexcept;

		void Enter() noexcept;

		void Leave() noexcept;

	protected:
		friend class VeResourceManager;
		VeRefNode<FileCache*> m_kNode;
		VeFixedString m_kFullPath;
		VeFixedString m_kFileName;
		VeResourceGroupPtr m_spGroup;
		VeDirectoryPtr m_spDir;
		VeMemoryIStreamPtr m_spData;
		VeList<FileCallback> m_kCallbackList;
		VeUInt32 m_u32FreeTime = 0;
		VeUInt32 m_u32EnterCount = 0;

	};

	class VE_POWER_API JSONCache : public VeRefObject
	{
	public:
		JSONCache(const FileCachePtr& spFile) noexcept;

		virtual ~JSONCache() noexcept;

		inline const VeChar8* GetFullPath() noexcept;

		inline const VeChar8* GetGroupName() noexcept;

		void Parse() noexcept;

		void Create() noexcept;

	protected:
		VeRefNode<VeRefObject*> m_kNode;
		FileCachePtr m_spFile;
		VeFixedString m_kFullPath;
		VeFixedString m_kGroup;		
		VeJSONDoc m_kDoc;

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

	void RegistFileCreator(const VeChar8* pcExt, FileCreator kCreator) noexcept;

	void RegistFileCreator(std::initializer_list<const VeChar8*> kExtList, FileCreator kCreator) noexcept;

	void UnregistFileCreator(const VeChar8* pcExt) noexcept;

	void UnregistFileCreator(std::initializer_list<const VeChar8*> kExtList) noexcept;

	void RegistJSONCreator(const VeChar8* pcType, JSONCreator kCreator) noexcept;

	void UnregistJSONCreator(const VeChar8* pcType) noexcept;

	void RegistResCreator(const VeChar8* pcType, ResCreator kCreator) noexcept;

	void UnregistResCreator(const VeChar8* pcType) noexcept;

	void SetupGroupFromJSON(const VeChar8* pcText) noexcept;

	void SetupGroup(const VeJSONValue& kObj) noexcept;

	VeResourceGroupPtr GetGroup(const VeChar8* pcName, bool bCreate = true) noexcept;

	void SetDefaultGroup(const VeResourceGroupPtr& spGroup) noexcept;

	VeResourcePtr GetResource(const VeChar8* pcFullName, bool bCreate = true) noexcept;

	VeResourcePtr GetResource(const VeChar8* pcType, const VeChar8* pcName, bool bCreate = true) noexcept;

	template <class _Ty>
	VePointer<_Ty> Get(const VeChar8* pcFullName, bool bCreate = true) noexcept
	{
		return VeDynamicCast(_Ty, (VeResource*)GetResource(pcFullName, bCreate));
	}

	template <class _Ty>
	VePointer<_Ty> Get(const VeChar8* pcType, const VeChar8* pcName, bool bCreate = true) noexcept
	{
		return VeDynamicCast(_Ty, (VeResource*)GetResource(pcType, pcName, bCreate));
	}

	void LoadFile(const VeChar8* pcPath) noexcept;

	void LoadGroup(const VeChar8* pcGroup) noexcept;

	void ParseJSON(FileCachePtr spCache) noexcept;

	void CacheFile(const VeChar8* pcPath, FileCallback kCallback) noexcept;

	void CacheFile(const VeChar8* pcFile, const VeResourceGroupPtr& spGroup,
		FileCallback kCallback) noexcept;

protected:
	VeTaskQueue m_akTaskQueues[TASK_NUM];
	DirCreator m_kDefaultDirCreator;
	VeStringMap<DirCreator> m_kDirCreatorMap;
	StreamCreator m_kDefaultStreamCreator;
	VeStringMap<StreamCreator> m_kStreamCreatorMap;
	VeStringMap<FileCreator> m_kFileCreatorMap;	
	VeStringMap<JSONCreator> m_kJSONCreatorMap;
	VeStringMap<ResCreator> m_kResCreatorMap;
	VeStringMap<VeResourceGroupPtr> m_kGroupMap;
	VeResourceGroupPtr m_spDefaultGroup;
	VeRefList<VeResource*> m_kLoadingResList;
	VeRefList<VeResource*> m_kUsingResList;
	VeRefList<VeResource*> m_kFreeResList;
	VeStringMap<VeResourcePtr> m_kResourceMap;
	VeRefList<FileCache*> m_kLoadingFileList;
	VeRefList<FileCache*> m_kUsingFileList;
	VeRefList<FileCache*> m_kFreeFileList;
	VeStringMap<FileCachePtr> m_kFileCacheMap;
	VeUInt32 m_u32FileCacheFreeTime = 1000;

};

#define ve_res_mgr VeResourceManager::GetSingleton()

#include "VeResourceManager.inl"
