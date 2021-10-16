ldpath = $LD_LIBRARY_PATH

export LD_LIBRARY_PATH=`pwd`/install/unix_dynamic_release/lib:$ldpath
python cmake-git.py install all -arch unix


# python cmake-git.py install all -arch unix -debug


# python cmake-git.py install all -arch unix -static


# python cmake-git.py install all -arch unix -debug -static


export LD_LIBRARY_PATH=$ldpath
