#pragma once

#include "../../WindowsDefines.h"
#include "../ScripterExport.h"
#include "../../strbasic.h"

class Editor;

struct EditFileResult
{
	bool fileNotFound;
	bool fileAlreadyOpen;
	bool fileNewOpen;
	HWND edit_hwnd;

	EditFileResult() : fileNotFound(false), fileAlreadyOpen(false), fileNewOpen(false), edit_hwnd(NULL) {}
};

class EditorStyleDef {
public:
	MCHAR font[200];
	int size;
	long fore;
	long back;
	bool bold;
	bool italics;
	bool eolfilled;
	bool underlined;
	enum flags { sdNone = 0, sdFont = 0x1, sdSize = 0x2, 
		sdFore = 0x4, sdBack = 0x8, sdBold = 0x10, sdItalics = 0x20, 
		sdEOLFilled = 0x40, sdUnderlined = 0x80} specified;
	EditorStyleDef() { memset(this, 0, sizeof(EditorStyleDef)); }
};


class Editor_Interface {
public:
	Editor *Editor_Instance;

	enum OpenFlags {
		ofNone=0, 			// Default
		ofNoSaveIfDirty=1, 	// Suppress check for unsaved changes
		ofForceLoad=2,		// Reload file even if already in a buffer
		ofPreserveUndo=4,	// Do not delete undo history
		ofQuiet=8			// Avoid "Could not open file" message
	};

	Editor_Interface(Editor *instance) : Editor_Instance(instance) {}
	virtual ~Editor_Interface() {}
	virtual void DeleteThis();

	virtual void SetTabWidth(int tabWidth);

	virtual HWND GetMainHWND();
	virtual HWND GetEditHWND();
	virtual bool Run(HWND parentHWND);
	virtual bool EditFile(const MCHAR* filename = NULL, bool useOpenfilenameDialogIfFilenameNull = false, const MCHAR* initialPath = NULL, 
						  int openToPos = -1, Editor_Interface::OpenFlags of = Editor_Interface::ofNone, bool unhide = true, 
						  bool setFocus = true);
	virtual int NumberDocuments();
	virtual const MCHAR* GetDocumentFilename(int i);
	virtual void SetDocumentFilename(int i, const MCHAR* file_name);
	virtual const MCHAR* GetDocumentFilename();
	virtual void SetDocumentFilename(const MCHAR* file_name);

	virtual bool IsEditorMessage(MSG &msg);
	virtual bool CloseEditor();
	virtual void DeleteEditor();
	virtual void Show(bool show, bool setFocus = false);
	virtual void SetTitle(const MCHAR* title);
	virtual void SetProperty(const MCHAR* prop_name, const MCHAR* prop_value);
	virtual const MCHAR* GetProperty(const MCHAR* prop_name);
	virtual int GetIntProperty(const MCHAR* prop_name, int defaultValue = 0);
	virtual void SetCodePageAndCharacterSet(int code_page, int character_set);
	virtual void GetCodePageAndCharacterSet(int &code_page, int &character_set);
	virtual EditorStyleDef GetEditorStyleDef(const MCHAR* lang, int style);
};

class MXSEditor_Interface {
public:
	MXSEditor_Interface() {}
	virtual ~MXSEditor_Interface() {}
	virtual void DeleteThis();

	virtual void DisableAccelerators();
	virtual void EnableAccelerators();
	virtual void EditorShowStateChanged(bool open);
	virtual void ShowListener();
	virtual void PositionListenerAtEOF();
	virtual void ExecuteString(const wchar_t *executeString, const MCHAR *file_name, unsigned int file_offset, unsigned int file_linenum);
	virtual void DoLoadProperties();
	virtual const MCHAR* GetLocalUserDataDirectory();
	virtual int DisplayHelp(const MCHAR* word);

	virtual void NewRollout(HWND mainHWND, HWND editHWND);
	virtual void EditRollout(HWND mainHWND, HWND editHWND);

	virtual UINT GetSaveCodePage();
	virtual UINT GetLoadCodePage();
};

class MXSEditor_Interface2 : public MXSEditor_Interface {
public:
	virtual UINT GetSaveCodePageEx(bool allow_utf8);
	virtual bool LegacyFilesCanBeStoredUsingUTF8();
};
	
class MXS_Editor_Interface {
public:
	Editor_Interface *Editor_Interface_Instance;
	MXSEditor_Interface2 *MXSEditor_Interface_Instance;

	MXS_Editor_Interface() : Editor_Interface_Instance(NULL), MXSEditor_Interface_Instance(NULL) {}
	~MXS_Editor_Interface() 
	{
		if (Editor_Interface_Instance)
			Editor_Interface_Instance->DeleteThis();
		if (MXSEditor_Interface_Instance)
			MXSEditor_Interface_Instance->DeleteThis();
	}

