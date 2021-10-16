@echo off
set st=%time%
echo %time%

REM python cmake-git.py install all -arch ninja
REM python cmake-git.py install all -arch ninja -debug

REM python cmake-git.py install all -arch ninja -static
REM python cmake-git.py install all -arch ninja -debug -static

set et=%time%
echo %time%

echo %st% %et%
