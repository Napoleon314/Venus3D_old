//**************************************************************************/
// Copyright (c) 1998-2011 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: INI File I/O
// AUTHOR:      Christian Roy
// DATE:        June 2011
//***************************************************************************/

#pragma once

#include "../utilexp.h"
#include <WTypes.h>

namespace MaxSDK
{
    namespace Util
    {
        /** 
         * These service should replace every occurrence of the standard WIN32 implementation. 
         * The WIN32 implementation specifies that when the file name contains UNICODE character, 
         * the file will be created with a UNICODE BOM.  This is ABSOLUTLY INCORRECT!!!
         * In order to properly save UNICODE INI file, we need to enforce that every INI file
         * has a BOM token.
         **/

        /**
         * Copies a string into the specified section of an initialization file.
         * This service will create the new INI file with a UTF-16LE
         * and delegate to WIN32 implementation to actually write the new key value pair.
         * If the INI file exist and is not a UNICODE file, it will be 
         * re-created as a UTF-16LE and the key|value pair will be written in the proper 
         * section.
         * @param lpAppName     The name of the section to which the string will be copied. 
         *                      If the section does not exist, it is created. The name of the section is case-independent; 
         *                      the string can be any combination of uppercase and lowercase letters.
         * @param lpKeyName     The name of the key to be associated with a string. 
         *                      If the key does not exist in the specified section, it is created. 
         *                      If this parameter is NULL, the entire section, including all 
         *                      entries within the section, is deleted.
         * @param lpString      A null-terminated string to be written to the file. If this parameter 
         *                      is NULL, the key pointed to by the lpKeyName parameter is deleted.
         * @param lpFileName    The name of the initialization file.
         * @return              If the function successfully copies the string to the initialization 
         *                      file, the return value is nonzero.
         *                      If the file was created using Unicode characters, the function writes 
         *                      Unicode characters to the file. Otherwise, the function writes ANSI characters.
        */
        UtilExport BOOL WritePrivateProfileString(
            LPCTSTR lpAppName,
            LPCTSTR lpKeyName,
            LPCTSTR lpString,
            LPCTSTR lpFileName
        );

        /** 
         * Copies data into a key in the specified section of an initialization file. 
         * As it copies the data, the function calculates a checksum and appends it to the end of the data. 
         * @param lpszSection   The name of the section to which the string will be copied. If the section 
         * does not exist, it is created. The name of the section is case independent, 
         * the string can be any combination of uppercase and lowercase letters.
         * @param lpszKey       The name of the key to be associated with a string. If the key 
         *                      does not exist in the specified section, it is created. If this parameter is 
         *                      NULL, the entire section, including all keys and entries within the section, 
         *                      is deleted.
         * @param lpStruct      The data to be copied. If this parameter is NULL, the key is deleted.
         * @param uSizeStruct   The size of the buffer pointed to by the lpStruct parameter, in bytes.
         * @param lpFileName    The name of the initialization file.
         * @return              If the function successfully copies the string to the initialization file, 
         *                      the return value is nonzero.
         *                      If the function fails, or if it flushes the cached version of the most 
         *                      recently accessed initialization file, the return value is zero. 
         *                      To get extended error information, call GetLastError.
         */
        UtilExport BOOL WritePrivateProfileStruct(
            LPCTSTR lpszSection,
            LPCTSTR lpszKey,
            LPVOID lpStruct,
            UINT uSizeStruct,
            LPCTSTR lpFileName
        );

        /** 
         * Replaces the keys and values for the specified section in an initialization file.
         * @param lpAppName The name of the section in which data is written. This section 
         *                  name is typically the name of the calling application.
         * @param lpString  The new key names and associated values that are to be written 
         *                  to the named section. This string is limited to 65,535 bytes.
         * @param lpFileName The name of the initialization file. If this parameter does not contain 
         *                  a full path for the file, the function searches the Windows directory for 
         *                  the file. If the file does not exist and lpFileName does not contain a full path, 
         *                  the function creates the file in the Windows directory.
         *                  If the file exists and was created using Unicode characters, the function 
         *                  writes Unicode characters to the file. Otherwise, the function creates
         *                  a file using ANSI characters.
         * @return          If the function succeeds, the return value is nonzero.
         *                  If the function fails, the return value is zero. 
         *                  To get extended error information, call GetLastError.
         */
        UtilExport BOOL WritePrivateProfileSection(
            LPCTSTR lpAppName,
            LPCTSTR lpString,
            LPCTSTR lpFileName
        );
    }
}

