@echo off
@REM $Id: rdfserver.bat,v 1.4 2004/04/13 15:49:23 andy_seaborne Exp $

rem if not "%JOSEKIROOT%" == "" goto :ok
rem echo Environment variable JOSEKIROOT not set
rem goto :theEnd

:ok
call bin\joseki_path.bat

REM set LOGCONFIG=file:%JOSEKIROOT%\etc\log4j-detail.properties
set LOGCONFIG=file:%JOSEKIROOT%\etc\log4j.properties
set LOG=-Dlog4j.configuration=%LOGCONFIG%
set MAXFORMPARAMS=-Dorg.mortbay.jetty.Request.maxFormContentSize=6000000

set JAVA=java.exe

:endJavaHome

%JAVA% -cp %CP% %LOG% %MAXFORMPARAMS% -Xms512m -Xmx1g -Xss8m joseki.rdfserver %1 %2 %3 %4 %5 %6 %7 %8 %9

:theEnd
