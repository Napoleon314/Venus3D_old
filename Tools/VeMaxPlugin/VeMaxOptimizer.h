////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMaxPlugin
//  File name:   VeMaxOptimizer.h
//  Created:     2015/09/27 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

void VeMaxWeldVertices(VeMaxMesh& mesh, VeFloat32 f32Epsilon = 0) noexcept;

void VeMaxOptimizeMesh(VeMaxMesh& mesh) noexcept;
