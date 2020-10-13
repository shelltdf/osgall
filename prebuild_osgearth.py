#!/usr/bin/python
# -*- coding: cp936 -*-

import os
import sys
import string
import subprocess
from common import *


#内部路径 升级后可能会替换
# osgearth_DIR_NAME = "gwaldron-osgearth-25ce0e1"   #github 2.7
# osgearth_DIR_NAME = "osgearth28"   #github 2.8
# osgearth_DIR_NAME = "osgearth-master-20180304"   #github 2.10
osgearth_DIR_NAME = "osgearth-2.10.1"   #osgearth-2.10.1


#    
#库建造定义
#

def build_osgearth( str_target ):
    print ("build osgearth")
    my_make_build_dir( osgearth_DIR_NAME , str_target )
    
    #删除源码中 gwen 的定义 -D_STATIC_CPPLIB
    
    str_ops = ""

    
    # str_ops += " -DINSTALL_EXTRA_LIBS=ON -DINSTALL_LIBS=ON";
    # str_ops += " -DBUILD_SHARED_LIBS=0 ";   #无法正常编译dll 只能静态
    
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops += ' -DDYNAMIC_OSGEARTH=1'
    else:
        str_ops += ' -DDYNAMIC_OSGEARTH=0'
        
    if(Qt_ON):
        str_ops += " -DQt5_DIR=" + Qt_DIR + "/../Qt5"
        str_ops += " -DQt5Widgets_DIR=" + Qt_DIR + "/../Qt5Widgets"
        str_ops += " -DQt5OpenGL_DIR=" + Qt_DIR + "/../Qt5OpenGL"
        str_ops += " -DBUILD_OPENTHREADS_WITH_QT=1"
        str_ops += " -DOSGEARTH_USE_QT=1"
        str_ops += " -DOSGEARTH_QT_BUILD=1"
    else:
        str_ops += " -DBUILD_OPENTHREADS_WITH_QT=0"
        str_ops += " -DOSGEARTH_USE_QT=0"
        str_ops += " -DOSGEARTH_QT_BUILD=0"
        
    if( str_target == "vc"):
        str_ops += " -DWIN32_USE_MP=1";
        str_ops += " -DBUILD_OSGEARTH_EXAMPLES=1";
        
        str_ops += ' -DSQLITE3_LIBRARY='+ os.getcwd() +'/../../../3rdparty/lib/sqlite.lib'
        str_ops += ' -DSQLITE3_LIBRARY_DEBUG='+ os.getcwd() +'/../../../3rdparty/lib/sqlited.lib'
        
        # str_ops += " -DGDAL_DIR=../../../3rdparty/include/gdal ";
        str_ops += ' -DGDAL_INCLUDE_DIR='+ os.getcwd() +'/../../../3rdparty/include/gdal'
        str_ops += ' -DGDAL_LIBRARY='+ os.getcwd() +'/../../../3rdparty/lib/gdal31.lib'
        str_ops += ' -DGDAL_LIBRARY_DEBUG='+ os.getcwd() +'/../../../3rdparty/lib/gdal31d.lib'
        
        str_ops += ' -DCURL_DIR='+ os.getcwd() +'/../../../3rdparty/include/curl'
        str_ops += ' -DCURL_LIBRARY='+ os.getcwd() +'/../../../3rdparty/lib/libcurl_imp.lib'
        str_ops += ' -DCURL_LIBRARY_DEBUG='+ os.getcwd() +'/../../../3rdparty/lib/libcurld_imp.lib'

        str_ops += ' -DZLIB_INCLUDE_DIR='+ os.getcwd() +'"/../../../3rdparty/include"'
        if( BUILD_DYNAMIC_LINK_LIBRARY ):
            str_ops += ' -DZLIB_LIBRARY='+ os.getcwd() +'"/../../../3rdparty/lib/zlib.lib"'
            str_ops += ' -DZLIB_LIBRARY_DEBUG='+ os.getcwd() +'"/../../../3rdparty/lib/zlibd.lib"';
            # str_ops += ' -DZLIB_LIBRARY_RELEASE=""';
        else:
            str_ops += ' -DZLIB_LIBRARY='+ os.getcwd() +'"/../../../3rdparty/lib/zlibstatic.lib"'
            str_ops += ' -DZLIB_LIBRARY_DEBUG='+ os.getcwd() +'"/../../../3rdparty/lib/zlibstaticd.lib"';

        # str_ops += ' -DMINIZIP_INCLUDE_DIR='+ os.getcwd() +'/../../../3rdparty/include'
        
        if(VC_PPAPI_MODE):
            # str_ops += " -DBUILD_OSG_APPLICATIONS=0";
            # str_ops += " -DBUILD_OSG_EXAMPLES=0"
            # str_ops += " -DOPENGL_PROFILE=\"GLES2\""
            # str_ops += " -DOSG_GL_LIBRARY_STATIC=ON"
            # str_ops += ' -DOSG_WINDOWING_SYSTEM="none"'
            str_ops += ' -DOPENGL_INCLUDE_DIR:PATH="' + NACL_ROOT +'/'+ NACL_PEPPER_DIR+'/include"'
            # str_ops += ' -DOPENGL_gl_LIBRARY=' + NACL_ROOT +'/'+ NACL_PEPPER_DIR+'/lib/pnacl/Release/libppapi_gles2.a'
            str_ops += ' -DOPENGL_gl_LIBRARY=' + NACL_GLES_LIB 
            
            
    if(str_target == "vc" and (not BUILD_DYNAMIC_LINK_LIBRARY)):
        str_ops += ' -DOPENTHREADS_LIBRARY=../../../3rdparty/lib/ot20-OpenThreads.lib'
        str_ops += ' -DOSG_LIBRARY=../../../3rdparty/lib/osg130-osg.lib'
        str_ops += ' -DOSGFX_LIBRARY=../../../3rdparty/lib/osg130-osgFX.lib'
        str_ops += ' -DOSGGA_LIBRARY=../../../3rdparty/lib/osg130-osgGA.lib'
        str_ops += ' -DOSGTEXT_LIBRARY=../../../3rdparty/lib/osg130-osgText.lib'
        str_ops += ' -DOSGDB_LIBRARY=../../../3rdparty/lib/osg130-osgDB.lib'
        str_ops += ' -DOSGVIEWER_LIBRARY=../../../3rdparty/lib/osg130-osgViewer.lib'
        str_ops += ' -DOSGSIM_LIBRARY=../../../3rdparty/lib/osg130-osgSim.lib'
        str_ops += ' -DOSGUTIL_LIBRARY=../../../3rdparty/lib/osg130-osgUtil.lib'
        str_ops += ' -DOSGSHADOW_LIBRARY=../../../3rdparty/lib/osg130-osgShadow.lib'
        str_ops += ' -DOSGMANIPULATOR_LIBRARY=../../../3rdparty/lib/osg130-osgManipulator.lib'
        str_ops += ' -DOSGTERRAIN_LIBRARY=../../../3rdparty/lib/osg130-osgTerrain.lib'
        str_ops += ' -DOSGWIDGET_LIBRARY=../../../3rdparty/lib/osg130-osgWidget.lib'
    
        str_ops += " -DCURL_LIBRARY=../../../3rdparty/lib/libcurl.lib"
        str_ops += " -DCURL_LIBRARY_DEBUG=../../../3rdparty/lib/libcurld.lib"
        
        str_ops += " -DCRUNCH_INCLUDE_DIR=../../../3rdparty/include/"
        str_ops += " -DCRUNCH_LIBRARY=../../../3rdparty/lib/crnlib.lib"
        str_ops += " -DCRUNCH_LIBRARY_DEBUG=../../../3rdparty/lib/crnlibd.lib"
        
        
    if( str_target == "nc"):
    
        str_ops += " -DDYNAMIC_OSGEARTH=0";
        
        str_ops += ' -DOSG_DIR='+ os.getcwd() +'/../../../'+NACL_INSTALL_DIR
        
        str_ops += ' -DOSG_GEN_INCLUDE_DIR='+ os.getcwd() +'"/../../../'+NACL_INSTALL_DIR+'/include"'
        str_ops += ' -DOSG_INCLUDE_DIR='+ os.getcwd() +'"/../../../'+NACL_INSTALL_DIR+'/include"'
        
        str_ops += ' -DOPENTHREADS_LIBRARY='+ os.getcwd() +'/../../../'+NACL_INSTALL_DIR+'/lib/libOpenThreads.a'
        str_ops += ' -DOSG_LIBRARY='+ os.getcwd() +'/../../../'+NACL_INSTALL_DIR+'/lib/libosg.a'
        str_ops += ' -DOSGWIDGET_LIBRARY='+ os.getcwd() +'/../../../'+NACL_INSTALL_DIR+'/lib/libosgWidget.a'
        str_ops += ' -DOSGVIEWER_LIBRARY='+ os.getcwd() +'/../../../'+NACL_INSTALL_DIR+'/lib/libosgViewer.a'
        str_ops += ' -DOSGUTIL_LIBRARY='+ os.getcwd() +'/../../../'+NACL_INSTALL_DIR+'/lib/libosgUtil.a'
        str_ops += ' -DOSGTEXT_LIBRARY='+ os.getcwd() +'/../../../'+NACL_INSTALL_DIR+'/lib/libosgText.a'
        str_ops += ' -DOSGTERRAIN_LIBRARY='+ os.getcwd() +'/../../../'+NACL_INSTALL_DIR+'/lib/libosgTerrain.a'
        str_ops += ' -DOSGSIM_LIBRARY='+ os.getcwd() +'/../../../'+NACL_INSTALL_DIR+'/lib/libosgSim.a'
        str_ops += ' -DOSGSHADOW_LIBRARY='+ os.getcwd() +'/../../../'+NACL_INSTALL_DIR+'/lib/libosgShadow.a'
        str_ops += ' -DOSGMANIPULATOR_LIBRARY='+ os.getcwd() +'/../../../'+NACL_INSTALL_DIR+'/lib/libosgManipulator.a'
        str_ops += ' -DOSGGA_LIBRARY='+ os.getcwd() +'/../../../'+NACL_INSTALL_DIR+'/lib/libosgGA.a'
        str_ops += ' -DOSGFX_LIBRARY='+ os.getcwd() +'/../../../'+NACL_INSTALL_DIR+'/lib/libosgFX.a'
        str_ops += ' -DOSGDB_LIBRARY='+ os.getcwd() +'/../../../'+NACL_INSTALL_DIR+'/lib/libosgDB.a'
        
        str_ops += ' -DZLIB_INCLUDE_DIR='+ os.getcwd() +'"/../../../'+NACL_INSTALL_DIR+'/include"'
        str_ops += ' -DZLIB_LIBRARY_RELEASE='+ os.getcwd() +'/../../../'+NACL_INSTALL_DIR+'/lib/libz.a'
        # str_ops += ' -DZLIB_LIBRARY_DEBUG='+ os.getcwd() +'"/../../../'+NACL_INSTALL_DIR+'/lib/zlibd.a"';
        
        str_ops += ' -DGDAL_INCLUDE_DIR=../../../'+NACL_INSTALL_DIR+'/include/gdal';
        str_ops += ' -DGDAL_LIBRARY=../../../'+NACL_INSTALL_DIR+'/lib/gdal31.a';
        
        str_ops += ' -DCURL_INCLUDE_DIR=../../../'+NACL_INSTALL_DIR+'/include';
        str_ops += ' -DCURL_LIBRARY=../../../'+NACL_INSTALL_DIR+'/lib/libcurl.a';
        
        str_ops += ' -DMATH_LIBRARY='+NACL_ROOT+'/'+NACL_PEPPER_DIR+'/toolchain/win_pnacl/le32-nacl/lib/libm.a';
        
        pass
        
    if( str_target == "ndk"):
    
        # str_ops += " -DHAVE_SYS_SOCKET_H=1";
        
        str_ops += ' -DOPENGL_INCLUDE_DIR='+ ANDROID_NDK_PATH +'/sysroot/usr/include'
        
        str_ops += ' -DOSG_GEN_INCLUDE_DIR='+ os.getcwd() +'"/../../../'+ANDROID_ABI+'/include"'
        str_ops += ' -DOSG_INCLUDE_DIR='+ os.getcwd() +'"/../../../'+ANDROID_ABI+'/include"'
        
        str_ops += ' -DOPENTHREADS_LIBRARY='+ os.getcwd() +'/../../../'+ANDROID_ABI+'/lib/libOpenThreads.a'
        str_ops += ' -DOSG_LIBRARY='+ os.getcwd() +'/../../../'+ANDROID_ABI+'/lib/libosg.a'
        str_ops += ' -DOSGWIDGET_LIBRARY='+ os.getcwd() +'/../../../'+ANDROID_ABI+'/lib/libosgWidget.a'
        str_ops += ' -DOSGVIEWER_LIBRARY='+ os.getcwd() +'/../../../'+ANDROID_ABI+'/lib/libosgViewer.a'
        str_ops += ' -DOSGUTIL_LIBRARY='+ os.getcwd() +'/../../../'+ANDROID_ABI+'/lib/libosgUtil.a'
        str_ops += ' -DOSGTEXT_LIBRARY='+ os.getcwd() +'/../../../'+ANDROID_ABI+'/lib/libosgText.a'
        str_ops += ' -DOSGTERRAIN_LIBRARY='+ os.getcwd() +'/../../../'+ANDROID_ABI+'/lib/libosgTerrain.a'
        str_ops += ' -DOSGSIM_LIBRARY='+ os.getcwd() +'/../../../'+ANDROID_ABI+'/lib/libosgSim.a'
        str_ops += ' -DOSGSHADOW_LIBRARY='+ os.getcwd() +'/../../../'+ANDROID_ABI+'/lib/libosgShadow.a'
        str_ops += ' -DOSGMANIPULATOR_LIBRARY='+ os.getcwd() +'/../../../'+ANDROID_ABI+'/lib/libosgManipulator.a'
        str_ops += ' -DOSGGA_LIBRARY='+ os.getcwd() +'/../../../'+ANDROID_ABI+'/lib/libosgGA.a'
        str_ops += ' -DOSGFX_LIBRARY='+ os.getcwd() +'/../../../'+ANDROID_ABI+'/lib/libosgFX.a'
        str_ops += ' -DOSGDB_LIBRARY='+ os.getcwd() +'/../../../'+ANDROID_ABI+'/lib/libosgDB.a'
        
        
        str_ops += ' -DZLIB_INCLUDE_DIR='+ os.getcwd() +'"/../../../'+ANDROID_ABI+'/include"'
        str_ops += ' -DZLIB_LIBRARY_RELEASE='+ os.getcwd() +'/../../../'+ANDROID_ABI+'/lib/libz.a'
        # str_ops += ' -DZLIB_LIBRARY_DEBUG='+ os.getcwd() +'"/../../../'+ANDROID_ABI+'/lib/zlibd.a"';
        
        str_ops += ' -DGDAL_INCLUDE_DIR=../../../'+ANDROID_ABI+'/include/gdal';
        str_ops += ' -DGDAL_LIBRARY=../../../'+ANDROID_ABI+'/lib/gdal31.a';
        
        str_ops += ' -DCURL_INCLUDE_DIR=../../../'+ANDROID_ABI+'/include';
        str_ops += ' -DCURL_LIBRARY=../../../'+ANDROID_ABI+'/lib/libcurl.a';
        
    if( str_target == "mingw"):

        # str_ops += " -DCURL_LIBRARY=../../../3rdparty/lib/libcurl_imp.lib";
        # str_ops += " -DCURL_LIBRARY_DEBUG=../../../3rdparty/lib/libcurld_imp.lib";

        str_ops += ' -DGDAL_INCLUDE_DIR='+os.getcwd()+'/../../../3rdparty/include/gdal';
        str_ops += ' -DGDAL_LIBRARY='+os.getcwd()+'/../../../3rdparty/lib/libgdal31.dll.a';
        str_ops += ' -DGDAL_LIBRARY_DEBUG='+os.getcwd()+'/../../../3rdparty/lib/libgdal31d.dll.a';
        
        
    # else:
        # str_ops += " -DUSE_MSVC_RUNTIME_LIBRARY_DLL=1 ";
        # str_ops += " -DLIBRARY_OUTPUT_PATH=../../../3rdparty/lib ";
    
    if( str_target == "ndk"):
        my_configure(str_target , str_ops ,True , False)
    else:
        my_configure(str_target , str_ops ,False , False)
        
    my_build(str_target)


    
#main    
def main():

    ALL_TARGET = "vc" #默认vc
    if len(sys.argv) >= 2:
        ALL_TARGET = sys.argv[1]
    print( ALL_TARGET )

        
    #
    build_osgearth(ALL_TARGET)

main()
