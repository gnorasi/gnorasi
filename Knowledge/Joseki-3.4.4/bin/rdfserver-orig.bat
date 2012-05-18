@echo off
@REM $Id: rdfserver.bat,v 1.4 2004/04/13 15:49:23 andy_seaborne Exp $

if not "%JOSEKIROOT%" == "" goto :ok
echo Environment variable JOSEKIROOT not set
goto :theEnd

:ok
call bin\joseki_path.bat

REM set LOGCONFIG=file:%JOSEKIROOT%\etc\log4j-detail.properties
set LOGCONFIG=file:%JOSEKIROOT%\etc\log4j.properties
set LOG=-Dlog4j.configuration=%LOGCONFIG%

set JAVA=java.exe

:endJavaHome

%JAVA% -cp %CP% %LOG% joseki.rdfserver %1 %2 %3 %4 %5 %6 %7 %8 %9

:theEnd
