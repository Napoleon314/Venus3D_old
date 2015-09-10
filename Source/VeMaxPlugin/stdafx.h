////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMaxPlugin
//  File name:   stdafx.h
//  Created:     2015/09/08 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#include <VeMainPch.h>

#include <istdplug.h>
#include <iparamb2.h>
#include <iparamm2.h>
#include <maxtypes.h>
#include <impexp.h>
#include <triobj.h>
#include <iskin.h>

#include <CS/Phyexp.h>
#include <CS/bipexp.h>
#include <IGame/IGame.h>
#include <IGame/IGameType.h>
#include <IGame/IGameObject.h> 
#include <IGame/IGameModifier.h>
#if defined(MAX_RELEASE_R12_ALPHA) && MAX_RELEASE >= MAX_RELEASE_R12_ALPHA
#include <IFileResolutionManager.h>
#else
#include <IPathConfigMgr.h>
#endif

#include "VeMaxEntry.h"
#include "VeMaxMix.h"
#include "VeMaxNormalRender.h"
#include "VeMaxScene.h"
#include "VeMaxSerializer.h"
#include "VeExportClass.h"
#include "VeMaxSceneLoader.h"
