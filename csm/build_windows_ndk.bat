@echo off
set st=%time%
echo %time%

REM python cmake-git.py install all -arch ndk
REM python cmake-git.py install all -arch ndk -debug

python cmake-git.py install all -arch ndk -static
python cmake-git.py install all -arch ndk -debug -static

set et=%time%
echo %time%

echo %st% %et%
