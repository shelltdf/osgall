#!/usr/bin/python
# -*- coding: cp936 -*-

import os
import sys
import string
import subprocess
from common import *
import urllib

#内部路径 升级后可能会替换
# boost_DIR_NAME = "boost_1_60_0"
# boost_DIR_NAME = "boost_1_66_0"
boost_DIR_NAME = "boost_1_71_0"


def build_boost( str_target ):
    print ("build boost")
    os.chdir( "./prebuild/" + boost_DIR_NAME )
    
    if( str_target == "vc"):
    
        my_exec( str_target,"bootstrap.bat" )
        
        str = 'b2 --toolset=msvc install debug release threading=multi runtime-link=shared'
        str += ' architecture='+BOOST_ARCH+' address-model='+BOOST_ADDRESS_MODEL
        # str += ' --stagedir=../../3rdparty --includedir=../../3rdparty/include'
        str += ' --prefix=../../3rdparty/'
        str += ' --layout=tagged'
        str += ' --without-python'
        if( BUILD_DYNAMIC_LINK_LIBRARY ):
            str += ' link=shared'
        else:
            str += ' link=static'
        my_exec( str_target, str )

        #move dll to bin
        os.chdir( "../../3rdparty/" )
        os.system( "mkdir bin" )
        os.chdir( "../prebuild/" + boost_DIR_NAME )
        os.system( 'move /y ../../3rdparty/lib/*.dll ../../3rdparty/bin/' )
        
    elif( str_target == "ndk"):
    
        # os.system( "bootstrap.sh" )
        # my_exec( "bootstrap.bat" )
        
        str = 'bjam --toolset=gcc target-os=linux --stagedir=android'
        # str = 'bjam --toolset=gcc-arm install debug release threading=multi runtime-link=static'
        # str += ' link=static'
        # str += ' target-os=linux --stagedir=android'
        my_exec( str )
        
        pass
    else:
        pass
        
    os.chdir("../../")

    
#main    
def main():

    ALL_TARGET = "vc" #默认vc
    if len(sys.argv) >= 2:
        ALL_TARGET = sys.argv[1]
    print( ALL_TARGET )

    
    build_boost(ALL_TARGET)
    
    
main()
