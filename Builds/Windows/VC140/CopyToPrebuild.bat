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
if not exist "%SDK_DIRECTORY%\Prebuild"																						mkdir "%SDK_DIRECTORY%\Prebuild"
if not exist "%SDK_DIRECTORY%\Prebuild\Include"																		mkdir "%SDK_DIRECTORY%\Prebuild\Include"
if not exist "%SDK_DIRECTORY%\Prebuild\Windows"																		mkdir "%SDK_DIRECTORY%\Prebuild\Windows"
if not exist "%SDK_DIRECTORY%\Prebuild\Windows"																		mkdir "%SDK_DIRECTORY%\Prebuild\Windows"
if not exist "%SDK_DIRECTORY%\Prebuild\Windows\%2"																mkdir "%SDK_DIRECTORY%\Prebuild\Windows\%2"
if not exist "%SDK_DIRECTORY%\Prebuild\Windows\%2\%3"															mkdir "%SDK_DIRECTORY%\Prebuild\Windows\%2\%3"
if not exist "%SDK_DIRECTORY%\Prebuild\Windows\%2\%3\%4"													mkdir "%SDK_DIRECTORY%\Prebuild\Windows\%2\%3\%4"
if exist "%VENUS_DIRECTORY%\Externals\%1\%1" if not exist "%SDK_DIRECTORY%\Prebuild\Include\%1" mkdir "%SDK_DIRECTORY%\Prebuild\Include\%1"

echo (only copying new or modified files)

:: COPY STANDARD FILES
if not exist "%VENUS_DIRECTORY%\Externals\%1\%1" if exist "%VENUS_DIRECTORY%\Externals\%1\*.h" xcopy /D /F /I /R /Y "%VENUS_DIRECTORY%\Externals\%1\*.h" "%SDK_DIRECTORY%\Prebuild\Include"
if exist "%VENUS_DIRECTORY%\Externals\%1\%1\*.h" xcopy /D /F /I /R /Y "%VENUS_DIRECTORY%\Externals\%1\%1\*.h" "%SDK_DIRECTORY%\Prebuild\Include\%1"

if exist "%VENUS_DIRECTORY%\Binaries\Windows\%2\%3\%4\%1.lib" xcopy /D /F /I /R /Y "%VENUS_DIRECTORY%\Binaries\Windows\%2\%3\%4\%1.lib" "%SDK_DIRECTORY%\Prebuild\Windows\%2\%3\%4"
if exist "%VENUS_DIRECTORY%\Binaries\Windows\%2\%3\%4\%1.dll" xcopy /D /F /I /R /Y "%VENUS_DIRECTORY%\Binaries\Windows\%2\%3\%4\%1.dll" "%SDK_DIRECTORY%\Prebuild\Windows\%2\%3\%4"
if exist "%VENUS_DIRECTORY%\Binaries\Windows\%2\%3\%4\%1.pdb" xcopy /D /F /I /R /Y "%VENUS_DIRECTORY%\Binaries\Windows\%2\%3\%4\%1.pdb" "%SDK_DIRECTORY%\Prebuild\Windows\%2\%3\%4"

if exist "%SDK_DIRECTORY%\Prebuild\Include" attrib +R "%SDK_DIRECTORY%\Prebuild\Include\*"
if exist "%SDK_DIRECTORY%\Prebuild\Include\%1" attrib +R "%SDK_DIRECTORY%\Prebuild\Include\%1\*"
if exist "%SDK_DIRECTORY%\Prebuild\Windows\%2\%3\%4" attrib +R "%SDK_DIRECTORY%\Prebuild\Windows\%2\%3\%4\*"

exit /b

:==ERROR_NO_SDK
echo %0: fatal error: Cannot locate SDK directory.
exit 1
