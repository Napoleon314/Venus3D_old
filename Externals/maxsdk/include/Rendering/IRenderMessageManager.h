//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2014 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////
#pragma once

#include "../maxheap.h"
#include "../strbasic.h"
#include "../maxapi.h"

#define RENDERMESSAGEMANAGER_INTERFACE	Interface_ID(0x794e519b, 0x3a74408a)

//! \brief An interface to manage the renderer messages (error, warning, progress, etc.). 
/*! Clients log messages to this manager, and the manager will automatically dispatch the messages  
 to the message window, or to Max log system (if Max is running in quite mode or network rendering is taking place).  
 \remark A pointer to this interface may be retrieved using GetRenderMessageManager().
*/
class IRenderMessageManager: public MaxHeapOperators
{
public: 

	//! \brief Different sources for renderer message.
	enum MessageSource
	{
		//! Production Renderer
		kSource_ProductionRenderer = 0, 

		//! Material Editor Renderer
		kSource_MEditRenderer,			

		//! Active Shade Renderer
		kSource_ActiveShadeRenderer,	

		kSource_Count,
	};

	//! \brief Type of a message.
	enum MessageType
	{
		//! System/module crash, etc. 
		//! Fatal messages are always shown on the message window. 
		kType_Fatal			= 0x0001, 

		//! Coverable errors. 
		//! Error messages are always shown on the message window. 
		kType_Error			= 0x0002, 

		//! Minor issues. 
		//! Warning messages are always shown on the message window. 
		kType_Warning		= 0x0004, 

		//! Brief information messages for monitoring, debugging purposes and so on.
		//! Client can choose to show/hide information messages on the message window. 
		//! \see GetShowInformationMessages, SetShowInformationMessages
		kType_Info			= 0x0008, 

		//! Messages to provide progress information for a time-consuming job.
		//! Client can choose to show/hide progress messages on the message window. 
		//! \see GetShowProgressMessages, SetShowProgressMessages
		kType_Progress		= 0x0010,

		//! Detailed information messages solely for debugging purpose.
		//! These messages are never shown on the message window. 
		//! Client can choose to log debug messages to the log file (if present). 
		//! \see GetLogDebugMessages, SetLogDebugMessages
		kType_Debug			= 0x0020, 

		//! Messages from the system. Client does not use this flag. 
		kType_System		= 0x0040,
	};

public: 
	virtual ~IRenderMessageManager() {}

	//! \brief Open the message window. 
	//! \return True if the window is opened successfully, false otherwise. 
	virtual bool OpenMessageWindow() = 0;

	//! \brief Clear all the displayed messages on the message window.
	virtual void ClearDisplayedMessages() = 0;

	//! \brief Destroy the message window. 
	virtual void CloseMessageWindow() = 0;

	//! \brief Make the message window invisible. 
	virtual void HideMessageWindow() = 0;

	//! \brief Log a message to the manager. 
	//! \remarks This function is thread-safe. 
	//! Message manager caches incoming messages to a queue and returns immediately. 
	//! Then from main thread it regularly flushes the pending messages to either the message window, 
	//! or Max log system, depending on if Max is running in "window" mode.
	//! \param msgSource Source of the message; determines which tab, in the render message dialog, the message is reports in.
	//! \param msgType Type of the message for purposes of <b>reporting to the user</b>. Affects how the message is reported to the render message window. 
	//! \param systemLogType Type of the message for purposes of <b>processing by the system</b>. See systemErrorLogMessageTypes in log.h. 
    //! \remark The difference between <b>msgType</b> and <b>systemLogType</b> is particularly important when reporting errors. A msgType of kType_Error will cause
    //!     the message to be shown red in the render message window, while a systemLogType of SYSLOG_ERROR will cause a network render to abort (if
    //!     the system is set to abort on errors). Both types should not necessarily be set identically! For example, an error in scene translation
    //!     (say, a material not supported by the renderer) could be reported as an error to the user, but would most likely by a warning to the system
    //!     as one would not expect network rendering to fail.
	//! This will be used if the manager attempts to send the message to Max main log system. 
	//! Set 0 to let the manager automatically assign a proper value. 
	//! \param msgBody Text of the message.
	//! \return True if the message is successfully logged, false otherwise. 
	virtual bool LogMessage(MessageSource msgSource, MessageType msgType, DWORD systemLogType, const MCHAR* msgBody) = 0;

	//! \brief Flush pending messages from a certain source. 
	//! \remarks This function must be called from main thread. 
	//! If the manager is working in non-window mode, and the source is kSource_Production, 
	//! the pending messages will be sent to 3ds Max main log system. 
	//! If the manager is working in window mode, the pending messages will be flushed to the 
	//! message window. 
	//! This function will also get called automatically from a system timer, to ensure that
	//! pending messages are always flushed regularly. 
	//! \param source Source of the message.
	virtual void FlushPendingMessages(MessageSource source) = 0;

	//! \brief Clear displayed messages from a certain source.
	//! \param source Source of the messages that client wants to clear.
	virtual void ClearDisplayedMessages(MessageSource source) = 0;

	//! \brief Open the log file for message logging if possible. 
	//! Typically this is called from a renderer's Open() implementation.
	//! \return True if the log file is successfully created.
	virtual bool OpenLogFile(MessageSource source) = 0;

