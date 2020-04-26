@echo off
setlocal
set devenv="C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv"

if /i "%1" == "" goto all
if /i "%1" == "release" goto release
if /i "%1" == "debug" goto debug

:release
%devenv% %~dp0dom.sln /build "Release 1.4"
goto end

:debug
%devenv% %~dp0dom.sln /build "Debug 1.4"
goto end

:all
%devenv% %~dp0dom.sln /build "Release 1.4"
%devenv% %~dp0dom.sln /build "Debug 1.4"
goto end

:end
