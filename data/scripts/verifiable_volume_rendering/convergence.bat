echo off
REM Enable use of exclamation marks
setlocal EnableDelayedExpansion

set test=dataset_convergence
set imagePath=..\..\images\vvr\%test%
call:runConvergence %imagePath%, %test%

set test=stepsize_convergence
set imagePath=..\..\images\vvr\%test%
call:runConvergence %imagePath%, %test%

set test=pixel_convergence
set imagePath=..\..\images\vvr\%test%
call:runConvergence %imagePath%, %test%

GOTO:EOF

:runConvergence 
echo Running convergence for images in %~1
set images=
for /f %%a in ('dir /b %~1\*.*') do set images=!images! %~1/%%a
python convergence.py -p -o %~2 %images%
GOTO:EOF