 /*	
  *		WindowStreams.h - MAXScript
  *
  *			Copyright © John Wainwright 1996
  *
  */
 
#pragma once
#include "CharStream.h"
#include "../kernel/value.h"

 visible_class_debug_ok(WindowStream)
 
 // Collectable::flags3 - bit 0 set if WindowStream is wrapping a MXS_Scintilla edit control. If not, standard
 // edit control is assumed
 class WindowStream : public CharStream
 {
 public:
 	HWND		window;
 	int			cursor;
 	MSTR		title;
 	Listener*	listener;
 	CharStream*	log;
	MCHAR		wputs_buf[512];			// edit control output buffer
	MCHAR*		wputs_p;
	HWND		echo;
 
	// Constructor / Destructor
 				WindowStream(HWND iwin);
 				WindowStream(const MCHAR* title);		/* for background scripts; window with given title will open if output generated */
 			   ~WindowStream();
 
 				classof_methods (WindowStream, CharStream);
 	void		collect();
 	void		gc_trace();
 	ScripterExport void	sprin1(CharStream* s);
 
 #include "../macros/define_implementations.h"
 	def_generic(sprint,	  "sprint");
  	use_generic( coerce,  "coerce");
 	use_generic( flush,   "flush"); 
		
 	/* internal MCHAR stream protocol */
 	
 	MaxSDK::Util::Char get_char() { return MaxSDK::Util::Char(); }
 	void               unget_char(const MaxSDK::Util::Char& c) { UNUSED_PARAM(c); }
 	MaxSDK::Util::Char peek_char() {return MaxSDK::Util::Char(); }
 	int			at_eos() { return TRUE; }
 	void		rewind() {}
 	void		flush_to_eol() {}
 
 	ScripterExport MaxSDK::Util::Char	putch(const MaxSDK::Util::Char& c);
 	ScripterExport const MCHAR*	puts(const MCHAR* str);
 	ScripterExport int		printf(const MCHAR *format, ...);
 	ScripterExport void		flush();

 	void		ensure_window_open();
 
 	void		log_to(CharStream* log);
 	void		close_log();
 	Listener*	get_listener() { return listener; }
 	CharStream* get_log() { return log; }

	// edit control output primitives
	ScripterExport MCHAR*	wputs(const MCHAR *str);
	ScripterExport void		wflush();
	ScripterExport MaxSDK::Util::Char wputch(const MaxSDK::Util::Char& c);
	ScripterExport int		wprintf(const MCHAR *format, ...);
				   void		set_echo_window(HWND wnd) { echo = wnd; }
				   void		echo_cur_line();
 				   int		get_cur_line(MSTR& line);
};
 
 
