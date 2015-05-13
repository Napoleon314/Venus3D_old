@echo off

set SDK_DIRECTORY=not_found

if exist "..\..\..\..\..\SDK"																											set VENUS_DIRECTORY=..\..\..\..\..
if exist "..\..\..\..\SDK"																												set VENUS_DIRECTORY=..\..\..\..
if exist "..\..\..\SDK"																														set VENUS_DIRECTORY=..\..\..
if exist "..\..\SDK"																															set VENUS_DIRECTORY=..\..
if exist "..\SDK"																																	set VENUS_DIRECTORY=..
if (%VENUS_DIRECTORY%)==(not_found)																								goto :ERROR_NO_SDK
set SDK_DIRECTORY="%VENUS_DIRECTORY%\SDK"
set VENUS_DIRECTORY="%VENUS_DIRECTORY%"

echo (Creating SDK directories)
if not exist "%SDK_DIRECTORY%\Include"																						mkdir "%SDK_DIRECTORY%\Include"
if not exist "%SDK_DIRECTORY%\Windows"																						mkdir "%SDK_DIRECTORY%\Windows"
if not exist "%SDK_DIRECTORY%\Windows\%2"																					mkdir "%SDK_DIRECTORY%\Windows\%2"
if not exist "%SDK_DIRECTORY%\Windows\%2\Win32"																		mkdir "%SDK_DIRECTORY%\Windows\%2\Win32"
if not exist "%SDK_DIRECTORY%\Windows\%2\Win32\Debug"															mkdir "%SDK_DIRECTORY%\Windows\%2\Win32\Debug"
if not exist "%SDK_DIRECTORY%\Windows\%2\Win32\Release"														mkdir "%SDK_DIRECTORY%\Windows\%2\Win32\Release"
if not exist "%SDK_DIRECTORY%\Windows\%2\Win32\DebugLib"													mkdir "%SDK_DIRECTORY%\Windows\%2\Win32\DebugLib"
if not exist "%SDK_DIRECTORY%\Windows\%2\Win32\ReleaseLib"												mkdir "%SDK_DIRECTORY%\Windows\%2\Win32\ReleaseLib"
if not exist "%SDK_DIRECTORY%\Windows\%2\x64"																			mkdir "%SDK_DIRECTORY%\Windows\%2\x64"
if not exist "%SDK_DIRECTORY%\Windows\%2\x64\Debug"																mkdir "%SDK_DIRECTORY%\Windows\%2\x64\Debug"
if not exist "%SDK_DIRECTORY%\Windows\%2\x64\Release"															mkdir "%SDK_DIRECTORY%\Windows\%2\x64\Release"
if not exist "%SDK_DIRECTORY%\Windows\%2\x64\DebugLib"														mkdir "%SDK_DIRECTORY%\Windows\%2\x64\DebugLib"
if not exist "%SDK_DIRECTORY%\Windows\%2\x64\ReleaseLib"													mkdir "%SDK_DIRECTORY%\Windows\%2\x64\ReleaseLib"

echo (only copying new or modified files)

:: COPY STANDARD FILES
if exist "%VENUS_DIRECTORY%\Source\%1\Ve*.h"																			xcopy /D /F /I /R /Y "%VENUS_DIRECTORY%\Source\%1\Ve*.h"																		"%SDK_DIRECTORY%\Include"
if exist "%VENUS_DIRECTORY%\Source\%1\Ve*.inl"																		xcopy /D /F /I /R /Y "%VENUS_DIRECTORY%\Source\%1\Ve*.inl"																	"%SDK_DIRECTORY%\Include"

if exist "%VENUS_DIRECTORY%\Binaries\Windows\%2\%3\%4\%1.lib"											xcopy /D /F /I /R /Y "%VENUS_DIRECTORY%\Binaries\Windows\%2\%3\%4\%1.lib"										"%SDK_DIRECTORY%\Windows\%2\%3\%4"
if exist "%VENUS_DIRECTORY%\Binaries\Windows\%2\%3\%4\%1.dll"											xcopy /D /F /I /R /Y "%VENUS_DIRECTORY%\Binaries\Windows\%2\%3\%4\%1.dll"										"%SDK_DIRECTORY%\Windows\%2\%3\%4"
if exist "%VENUS_DIRECTORY%\Binaries\Windows\%2\%3\%4\%1.pdb"											xcopy /D /F /I /R /Y "%VENUS_DIRECTORY%\Binaries\Windows\%2\%3\%4\%1.pdb"										"%SDK_DIRECTORY%\Windows\%2\%3\%4"

if exist "%SDK_DIRECTORY%\Include" attrib +R "%SDK_DIRECTORY%\Include\*"
if exist "%SDK_DIRECTORY%\Windows\%2\%3\%4" attrib +R "%SDK_DIRECTORY%\Windows\%2\%3\%4\*"

exit /b

:==ERROR_NO_SDK
echo %0: fatal error: Cannot locate SDK directory.
exit 1
