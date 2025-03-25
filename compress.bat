@echo off
setlocal

:: Check if a suffix was provided
if "%~1"=="" (
    echo Usage: compress.bat suffix
    exit /b 1
)

:: Set the suffix from the first argument
set SUFFIX=%~1

:: Define the tar command (use full path to ensure Windows tar is used)
set TAR="C:\Windows\System32\tar.exe"

:: Check if tar.exe exists
if not exist %TAR% (
    echo Error: tar.exe not found!
    exit /b 1
)

:: Run tar to create the zip archive
%TAR% -a -c -f pes94_%SUFFIX%.zip -C out *.*

move pes94_%SUFFIX%.zip out/

echo Archive pes94_%SUFFIX%.zip created successfully.
exit /b 0