	HWND Editor_GetMainHWND() { return Editor_Interface_Instance->GetMainHWND(); }
	HWND Editor_GetEditHWND() { return Editor_Interface_Instance->GetEditHWND(); }
	bool Editor_Run(HWND parentHWND) { return Editor_Interface_Instance->Run(parentHWND); }
	bool Editor_EditFile(const MCHAR* filename = NULL, bool useOpenfilenameDialogIfFilenameNull = false, const MCHAR* initialPath = NULL, 
						 int openToPos = -1, Editor_Interface::OpenFlags of = Editor_Interface::ofNone, bool unhide = true, 
						 bool setFocus = true) 
		{ return Editor_Interface_Instance->EditFile(filename, useOpenfilenameDialogIfFilenameNull, initialPath, openToPos, of, unhide, setFocus); }

	int Editor_NumberDocuments() { return Editor_Interface_Instance->NumberDocuments(); }
	const MCHAR* Editor_GetDocumentFilename(int i) { return Editor_Interface_Instance->GetDocumentFilename(i); }
	void Editor_SetDocumentFilename(int i, const MCHAR* file_name) { Editor_Interface_Instance->SetDocumentFilename(i, file_name); }
	const MCHAR* Editor_GetDocumentFilename() { return Editor_Interface_Instance->GetDocumentFilename(); }
	void Editor_SetDocumentFilename(const MCHAR* file_name) { Editor_Interface_Instance->SetDocumentFilename(file_name); }
	bool Editor_IsEditorMessage(MSG &msg) { return Editor_Interface_Instance->IsEditorMessage(msg); }
	void Editor_SetTabWidth(int tabWidth) { Editor_Interface_Instance->SetTabWidth(tabWidth); }
	bool Editor_CloseEditor() { return Editor_Interface_Instance->CloseEditor(); }
	void Editor_DeleteEditor() { Editor_Interface_Instance->DeleteEditor(); }

	void Editor_Show(bool show, bool setFocus=false)
		{ Editor_Interface_Instance->Show(show, setFocus); }
	void Editor_SetTitle(const MCHAR* title)
		{ Editor_Interface_Instance->SetTitle(title); }
	void Editor_SetProperty(const MCHAR* prop_name, const MCHAR* prop_value)
		 { Editor_Interface_Instance->SetProperty(prop_name, prop_value); }
	const MCHAR* Editor_GetProperty(const MCHAR* prop_name)
		{ return Editor_Interface_Instance->GetProperty(prop_name); }
	int Editor_GetIntProperty(const MCHAR* prop_name, int defaultValue = 0)
		{ return Editor_Interface_Instance->GetIntProperty(prop_name, defaultValue); }
	void Editor_SetCodePageAndCharacterSet(int code_page, int character_set)  
		 { Editor_Interface_Instance->SetCodePageAndCharacterSet(code_page, character_set); }
	void Editor_GetCodePageAndCharacterSet(int &code_page, int &character_set)  
		 { Editor_Interface_Instance->GetCodePageAndCharacterSet(code_page, character_set); }
	EditorStyleDef Editor_GetEditorStyleDef(const MCHAR* lang, int style)
		{ return Editor_Interface_Instance->GetEditorStyleDef(lang, style); }

	void MXS_DisableAccelerators()
		{ MXSEditor_Interface_Instance->DisableAccelerators(); }
	void MXS_EnableAccelerators()
		{ MXSEditor_Interface_Instance->EnableAccelerators(); }
	void MXS_EditorShowStateChanged(bool open)
		{ MXSEditor_Interface_Instance->EditorShowStateChanged(open); }
	void MXS_ShowListener()
		{ MXSEditor_Interface_Instance->ShowListener(); }
	void MXS_PositionListenerAtEOF()
		{ MXSEditor_Interface_Instance->PositionListenerAtEOF(); }
	void MXS_ExecuteString(const wchar_t *executeString, const MCHAR *file_name, unsigned int file_offset, unsigned int file_linenum)
		{ MXSEditor_Interface_Instance->ExecuteString(executeString, file_name, file_offset, file_linenum); }
	void MXS_DoLoadProperties()
		{ MXSEditor_Interface_Instance->DoLoadProperties(); }
	const MCHAR* MXS_GetLocalUserDataDirectory()
		{ return MXSEditor_Interface_Instance->GetLocalUserDataDirectory(); }
	void MXS_DisplayHelp(const MCHAR* word)
		{ MXSEditor_Interface_Instance->DisplayHelp(word); }

	void MXS_NewRollout(HWND mainHWND, HWND editHWND)
		{ MXSEditor_Interface_Instance->NewRollout(mainHWND, editHWND); }
	void MXS_EditRollout(HWND mainHWND, HWND editHWND)
		{ MXSEditor_Interface_Instance->EditRollout(mainHWND, editHWND); }

	UINT GetSaveCodePage()
		{ return MXSEditor_Interface_Instance->GetSaveCodePage(); }
	UINT GetLoadCodePage()
		{ return MXSEditor_Interface_Instance->GetLoadCodePage(); }

	UINT GetSaveCodePageEx(bool allow_utf8)
		{ return MXSEditor_Interface_Instance->GetSaveCodePageEx(allow_utf8); }
	UINT LegacyFilesCanBeStoredUsingUTF8()
		{ return MXSEditor_Interface_Instance->LegacyFilesCanBeStoredUsingUTF8(); }


};

extern ScripterExport MXS_Editor_Interface *the_MXS_Editor_Interface;
