@echo off
set st=%time%
echo %time%

python cmake-git.py install all -arch ninja
python cmake-git.py install all -arch ninja -debug

python cmake-git.py install all -arch ninja -static
python cmake-git.py install all -arch ninja -debug -static

set et=%time%
echo %time%

echo %st% %et%
