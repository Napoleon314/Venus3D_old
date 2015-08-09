////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Inline File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VeMain
//  File name:   VeEngine.inl
//  Created:     2015/08/06 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
inline const VeEventQueuePtr& VeEngine::GetEventQueue() noexcept
{
	return m_spEventQueue;
}
//--------------------------------------------------------------------------
inline const VeVideoDevicePtr& VeEngine::GetVideoDevice() noexcept
{
	return m_spVideo;
}
//--------------------------------------------------------------------------
inline const VeKeyboardPtr& VeEngine::GetKeyboard() noexcept
{
	return m_spKeyboard;
}
//--------------------------------------------------------------------------
inline const VeMousePtr& VeEngine::GetMouse() noexcept
{
	return m_spMouse;
}
//--------------------------------------------------------------------------
inline const VeRendererPtr& VeEngine::GetRenderer() noexcept
{
	return m_spRenderer;
}
//--------------------------------------------------------------------------
