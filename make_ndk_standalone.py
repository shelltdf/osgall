#!/usr/bin/python
# -*- coding: cp936 -*-

import os
import sys
import string
import subprocess
from common import *


#    
#库建造定义
#
def make_ndk_standalone( arch , level ,toolchain ):
    str_cmd = "sh "
    str_cmd += os.environ["ANDROID_NDK"]
    # str_cmd += ANDROID_NDK_PATH
    str_cmd += '/build/tools/make-standalone-toolchain.sh'
    str_cmd += ' --platform=android-' + str(level)
    # str_cmd += ' --arch=' + arch
    # str_cmd += ' --toolchain=arm-linux-androideabi-4.9'
    str_cmd += ' --toolchain=' + toolchain
    str_cmd += ' --system='+ANDROID_NDK_HOST
    # str_cmd += ' --stl=gunstl' #gnustl libcxx libc++ stlport
    str_cmd += ' --install-dir=./_android-toolchain_' + ANDROID_ABI
    print str_cmd
    my_exec( str_cmd )

#main
def main():

    # ALL_arch = "arm" #默认vc
    # ALL_platform_version = "19" #默认vc
    
    # if len(sys.argv) >= 3:
        # ALL_arch = sys.argv[1]
        # ALL_platform_version = sys.argv[1]
        
    print( ANDROID_ARCH , '-' , ANDROID_API_LEVEL , '-' , ANDROID_toolchain)
    
    os.environ["ANDROID_NDK"] = ANDROID_NDK_PATH
    make_ndk_standalone( ANDROID_ARCH , ANDROID_API_LEVEL ,ANDROID_toolchain)

main()
