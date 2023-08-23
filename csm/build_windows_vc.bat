@echo off
set st=%time%
echo %time%

python cmake-git.py install all -arch vs2019-64
python cmake-git.py install all -arch vs2019-64 -debug
REM python cmake-git.py install all -arch vs2019-64 -debuginfo

python cmake-git.py install all -arch vs2019-64 -static
python cmake-git.py install all -arch vs2019-64 -debug -static
REM python cmake-git.py install all -arch vs2019-64 -debuginfo -static

set et=%time%
echo %time%

echo %st% %et%
