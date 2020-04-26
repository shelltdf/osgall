#!/usr/bin/python
# -*- coding: cp936 -*-

import os
import sys
import string
import subprocess


# TARGET
# ALL_TARGET = "vc"     Visual C++
# ALL_TARGET = "mingw"  mingw
# ALL_TARGET = "ndk"    android ndk
# ALL_TARGET = "nc"     ncal
# ALL_TARGET = "cw"     nvidia android codeworks

#version
CPP_VERSION = 11
OSG_VERSION = 340
OE_VERSION = 280

# release debug
BUILD_VC_DEBUG = True
BUILD_VC_RELEASE = True
BUILD_DYNAMIC_LINK_LIBRARY = True
CMAKE_BUILD_TYPE = "Debug" # Release Debug



#common tool
PYTHON_EXE = r'"C:/Python27/python.exe"'
CMAKE_EXE = r'cmake.exe'
CMAKE_VC_PLATFORM = r'"Visual Studio 15 2017 Win64"' # cmake -G "Visual Studio 11 2012 Win64"
USE_OS_SYSTEM = True # mingw os true


#common setting
USE_ANGLE = False
USE_NACL = False
GL_VER = 0 # 0-AUTO 1-GL1 2-GL2 3-GL3
GLES_VER = 0 # 0-GL 1-GLES1 2-GLES2
BOOST_ARCH = 'x86' # -march setting
BOOST_ADDRESS_MODEL = '64'  # 32 64
FFMPEG_ADDRESS_MODEL = '64' # 32 64
PTHREADS_BUILD_PROFILE = '-DPTHREADS_BUILD_CPP' # PTHREADS_BUILD_CPP PTHREADS_BUILD_SEH


#external library
FBX_ON = False
FBX_DIR = r'D:/Program Files/Autodesk/FBX/FBX SDK/2016.1.2'   # fbx install dir
FBX_LIB_PROFILE = r'vs2015\x86' # vs2013\x86 vs2013\x64
QuickTimeSDK_ON  = False
QuickTimeSDK_DIR = r'D:/QuickTime SDK'   #quicktime sdk install dir
Qt_ON = False
Qt_DIR = r'D:\Qt\Qt5.10.1\5.10.1\msvc2015_64\lib\cmake\Qt5' #lib/cmake/Qt5/


#android ndk setting
ANDROID_NDK_PATH = "D:/Android/sdk/ndk-bundle"    #ndk dir
ANDROID_NDK_HOST = "windows-x86_64"               #host
ANDROID_API_LEVEL = 23              #9 15 21 android6-23
ANDROID_ABI = "arm64-v8a" # armeabi-v7a arm64-v8a
ANDROID_ARCH = "arm"                            #make_ndk_standalone
ANDROID_toolchain="arm-linux-androideabi-4.9"   #make_ndk_standalone
CPU_NUM = 1



# ncal setting
NACL_ROOT = "d:/nacl_sdk" # nacl_sdk dir
NACL_PEPPER_DIR = "pepper_49"  # nacl_sdk/pepper_xx dir
NACL_TOOLCHAIN = "win_pnacl"   # nacl_sdk/pepper_xx/toolchain/xxxx
NACL_INSTALL_DIR = "nacl"
NACL_HOST = "win_x86_64_host"
# NACL_GLES_LIB = NACL_ROOT + '/' + NACL_PEPPER_DIR + '/lib/'+NACL_HOST+'/Release/ppapi_gles2.lib'
NACL_GLES_LIB = NACL_ROOT + '/' + NACL_PEPPER_DIR + '/lib/'+NACL_HOST+'/Debug/ppapi_gles2.lib'


# vc ppapi模式  vc本地arch 无窗口 gles2
VC_PPAPI_MODE = False


# 
CWD = ""

def my_exec( str_target, cmake_str ):
    print ("exec - "  + cmake_str)
    
    if(USE_OS_SYSTEM):
        os.system(cmake_str)
    else:
        ps = subprocess.Popen(cmake_str)
        ps.wait()
    

def my_make_build_dir( str_lib_dir_name , str_complior_name ):
    global CWD
    CWD = os.getcwd();
    print("CWD = " + CWD)
    os.chdir( "./prebuild/" + str_lib_dir_name )
    os.system( "mkdir build_" + str_complior_name )
    os.chdir( "build_" + str_complior_name)

