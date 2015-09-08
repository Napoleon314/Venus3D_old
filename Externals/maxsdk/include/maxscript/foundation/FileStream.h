 /*	
  *		FileStream.h - MAXScript
  *
  *			Copyright � John Wainwright 1996
  *
  */
 
#pragma once
#include "CharStream.h"
#include "CharBinaryStream.h"
 
 visible_class_debug_ok (FileStream)
 
 class FileStream : public CharBinaryStream
 {
 public:
 	MSTR	mode;
 	CharStream*	log;
	Parser* reader;
	// Constructor / Destructor
 			ScripterExport FileStream();
 		    ScripterExport ~FileStream();
 
 			classof_methods (FileStream, CharStream);
 #	define	is_filestream(v) ((DbgVerify(!is_sourcepositionwrapper(v)), (v))->tag == class_tag(FileStream))
	void	collect();
 	void	gc_trace();
 	ScripterExport void	sprin1(CharStream* s);
 
 #include "../macros/define_implementations.h"
 #	include "../protocols/streams.inl"

	virtual void seek(int pos);

 	/* internal char stream protocol */
	/**
	* Open an encrypted text file for read.
	* \param[in]  ifile_name The file name.
	* \param[in]  seed The seed encryption value.
	* \param[in]  encoding The encoding to use if if can not be determined from the file contents, If -1, a default
	*			encoding based on the file language settings in Preference /Files (exposed through Interface14) will
	*			be used.
	* \return The FileStream instance itself if max is able to open the file or undefined if it fails.
	*/
 	ScripterExport FileStream* open_decrypt(const MCHAR* ifile_name, int seed, unsigned int encoding = -1);
	/**
	* Open a text file
	* \param[in]  ifile_name The file name.
	* \param[in]  imode The file open mode. See the MAXScript Help file, topic FileStream Values, for a description of the mode string values.
	* \param[in]  encoding The encoding to use if if can not be determined from the file contents, If -1, a default
	*			encoding based on the file language settings in Preference /Files (exposed through Interface14) will
	*			be used.
	* \return The FileStream instance itself if max is able to open the file or undefined if it fails.
	*/
 	ScripterExport FileStream* open(const MCHAR* ifile_name, const MCHAR* imode, unsigned int encoding = -1);
 	void	log_to(CharStream* log);
 	void	close_log();

	void	undo_lookahead();

 protected:
	virtual void ThrowNotOpenedError() const;

 protected:
	MaxSDK::Util::BinaryStream* OpenStream(const MCHAR* fn, const MSTR& mode);
 };
