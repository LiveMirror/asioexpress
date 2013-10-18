@echo off
REM ==========================================================================
REM Post-Build Event script
REM
REM Usage:  PostBuild.cmd $(ProjectName)
REM ==========================================================================

setlocal
set ProjectName=%1
shift

if not "%ProjectName%"=="" goto start 
echo Error: ProjectName argument missing.
exit /b 1

:start
REM robocopy flags:
REM /A+:R   - Makes files read only
REM /S      - Copy subdirectories with content.
REM /PURGE  - Remove files that don't exist in source.
set Flags=/A+:R /S /PURGE

set Root=..\..\..
set SourceDir=%Root%\source\%ProjectName%
set DestDir=%Root%\include\%ProjectName%
set PublishPathRelative=..\..\..
set CopyCmd=%Root%\WinTools\Robocopy\robocopy.exe

if exist "%CopyCmd%" goto copyfiles
echo Error: The robocopy tool was not found: "%CopyCmd%"
exit /b 1

:copyfiles
echo.
echo Copying header files...

"%CopyCmd%" %Flags% %SourceDir%  %DestDir% *.h *.hpp /XD .svn /XD .git /XF pch.*
if errorlevel 4 goto error
exit /b 0
:error
