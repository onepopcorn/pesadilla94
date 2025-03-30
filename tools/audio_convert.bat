@REM audio_convert.bat "C:\path\to\source" "C:\path\to\destination"

@echo off
setlocal enabledelayedexpansion

:: Check if both parameters are provided
if "%~1"=="" (
    echo Usage: convert_u8.bat [source_folder] [destination_folder]
    exit /b
)
if "%~2"=="" (
    echo Usage: convert_u8.bat [source_folder] [destination_folder]
    exit /b
)

set "SRC=%~1"
set "DEST=%~2"

:: Ensure destination folder exists
if not exist "%DEST%" mkdir "%DEST%"

for %%F in ("%SRC%\*.wav") do (
    ffmpeg -i "%%F" -ar 11025 -ac 1 -acodec pcm_u8 "%DEST%\u8_%%~nxF"
)

echo Conversion complete!