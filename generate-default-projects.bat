@ECHO OFF

start "" "cmake" -G"Visual Studio 9 2008" -H"%CD%" -B"%CD%/build"
echo.
echo Inviwo projects are now located in %CD%\build
echo.

pause

exit