def my_configure( str_target , str_config ,is_osg = False , over_debug_postfix = True ,is_opencv = False):

    cmake_string = CMAKE_EXE
    
    global CWD
    
    if over_debug_postfix :
        cmake_string += r' -DCMAKE_DEBUG_POSTFIX="d"'
    
    if str_target == "vc" :
        cmake_string += ' -G ' + CMAKE_VC_PLATFORM
        cmake_string += ' -DCMAKE_INSTALL_PREFIX="' +CWD+ '/3rdparty/" '
        
        # cmake_string += ' -DCMAKE_CXX_FLAGS_RELEASE="${CMAKE_CXX_FLAGS_RELEASE} /MT"'
        # cmake_string += ' -DCMAKE_CXX_FLAGS_DEBUG="${CMAKE_CXX_FLAGS_DEBUG} /MTd"'
        
    if str_target == "mingw" :
        cmake_string += ' -G "MSYS Makefiles"  -DCMAKE_INSTALL_PREFIX="' +CWD+ '/3rdparty/" '
    if str_target == "cw" :
        cmake_string += ' -G ' + CMAKE_VC_PLATFORM
        cmake_string += ' -DCMAKE_TOOLCHAIN_FILE=../../../android.toolchain.cmake'
    if str_target == "ndk" :
        if is_opencv :
            cmake_string += ' -DCMAKE_TOOLCHAIN_FILE='+os.getcwd()+'\\..\\platforms\\android\\android.toolchain.cmake -G "MSYS Makefiles"'
            cmake_string += ' -DANDROID_TOOLCHAIN_NAME=' + ANDROID_toolchain
        else:
            cmake_string += ' -DCMAKE_TOOLCHAIN_FILE=../../../android.toolchain.cmake -G "MSYS Makefiles"'
    if str_target == "ndk" or str_target == "cw" :
        cmake_string += ' -DANDROID_ABI=' + ANDROID_ABI
        if is_osg :
            cmake_string += ' -DCMAKE_INSTALL_PREFIX="' +CWD+ '/' + ANDROID_ABI + '"'
        else:
            cmake_string += ' -DCMAKE_INSTALL_PREFIX="' +CWD+ '/' + ANDROID_ABI + '"'
        cmake_string += ' -DJ=' + str(CPU_NUM)
        cmake_string += ' -DANDROID_NATIVE_API_LEVEL=' + str(ANDROID_API_LEVEL)
        # cmake_string += " -DCMAKE_ANDROID_API="+str(ANDROID_API_LEVEL)
        # cmake_string += " -DCMAKE_ANDROID_API_MIN="+str(ANDROID_API_LEVEL)
        cmake_string += ' -DANDROID_NDK=' + ANDROID_NDK_PATH
        
        if(BUILD_DYNAMIC_LINK_LIBRARY):
            cmake_string += ' -DANDROID_STL=gnustl_shared '
        else:
            cmake_string += ' -DANDROID_STL=gnustl_static '
        
    if str_target == "nc" :
        cmake_string += ' -G "Unix Makefiles"'
        cmake_string += ' -DCMAKE_TOOLCHAIN_FILE=../../../pnacl.toolchain.cmake'
        cmake_string += ' -DCMAKE_INSTALL_PREFIX="' +CWD+ '/'+NACL_INSTALL_DIR+'/" '
        cmake_string += ' -DNACL_ROOT=' + NACL_ROOT
        cmake_string += ' -DNACL_PEPPER_DIR=' + NACL_PEPPER_DIR
        cmake_string += ' -DNACL_TOOLCHAIN=' + NACL_TOOLCHAIN
        cmake_string += ' -DCMAKE_OSX_ARCHITECTURES="nacl"'
        
        # cmake_string += ' -DCMAKE_C_USE_RESPONSE_FILE_FOR_LIBRARIES=1'
        # cmake_string += ' -DCMAKE_CXX_USE_RESPONSE_FILE_FOR_LIBRARIES=1'
        # cmake_string += ' -DCMAKE_C_USE_RESPONSE_FILE_FOR_OBJECTS=1'
        # cmake_string += ' -DCMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS=1'
        # cmake_string += ' -DCMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES=1'
        # cmake_string += ' -DCMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES=1'
        # cmake_string += ' -DCMAKE_C_RESPONSE_FILE_LINK_FLAG="@"'
        # cmake_string += ' -DCMAKE_CXX_RESPONSE_FILE_LINK_FLAG="@"'
        
    
    if str_target != "vc" :
        cmake_string += ' -DCMAKE_BUILD_TYPE=' + CMAKE_BUILD_TYPE
    
    cmake_string += ' ' + str_config+ ' ..'
    
    my_exec( str_target, cmake_string )
    

def my_build( str_target , only_release = False):
    if(str_target == "vc" or str_target == "cw" ):
        if( not only_release ):
            if(BUILD_VC_DEBUG):
                os.system('msbuild INSTALL.vcxproj /p:Configuration=Debug')
                pass
        if(BUILD_VC_RELEASE):
            os.system('msbuild INSTALL.vcxproj /p:Configuration=Release')
    # elif str_target == "cw" :
        # pass
    elif(str_target == "ndk"):
        # os.system(CMAKE_EXE + " --build .")
        os.system("make install")
    else:
        os.system("make install")
    # os.chdir( "../../..")
    
    global CWD
    print("CWD = " + CWD)
    os.chdir(CWD)
    