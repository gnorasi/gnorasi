@echo off
rem Uncomment, if qt is not in your path
rem set path=c:\qt\4.4.3\bin

rem Select your Visual Studio version
rem set QMAKESPEC=win32-msvc.net
rem set QMAKESPEC=win32-msvc2005
set QMAKESPEC=win32-msvc2008

qmake.exe -tp vc -r
cd tools/additional
call "project-deps.py"
cd ../..

cd tools/newfile
qmake.exe -tp vc -r
cd ../..

call "tools/additional/nomanifest.py" ext/tgt/tgt.vcproj src/core/voreen_core.vcproj src/qt/voreen_qt.vcproj
