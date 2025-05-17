@echo off
REM Usage: ffmpegVideo2Twitter.bat input.avi output.mp4 [--no-audio]

if "%~1"=="" (
    echo Please provide input file as first parameter.
    exit /b 1
)

if "%~2"=="" (
    echo Please provide output file as second parameter.
    exit /b 1
)

set "AUDIO_OPTS=-c:a aac -b:a 192k"

if "%~3"=="--no-audio" (
    set "AUDIO_OPTS=-an"
)

ffmpeg -i "%~1" -vf "scale=1600:1000:flags=neighbor,pad=1920:1080:(ow-iw)/2:(oh-ih)/2:black" -c:v libx264 -crf 18 -preset slow -pix_fmt yuv420p %AUDIO_OPTS% "%~2"
