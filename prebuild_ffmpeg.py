#!/usr/bin/python
# -*- coding: cp936 -*-

import os
import sys
import string
import subprocess
from common import *
import urllib

#内部路径 升级后可能会替换
ffmpeg_DIR_NAME = "ffmpeg-2.5.1"
x264_DIR_NAME = "x264-snapshot-20141218-2245"

standlone_toolshain_dir="../../_android-toolchain_"+ANDROID_ABI


def posixpath( str ):
    ret_str = ""
    for s in str :
        # print s
        if s == '\\':
            ret_str += '/'
        elif s == ':':
            pass
        else:
            ret_str += s
    return ret_str

def build_x264( str_target ):
    print "build x264"
    os.chdir( "./prebuild/" + x264_DIR_NAME )
    
    if( str_target == "vc"):
        str = "sh ./configure"
        str+= " --prefix=../../3rdparty/"
        # str += " --enable-shared "
        str += " --enable-static "
        # str += " --enable-pic "
        # str += " --disable-asm "
        # str += " --disable-cli "
        
        # 64bit
        # str += " --prefix=/mingw --enable-win32thread --host=x86_64-w64-mingw32"
        
        # print str
        my_exec( str )
        my_exec( "make install" )
        
    elif( str_target == "ndk"):
        # standlone_toolshain_dir="../_android-toolchain/"
        str = "sh ./configure"
        # str += " --prefix=$PREFIX "
        str += " --prefix=../../" + ANDROID_ABI + "/"
        str += " --enable-static "
        str += " --enable-pic "
        str += " --disable-asm "
        str += " --disable-cli "
        str += " --host=arm-linux"
        # str += " --cross-prefix=$PREBUILT/linux-x86/bin/arm-linux-androideabi- "
        # str += " --cross-prefix="+standlone_toolshain_dir+"bin/arm-linux-androideabi-"
        # str += " --sysroot=$PLATFORM"
        
        # print str
        my_exec( str )
        my_exec( "make install" )
    else:
        pass
        
    os.chdir("../../")

    
def build_ffmpeg( str_target ):
    print "build ffmpeg"
    os.chdir( "./prebuild/" + ffmpeg_DIR_NAME )
    
    cwd_str = os.getcwd();
    cwd_str = os.path.dirname(cwd_str)
    cwd_str = os.path.dirname(cwd_str)
    # cwd_str = urllib.pathname2url(cwd_str)
    cwd_str = posixpath( cwd_str )
    print cwd_str
    # return 
    
    if( str_target == "vc"):
        my_exec( str_target,"pwd" )
        str = "sh ./configure"
        str+= ' --disable-static --enable-shared'
        str+= ' --prefix=../../3rdparty/'
        
        str+= ' --enable-gpl --enable-libx264 '
        str+= ' --enable-encoder=libx264 '
        str+= ' --enable-decoder=h264 '
        
        str+= ' --extra-cflags=-I/usr/local/include --extra-ldflags=-L/usr/local/lib '
        # str+= ' --extra-cflags=-I'+cwd_str+'/3rdparty/include --extra-ldflags=-L'+cwd_str+'/3rdparty/lib'
        
        #64bit
        if( FFMPEG_ADDRESS_MODEL == '64'):
            str+= ' --arch=x86_64 --host-os=win64 --extra-cflags=-I/mingw/include --extra-ldflags=-L/mingw/lib'
        
        my_exec( str_target,str )
        my_exec( str_target,"make install" )
        
        os.system( 'mv -t ../../3rdparty/lib/ ../../3rdparty/bin/*.lib' )
        
    elif( str_target == "ndk"):
        
        str = "sh ./configure"
        str+= " --target-os=linux --arch=arm"
        str+= " --enable-cross-compile"
        # str+= " --cc=" +standlone_toolshain_dir+ "bin/arm-linux-androideabi-gcc"
        str+= " --cross-prefix="+standlone_toolshain_dir+"/bin/arm-linux-androideabi-"
        # str+= ' --extra-cflags="-fPIC -DANDROID"'
        # str+= ' --extra-cflags="-DANDROID "'
        # str+= ' --extra-cflags="-std=gnu++0x "'
        str+= " --prefix=../../" + ANDROID_ABI + "/"
        str+= ' --disable-ffmpeg'
        str+= ' --disable-stripping'
        str+= ' --disable-ffplay'
        str+= ' --disable-ffserver'
        str+= ' --disable-ffprobe'
        # str+= ' --disable-network'
        # str+= ' --enable-armv5te'
        str+= ' --enable-static'
        str+= ' --disable-shared'
        # str+= ' --disable-static'
        # str+= ' --enable-shared'
        # str+= ' --disable-decoder=ffv1 '
        # str+= ' --disable-decoder=imc '
        str+= ' --enable-gpl --enable-libx264'
        str+= ' --enable-encoder=libx264 '
        str+= ' --enable-decoder=h264 '
        str+= ' --extra-cflags=-I' + cwd_str + '/' + ANDROID_ABI + '/include --extra-ldflags=-L'+ cwd_str + '/' + ANDROID_ABI + '/lib'
        # str+= ' --extra-ldflags="-Wl,-T,'
        # str+= standlone_toolshain_dir+'/arm-linux-androideabi/lib/ldscripts/armelf_linux_eabi.x -Wl,-rpath-link='
        # str+= standlone_toolshain_dir+'/sysroot/usr/lib -L'
        # str+= standlone_toolshain_dir+'/sysroot/usr/lib -nostdlib '
        # str+= standlone_toolshain_dir+'/lib/gcc/arm-linux-androideabi/4.8/crtbegin.o '
        # str+= standlone_toolshain_dir+'/lib/gcc/arm-linux-androideabi/4.8/crtend.o -lc -lm -ldl"'
        # str+= ' -v'
        
        # print str
        my_exec( str_target,str )
        my_exec( str_target,"make install" )
    else:
        pass
        
    os.chdir("../../")
    
    
    
#main    
def main():

    ALL_TARGET = "vc" #默认vc
    if len(sys.argv) >= 2:
        ALL_TARGET = sys.argv[1]
    print( ALL_TARGET )


    if ALL_TARGET != "vc":
        #在vc下设置x264的路径总是不成功 被迫使用安装路径
        #在ndk下 可以直接使用自定义路径
        build_x264(ALL_TARGET)
    build_ffmpeg(ALL_TARGET)
    
    
main()
