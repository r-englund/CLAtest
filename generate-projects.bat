@ECHO OFF

REM Specify the used Visual Studio version
SET QMAKESPEC = win32-msvc2008
REM SET QMAKESPEC = win32-msvc2010

rem Generate Solution File if it does not exist.
rem Project Dependency Needs to be set before compilation.
IF NOT EXIST inviwo.sln (
echo Processing inviwo-projects.pro ...
qmake.exe -tp vc -r inviwo-projects.pro
IF ERRORLEVEL 1 GOTO :error
cd ..\..
echo.
echo Finished - Project Dependency Needs to be set in the solution file before compilation.
echo.
pause
GOTO :EOF
)

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

REM ECHO Processing glutminimum.pro...
REM cd apps\glutminimum
REM qmake.exe -tp vc glutminimum.pro
REM IF ERRORLEVEL 1 GOTO :ERROR
REM cd ..\..
REM ECHO.

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
