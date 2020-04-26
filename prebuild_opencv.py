#!/usr/bin/python
# -*- coding: cp936 -*-

import os
import sys
import string
import subprocess
from common import *


#内部路径 升级后可能会替换
CV_DIR_NAME = "opencv-2.4.13"


#    
#库建造定义
#

    
def build_cv( str_target ):
    print "build opencv"
    my_make_build_dir( CV_DIR_NAME , str_target )
    if(str_target == "ndk"):
        str  = ' -DWITH_CUDA=OFF'
        str  += ' -DWITH_FFMPEG=OFF'
        str  += " -DBUILD_EXAMPLES=OFF"
        str  += " -DBUILD_TESTS=OFF"
        str  += " -DBUILD_PERF_TESTS=OFF"
        str  += " -DBUILD_DOCS=OFF"
        # str  += " -DBUILD_ANDROID_EXAMPLE=OFF"
        str  += " -DBUILD_opencv_java=OFF"
        my_configure( str_target , str ,False,True,True)
    else:
        str  = ' -DWITH_CUDA=OFF'
        str  += ' -DWITH_FFMPEG=ON'
        str  += " -DBUILD_EXAMPLES=OFF"
        str  += " -DBUILD_PERF_TESTS=OFF"
        str  += " -DBUILD_TESTS=OFF"
        str  += " -DENABLE_PRECOMPILED_HEADERS=OFF"
        str  += " -DBUILD_WITH_STATIC_CRT=OFF"
        my_configure( str_target , str)
        
    # print str
    my_build(str_target)
    
    
#main    
def main():

    ALL_TARGET = "vc" #默认vc
    if len(sys.argv) >= 2:
        ALL_TARGET = sys.argv[1]
    print( ALL_TARGET )

    build_cv( ALL_TARGET )

main()
