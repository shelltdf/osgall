@echo off
set st=%time%
echo %time%

REM python cmake-git.py install all -arch em
REM python cmake-git.py install all -arch em -debug

python cmake-git.py install all -arch em -static
REM python cmake-git.py install all -arch em -debug -static

set et=%time%
echo %time%

echo %st% %et%
