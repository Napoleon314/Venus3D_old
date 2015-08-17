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
public:
	enum Task
	{
		TASK_FILE,
		TASK_PROCESS,
		TASK_NUM
	};

	typedef std::function<VeDirectoryPtr(const VeChar8*, bool)>
		DirCreator;

	typedef std::function<VeBinaryIStreamPtr(const VeChar8*)>
		StreamCreator;

	VeResourceManager() noexcept;

	virtual ~VeResourceManager() noexcept;

	inline VeTaskQueue& GetTaskQueue(Task eTask) noexcept;

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

protected:
	VeTaskQueue m_akTaskQueues[TASK_NUM];
	DirCreator m_kDefaultDirCreator;
	VeStringMap<DirCreator> m_kDirCreatorMap;
	StreamCreator m_kDefaultStreamCreator;
	VeStringMap<StreamCreator> m_kStreamCreatorMap;

};

VeSmartPointer(VeResourceManager);

//#define ve_res_mgr VeResourceManager::GetSingleton()

#include "VeResourceManager.inl"
