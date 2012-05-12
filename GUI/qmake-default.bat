@echo off
rem Uncomment and edit, if qt is not in your path
rem set path=C:\Qt\2010.05-vc\qt\bin

rem Select your Visual Studio version
set QMAKESPEC=win32-msvc2008
rem set QMAKESPEC=win32-msvc2010

rem Generate Solution File if it does not exist.
rem Project Dependency Needs to be set before compilation.
IF NOT EXIST voreen.sln (
echo Processing voreen.pro ...
qmake.exe -tp vc -r voreen.pro
IF ERRORLEVEL 1 GOTO :error
cd ..\..
echo.
echo Finished - Project Dependency Needs to be set in the solution file before compilation.
echo.
pause
GOTO :EOF
)

rem Generate applications?
set GENERATE_APPS=1
set GENERATE_TOOLS=0
set GENERATE_SIMPLE_APPS=0
set GENERATE_TEST_APPS=0


echo Processing tgt.pro ...
cd ext\tgt
qmake.exe -tp vc "CONFIG+=flat" tgt.pro
IF ERRORLEVEL 1 GOTO :error
cd ..\..
echo.

echo Processing voreenlib_core.pro ...
cd src\core
qmake.exe -tp vc "CONFIG+=flat" voreenlib_core.pro
IF ERRORLEVEL 1 GOTO :error
cd ..\..
echo.

echo Processing voreenlib_qt.pro ...
cd src\qt
qmake.exe -tp vc "CONFIG+=flat" voreenlib_qt.pro
IF ERRORLEVEL 1 GOTO :error
cd ..\..
echo.

echo Applications:
IF %GENERATE_APPS%==1 (

    echo     Processing voreenve.pro ...
    cd apps\voreenve
    qmake.exe -tp vc "CONFIG+=flat" voreenve.pro
    IF ERRORLEVEL 1 GOTO :error
    cd ..\..

    echo     Processing voltool.pro ...
    cd apps\voltool
    qmake.exe -tp vc "CONFIG+=flat" voltool.pro
    IF ERRORLEVEL 1 GOTO :error
    cd ..\..
    
) ELSE echo     Skipped.
echo.

echo Tools:
IF %GENERATE_TOOLS%==1 (

    echo     Processing newfile.pro ...
    cd tools\newfile
    qmake.exe -tp vc "CONFIG+=flat" newfile.pro
    IF ERRORLEVEL 1 GOTO :error
    cd ..\..

    echo     Processing itk_wrapper.pro ...
    cd apps\itk_wrapper
    qmake.exe -tp vc "CONFIG+=flat" itk_wrapper.pro
    IF ERRORLEVEL 1 GOTO :error
    cd ..\..    
   
) ELSE echo     Skipped.
echo.

echo Simple Applications:
IF %GENERATE_SIMPLE_APPS%==1 (

    echo     Processing simple-qt.pro ...
    cd apps\simple
    qmake.exe -tp vc "CONFIG+=flat" simple-qt.pro
    IF ERRORLEVEL 1 GOTO :error
    cd ..\..

    echo     Processing simple-glut.pro ...
    cd apps\simple
    qmake.exe -tp vc "CONFIG+=flat" simple-glut.pro
    IF ERRORLEVEL 1 GOTO :error
    cd ..\..

) ELSE echo     Skipped.
echo.

echo Test Applications:
IF %GENERATE_TEST_APPS%==1 (
    
    echo     Processing serializertest.pro ...
    cd apps\tests\serializertest
    qmake.exe -tp vc "CONFIG+=flat" serializertest.pro
    IF ERRORLEVEL 1 GOTO :error
    cd ..\..\..

    echo     Processing simple-memcheck.pro ...
    cd apps\tests
    qmake.exe -tp vc "CONFIG+=flat" simple-memcheck.pro
    IF ERRORLEVEL 1 GOTO :error
    cd ..\..

    echo     Processing simple-memcheckinit.pro ...
    cd apps\tests
    qmake.exe -tp vc "CONFIG+=flat" simple-memcheckinit.pro
    IF ERRORLEVEL 1 GOTO :error
    cd ..\..

    echo     Processing coveragetest.pro ...
    cd apps\tests\coveragetest
    qmake.exe -tp vc "CONFIG+=flat" coveragetest.pro
    IF ERRORLEVEL 1 GOTO :error
    cd ..\..\..

    echo     Processing descriptiontest.pro ...
    cd apps\tests\descriptiontest
    qmake.exe -tp vc "CONFIG+=flat" descriptiontest.pro
    IF ERRORLEVEL 1 GOTO :error
    cd ..\..\..

    echo     Processing voreenblastest.pro ...
    cd apps\tests\voreenblastest
    qmake.exe -tp vc "CONFIG+=flat" voreenblastest.pro
    IF ERRORLEVEL 1 GOTO :error
    cd ..\..\..
    
) ELSE echo     Skipped. 
echo.

echo Finished.
echo.
pause
GOTO :EOF

:error
echo.
echo FAILED!
echo.
pause
