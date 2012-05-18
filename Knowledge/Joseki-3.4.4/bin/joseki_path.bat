@echo off
@REM Script to build the Joseki classpath
@REM $Id: joseki_path.bat,v 1.5 2004/01/09 18:21:34 andy_seaborne Exp $

@REM Must be windows format \ not /
SET JOSEKIROOT=.\

if "%JOSEKIROOT%" == ""  goto noRoot

set CP=""
REM Do this to put the developement .class files first
REM NB no space before the ")"
if EXIST %JOSEKIROOT%\classes (
  if "%CP%" == "" (set CP=%JOSEKIROOT%\classes) ELSE (set CP=%CP%;%JOSEKIROOT%\classes)
)

pushd %JOSEKIROOT%
for %%f in (lib\*.jar) do call :oneStep %%f
popd

:oneStep
if "%CP%" == "" (set CP=%JOSEKIROOT%\%1) ELSE (set CP=%CP%;%JOSEKIROOT%\%1)
exit /B

:noRoot
echo Environment variable JOSEKIROOT needs to be set

:noMore

