@ECHO OFF

REM Specify the used Visual Studio version
SET QMAKESPEC = win32-msvc2008
REM SET QMAKESPEC = win32-msvc2010

ECHO Processing inviwo-core.pro...
cd src\core
qmake.exe -tp vc inviwo-core.pro
IF ERRORLEVEL 1 GOTO :ERROR
cd ..\..
ECHO.

ECHO Processing inviwo-qtwidgets.pro...
cd src\qt\widgets
qmake.exe -tp vc inviwo-qtwidgets.pro
IF ERRORLEVEL 1 GOTO :ERROR
cd ..\..\..
ECHO.

ECHO Processing inviwo-editor.pro...
cd src\qt\editor
qmake.exe -tp vc inviwo-editor.pro
IF ERRORLEVEL 1 GOTO :ERROR
cd ..\..\..
ECHO.

ECHO Processing glutminimum.pro...
cd apps\glutminimum
qmake.exe -tp vc glutminimum.pro
IF ERRORLEVEL 1 GOTO :ERROR
cd ..\..
ECHO.

ECHO Processing inviwo.pro...
cd apps\inviwo
qmake.exe -tp vc inviwo.pro
IF ERRORLEVEL 1 GOTO :ERROR
cd ..\..
ECHO.

ECHO Processing ticpp.pro...
cd ext\ticpp
qmake.exe -tp vc ticpp.pro
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
