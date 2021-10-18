@echo off
set st=%time%
echo %time%

python cmake-git.py install all -arch ndk
REM python cmake-git.py install all -arch ndk -debug

REM python cmake-git.py install all -arch ndk -static
REM python cmake-git.py install all -arch ndk -debug -static

set et=%time%
echo %time%

echo %st% %et%
