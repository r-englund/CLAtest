@ECHO OFF

REM Specify the used Visual Studio version
SET QMAKESPEC = win32-msvc2008
REM SET QMAKESPEC = win32-msvc2010

ECHO Processing voreen.pro...
cd ext\voreen
qmake.exe -tp vc voreen.pro
IF ERRORLEVEL 1 GOTO :ERROR
cd ..\..
ECHO.

ECHO Processing inviwo-core.pro...
cd src\core
qmake.exe -tp vc inviwo-core.pro
IF ERRORLEVEL 1 GOTO :ERROR
cd ..\..
ECHO.

ECHO Processing glutminimum.pro...
cd apps\glutminimum
qmake.exe -tp vc glutminimum.pro
IF ERRORLEVEL 1 GOTO :ERROR
cd ..\..
ECHO.

ECHO SUCCESS: Inviwo project files succesfully generated.
ECHO.
PAUSE
GOTO :EOF

:ERROR
ECHO.
ECHO ERROR: Inviwo project file generation failed!
ECHO.
PAUSE