	//! \brief Flush buffer to the log file for specific message source.
	virtual void FlushLogFile(MessageSource source) = 0;

	//! \brief Close log file for specific message source.
	//! Typically this is called from a renderer's Close() implementation.
	virtual void CloseLogFile(MessageSource source) = 0;

	//! \brief Callback function for client to process the "preference option changed" notification. 
	//! \param param A pointer to client data. This will be exactly the same pointer
	//! provided by client on calling RegisterChangeCallback, as parameter "param". Client is responsible 
	//! for interpreting the pointer to a specific type.
	//! \see RegisterChangeCallback
	typedef void(*rmmPreferencesCallback)(void* param);

	//! \brief Register a client defined callback to get notification whenever a preference option is changed
	//! via UI operation, API call, and max-script. 
	//! \param callback The callback function client wants to register. 
	//! \param param A pointer to client data. This pointer will be used as the only parameter of the callback function.
	virtual void RegisterChangeCallback(rmmPreferencesCallback callback, void* param) = 0;

	//! \brief Unregister preference change callbacks.
	//! \param callback The callback function client wants to unregister. 
	//! \param param A pointer to client data. Make sure it matches the one passed along with RegisterChangeCallback().
	virtual void UnRegisterChangeCallback(rmmPreferencesCallback callback, void* param) = 0;

	//! \brief Get the "Open Message Window on Error" option. 
	//! \return True if the window will be opened whenever there's a fatal/error message sent in, false otherwise.
	//! \remarks Note that if Max is running in quite mode, the message window will never show up. 
	virtual bool GetOpenMessageWindowOnError() const = 0;

	//! \brief Set the "Open Message Window on Error" option.
	//! \param flag Set true if client wants the manager to open the window whenever there's a fatal/error message 
	//! sent in, false otherwise. 
	//! \remarks Note that if Max is running in quite mode or is doing network rendering, 
	//! the message window will never show up. 
	virtual void SetOpenMessageWindowOnError(bool flag) = 0;

	//! \brief Get the "Show/Log Information Messages" option. 
	//! \return True if the manager handles information messages. 
	virtual bool GetShowInformationMessages() const = 0;

	//! \brief Set the "Show/Log Information Messages" option.
	//! \param flag Set true if client wants the manager to handle information messages, false otherwise. 
	virtual void SetShowInformationMessages(bool flag) = 0;

	//! \brief Get the "Show/Log Progress Messages" option.
	//! \return True if the manager handles progress messages, false otherwise. 
	virtual bool GetShowProgressMessages() const = 0;

	//! \brief Set the "Show/Log Progress Messages" option.
	//! \param flag Set true if client wants the manager to handle progress messages, false otherwise.
	virtual void SetShowProgressMessages(bool flag) = 0;

	//! \brief Get the "Log Debug Messages (To File)" option.
	//! \return True if the manager attempts to log debug messages to the log file, false otherwise.
	virtual bool GetLogDebugMessages() const = 0;

	//! \brief Set the "Log Debug Messages (To File)" option.
	//! \param flag Set true if client wants the manager to log debug messages to the log file, false otherwise. 
	//! \remarks Client needs to turn on "Write Messages to File" option to make debug message logging working. 
	//! \see SetLogFileON
	virtual void SetLogDebugMessages(bool flag) = 0;

	//! \brief Get the message log file name template. 
	//! \remarks Different postfixes will be appended to this template for Production/MEditor/ActiveShade renderers. 
	//! For instance, if the template name is "MaxRendererMessages.log", 
	//! then messages log file name for Production renderer will be "MaxRendererMessages_Production.log". 
	//! \return The log file name template. 
	virtual const MCHAR* GetLogFileNameTemplate() = 0;

	//! \brief Set the message log file name template.
	//! \param filename The log file name template to set. 
	virtual void SetLogFileNameTemplate(const MCHAR* fileName) = 0;

	//! \brief Get the actual message log file name for a specific message source.
	//! \param source Source of the messages that the log file is related to.
	//! \return The actual log file name which is related to the message source. 
	virtual const MCHAR* GetLogFileName(MessageSource source) = 0;

	//! \brief Get the "Write Messages to File" option. 
	//! \return True if the manager logs messages to files, false otherwise. 
	virtual bool GetLogFileON() const = 0;

	//! \brief Set the "Write Messages to File" option.
	//! \param flag Set true if client wants the manager to log messages to files, false otherwise.
	virtual void SetLogFileON(bool flag) = 0;

	//! \brief Get the "Append to File" option. 
	//! \return True if the manager appends messages to log files (rather than creating new ones).
	virtual bool GetLogFileAppend() const = 0;

	//! \brief Set the "Append to File" option. 
	//! \param flag Set true if client wants the manager to append messages to log files (rather than creating new ones), false otherwise. 
	virtual void SetLogFileAppend(bool flag) = 0;
};

//! Returns a pointer to the IRenderMessageManager core interface.
inline IRenderMessageManager* GetRenderMessageManager() {

	return dynamic_cast<IRenderMessageManager*>(GetCOREInterface(RENDERMESSAGEMANAGER_INTERFACE));
}


