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

VeSmartPointer(VeResource);

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
		IStreamCreator;

	typedef std::function<VeBinaryOStreamPtr(const VeChar8*)>
		OStreamCreator;

	class FileCache;
	typedef VePointer<FileCache> FileCachePtr;

	class JSONCache;
	typedef VePointer<JSONCache> JSONCachePtr;

	typedef std::function<void(FileCachePtr)> FileCallback;

	typedef std::function<void(void)> LoadCallback;

	typedef std::function<void(FileCachePtr,LoadCallback)> FileCreator;

	typedef std::function<void(FileCachePtr)> FileCreatorSync;

	typedef std::function<void(JSONCachePtr,VeJSONValue&)> JSONCreator;

	typedef std::function<void(const FileCachePtr&,VeJSONValue&)> JSONCreatorSync;

	typedef std::function<VeResourcePtr(const VeChar8*)> ResCreator;

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

		inline const VeBlobPtr& GetData() noexcept;

		void Load() noexcept;

		void LoadSync() noexcept;

		void Enter() noexcept;

		void Leave() noexcept;		

	protected:
		friend class VeResourceManager;
		VeRefNode<FileCache*> m_kNode;
		VeFixedString m_kFullPath;
		VeFixedString m_kFileName;
		VeResourceGroupPtr m_spGroup;
		VeDirectoryPtr m_spDir;
		VeBlobPtr m_spData;
		VeList<FileCallback> m_kCallbackList;
		VeUInt32 m_u32FreeTime = 0;
		VeUInt32 m_u32EnterCount = 0;

	};

	class VE_POWER_API JSONCache : public VeRefObject
	{
	public:
		JSONCache(const FileCachePtr& spFile, LoadCallback kCallback) noexcept;

		virtual ~JSONCache() noexcept;

		inline const VeChar8* GetFullPath() noexcept;

		inline const VeChar8* GetGroupName() noexcept;

		inline const VeResourceGroupPtr& GetGroup() noexcept;

		void Parse() noexcept;

		void Create() noexcept;

	protected:
		VeRefNode<VeRefObject*> m_kNode;
		FileCachePtr m_spFile;
		VeFixedString m_kFullPath;
		VeResourceGroupPtr m_spGroup;
		LoadCallback m_kCallback;
		VeJSONDoc m_kDoc;

	};

	VeResourceManager() noexcept;

	virtual ~VeResourceManager() noexcept;

	inline VeTaskQueue& GetTaskQueue(Task eTask) noexcept;

	inline void SetFileFreeTime(VeUInt32 u32TimeUInt) noexcept;

	inline VeUInt32 GetFileFreeTime() noexcept;

	inline const VeResourceGroupPtr& GetDefaultGroup() noexcept;

	bool HasTask() noexcept;

	void Update() noexcept;

	void SetDefaultDirCreator(DirCreator kCreator) noexcept;

	void RegistDirCreator(const VeChar8* pcName,
		DirCreator kCreator) noexcept;

	void UnregistDirCreator(const VeChar8* pcName) noexcept;

	void SetDefaultIStreamCreator(IStreamCreator kCreator) noexcept;

	void RegistIStreamCreator(const VeChar8* pcName,
		IStreamCreator kCreator) noexcept;	

	void UnregistIStreamCreator(const VeChar8* pcName) noexcept;

	void SetDefaultOStreamCreator(OStreamCreator kCreator) noexcept;

	void RegistOStreamCreator(const VeChar8* pcName,
		OStreamCreator kCreator) noexcept;

	void UnregistOStreamCreator(const VeChar8* pcName) noexcept;

	template <class _Ty>
	void RegistDirectory(bool bDefault = false) noexcept
	{
		RegistDirCreator(_Ty::GetTypeName(), &_Ty::Create);
		RegistIStreamCreator(_Ty::GetTypeName(), &_Ty::CreateIStream);
		RegistOStreamCreator(_Ty::GetTypeName(), &_Ty::CreateOStream);
		if (bDefault)
		{
			SetDefaultDirCreator(&_Ty::Create);
			SetDefaultIStreamCreator(&_Ty::CreateIStream);
			SetDefaultOStreamCreator(&_Ty::CreateOStream);
		}
	}

	template <class _Ty>
	void UnregistDirectory() noexcept
	{
		UnregistDirCreator(_Ty::GetTypeName());
		UnregistIStreamCreator(_Ty::GetTypeName());
		UnregistOStreamCreator(_Ty::GetTypeName());
	}

	VeDirectoryPtr CreateDir(const VeChar8* pcPath,
		bool bTryCreate = true) noexcept;

	VeBinaryIStreamPtr CreateIStream(const VeChar8* pcPath) noexcept;

	VeBinaryOStreamPtr CreateOStream(const VeChar8* pcPath) noexcept;

	void RegistFileCreator(const VeChar8* pcExt, FileCreator kCreator, FileCreatorSync kCreatorSync) noexcept;

	void RegistFileCreator(std::initializer_list<const VeChar8*> kExtList, FileCreator kCreator, FileCreatorSync kCreatorSync) noexcept;

	void UnregistFileCreator(const VeChar8* pcExt) noexcept;

	void UnregistFileCreator(std::initializer_list<const VeChar8*> kExtList) noexcept;

	void RegistJSONCreator(const VeChar8* pcType, JSONCreator kCreator, JSONCreatorSync kCreatorSync) noexcept;

	void UnregistJSONCreator(const VeChar8* pcType) noexcept;

	void RegistResCreator(const VeChar8* pcType, ResCreator kCreator) noexcept;

	void UnregistResCreator(const VeChar8* pcType) noexcept;

	void RegistExt(const VeChar8* pcExt, const VeChar8* pcResType) noexcept;

	void UnregistExt(const VeChar8* pcExt) noexcept;

	void SetupGroupFromJSON(const VeChar8* pcText) noexcept;

	void SetupGroup(const VeJSONValue& kObj) noexcept;

	VeResourceGroupPtr GetGroup(const VeChar8* pcName, bool bCreate = true) noexcept;

	void SetDefaultGroup(const VeResourceGroupPtr& spGroup) noexcept;

	VeResourcePtr GetResource(const VeChar8* pcFullName, bool bCreate = true) noexcept;

	template <class _Ty>
	VePointer<_Ty> Get(const VeChar8* pcName, bool bCreate = true) noexcept
	{
		return VeDynamicCast(_Ty, GetResource(pcFullName, bCreate).p());
	}

	void LoadFile(const VeChar8* pcPath, LoadCallback kCallback) noexcept;

	void LoadFile(const VeChar8* pcPath) noexcept;

	void LoadGroup(const VeChar8* pcGroup) noexcept;

	void ParseJSON(FileCachePtr spCache, LoadCallback kCallback) noexcept;

	void ParseJSON(FileCachePtr spCache) noexcept;

	void CacheFile(const VeChar8* pcPath, FileCallback kCallback) noexcept;

	void CacheFile(const VeChar8* pcFile, const VeResourceGroupPtr& spGroup,
		FileCallback kCallback) noexcept;

	FileCachePtr CacheFile(const VeChar8* pcPath) noexcept;

	FileCachePtr CacheFile(const VeChar8* pcFile, const VeResourceGroupPtr& spGroup) noexcept;

protected:
	FileCachePtr GetFileCache(const VeChar8* pcPath) noexcept;

	FileCachePtr GetFileCache(const VeChar8* pcFile, const VeResourceGroupPtr& spGroup) noexcept;

	friend class VeResource;
	VeTaskQueue m_akTaskQueues[TASK_NUM];
	DirCreator m_kDefaultDirCreator;
	VeStringMap<DirCreator> m_kDirCreatorMap;
	IStreamCreator m_kDefaultIStreamCreator;
	VeStringMap<IStreamCreator> m_kIStreamCreatorMap;
	OStreamCreator m_kDefaultOStreamCreator;
	VeStringMap<OStreamCreator> m_kOStreamCreatorMap;
	VeStringMap<std::pair<FileCreator,FileCreatorSync>> m_kFileCreatorMap;	
	VeStringMap<std::pair<JSONCreator,JSONCreatorSync>> m_kJSONCreatorMap;
	VeStringMap<ResCreator> m_kResCreatorMap;
	VeStringMap<VeFixedString> m_kResExtMap;
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
