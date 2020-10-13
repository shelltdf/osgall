#!/usr/bin/python
# -*- coding: cp936 -*-

import os
import sys
import string
import subprocess
from common import *


#内部路径 升级后可能会替换
# OSG_DIR_NAME = "OpenSceneGraph-3.2.1-rc3"
# OSG_DIR_NAME = "OpenSceneGraph-3.3.6"
# OSG_DIR_NAME = "OpenSceneGraph-3.4.0"
OSG_DIR_NAME = "OpenSceneGraph-3.6.4"
OSGQT_DIR_NAME = "osgQt-master"
VPB_DIR_NAME = "VirtualPlanetBuilder-master"



#    
#库建造定义
#

def build_osg( str_target ):
    print ("build OSG")
    my_make_build_dir( OSG_DIR_NAME , str_target )
    
    str_ops = " -DBUILD_OSG_APPLICATIONS=1 -DWIN32_USE_MP=1";
    str_ops += " -DACTUAL_3RDPARTY_DIR=../../../3rdparty";
    
    if( GLES_VER == 1 ):
        str_ops += " -DOPENGL_PROFILE=\"GLES1\""
        # str_ops += " -DOSG_GL1_AVAILABLE=OFF"
        # str_ops += " -DOSG_GL2_AVAILABLE=OFF"
        # str_ops += " -DOSG_GL3_AVAILABLE=OFF"
        # str_ops += " -DOSG_GLES1_AVAILABLE=ON"
        # str_ops += " -DOSG_GLES2_AVAILABLE=OFF"
        # str_ops += " -DOSG_GL_LIBRARY_STATIC=OFF"
        # str_ops += " -DOSG_GL_DISPLAYLISTS_AVAILABLE=OFF"
        # str_ops += " -DOSG_GL_MATRICES_AVAILABLE=ON"
        # str_ops += " -DOSG_GL_VERTEX_FUNCS_AVAILABLE=ON"
        # str_ops += " -DOSG_GL_VERTEX_ARRAY_FUNCS_AVAILABLE=ON"
        # str_ops += " -DOSG_GL_FIXED_FUNCTION_AVAILABLE=ON"
    if( GLES_VER == 2 ):
        str_ops += " -DOPENGL_PROFILE=\"GLES2\""
        # str_ops += " -DOSG_GL1_AVAILABLE=OFF"
        # str_ops += " -DOSG_GL2_AVAILABLE=OFF"
        # str_ops += " -DOSG_GL3_AVAILABLE=OFF"
        # str_ops += " -DOSG_GLES1_AVAILABLE=OFF"
        # str_ops += " -DOSG_GLES2_AVAILABLE=ON"
        # str_ops += " -DOSG_GL_LIBRARY_STATIC=OFF"
        # str_ops += " -DOSG_GL_DISPLAYLISTS_AVAILABLE=OFF"
        # str_ops += " -DOSG_GL_MATRICES_AVAILABLE=OFF"
        # str_ops += " -DOSG_GL_VERTEX_FUNCS_AVAILABLE=OFF"
        # str_ops += " -DOSG_GL_VERTEX_ARRAY_FUNCS_AVAILABLE=OFF"
        # str_ops += " -DOSG_GL_FIXED_FUNCTION_AVAILABLE=OFF"
    if( GLES_VER == 3 ):
        str_ops += " -DOPENGL_PROFILE=\"GLES3\""
        
    if( GL_VER == 1 ):
        str_ops += " -DOPENGL_PROFILE=\"GL1\""   
    if( GL_VER == 2 ):
        str_ops += " -DOPENGL_PROFILE=\"GL2\""           
    if( GL_VER == 3 ):
        str_ops += " -DOPENGL_PROFILE=\"GLCORE\""   
        # str_ops += ' -DGLCORE_ROOT=' + os.getcwd() + '/../../../3rdparty/include'   
        str_ops += ' -DGLCORE_INCLUDE_DIR=' + os.getcwd() + '/../../../3rdparty/include'   
        
            
    # for pc
    if( str_target == "vc" ):
        str_ops += ' -DLIBLAS_INCLUDE_DIR=""'
        
        str_ops += " -DGDAL_INCLUDE_DIR=../../../3rdparty/include/gdal"
        str_ops += " -DGDAL_LIBRARY=../../../3rdparty/lib/gdal31.lib"
        str_ops += " -DGDAL_LIBRARY_DEBUG=../../../3rdparty/lib/gdal31d.lib"
        
        str_ops += ' -DNVCORE_LIBRARY_DEBUG="' + os.getcwd() + '/../../../3rdparty/lib/nvcored.lib"'
        str_ops += ' -DNVIMAGE_LIBRARY_DEBUG="' + os.getcwd() + '/../../../3rdparty/lib/nvimaged.lib"'
        str_ops += ' -DNVMATH_LIBRARY_DEBUG="' + os.getcwd() + '/../../../3rdparty/lib/nvmathd.lib"'
        str_ops += ' -DNVTT_LIBRARY_DEBUG="' + os.getcwd() + '/../../../3rdparty/lib/nvttd.lib"'
        
        str_ops += ' -DTIFF_LIBRARY="' + os.getcwd() + '/../../../3rdparty/lib/tiff.lib"'
        str_ops += ' -DTIFF_LIBRARY_RELEASE="" ';
        str_ops += ' -DTIFF_LIBRARY_DEBUG="' + os.getcwd() + '/../../../3rdparty/lib/tiffd.lib"'
        

        if( USE_ANGLE ):
            str_ops += ' -DOPENGL_INCLUDE_DIR=' + os.getcwd() + '/../../../3rdparty/include'
    
        
        if( BUILD_DYNAMIC_LINK_LIBRARY ):
            str_ops += " -DDYNAMIC_OPENSCENEGRAPH=1 -DDYNAMIC_OPENTHREADS=1";
            str_ops += " -DCURL_IS_STATIC=OFF";
            str_ops += " -DJASPER_LIBRARY=../../../3rdparty/lib/jasper.lib"
            str_ops += " -DJASPER_LIBRARY_DEBUG=../../../3rdparty/lib/jasperd.lib"
        else:
            str_ops += " -DDYNAMIC_OPENSCENEGRAPH=0 -DDYNAMIC_OPENTHREADS=0";
            str_ops += " -DCURL_IS_STATIC=ON";
            str_ops += ' -DZLIB_LIBRARY="../../../3rdparty/lib/zlibstatic.lib"'
            str_ops += ' -DZLIB_LIBRARY_DEBUG="../../../3rdparty/lib/zlibstaticd.lib"'
            str_ops += ' -DPNG_LIBRARY="../../../3rdparty/lib/libpng16_static.lib"'
            str_ops += ' -DPNG_LIBRARY_DEBUG="../../../3rdparty/lib/libpng16_staticd.lib"'
        
    # str_ops = ' -DCOLLADA_DOM_ROOT="../../../3rdparty/include" ';
    # str_ops = ' -DCOLLADA_INCLUDE_DIR="../../../3rdparty/include" ';
    # str_ops = ' -DCOLLADA_BOOST_FILESYSTEM_LIBRARY="" ';
    # str_ops = ' -DCOLLADA_BOOST_FILESYSTEM_LIBRARY_DEBUG="" ';
    # str_ops = ' -DCOLLADA_BOOST_SYSTEM_LIBRARY="" ';
    # str_ops = ' -DCOLLADA_BOOST_SYSTEM_LIBRARY_DEBUG="" ';
    # str_ops = ' -DCOLLADA_STATIC_LIBRARY="../../../3rdparty/colladadom141.lib" ';
    
    if( str_target == "vc" ):
        if( BUILD_DYNAMIC_LINK_LIBRARY ):
            str_ops += " -DBUILD_OSG_EXAMPLES=0"
            str_ops += " -DBUILD_OSG_APPLICATIONS=1"
            
            str_ops += ' -DCURL_INCLUDE_DIR=/../../../3rdparty/include'
            str_ops += ' -DCURL_LIBRARY=../../../3rdparty/lib/libcurl_imp.lib'
            str_ops += ' -DCURL_LIBRARY_DEBUG=../../../3rdparty/lib/libcurld_imp.lib'
            
        else:
            str_ops += " -DBUILD_OSG_EXAMPLES=0"
            str_ops += " -DBUILD_OSG_APPLICATIONS=0"
            
            str_ops += ' -DJPEG_INCLUDE_DIR="' + os.getcwd() + '/../../../3rdparty/include"'
            str_ops += ' -DJPEG_LIBRARY=' + os.getcwd() + '../../../3rdparty/lib/jpeg.lib'
            str_ops += ' -DJPEG_LIBRARY_DEBUG=' + os.getcwd() + '../../../3rdparty/lib/jpegd.lib'
            
            str_ops += ' -DCURL_INCLUDE_DIR=' + os.getcwd() + '/../../../3rdparty/include'
            str_ops += ' -DCURL_LIBRARY=' + os.getcwd() + '../../../3rdparty/lib/libcurl_imp.lib'
            str_ops += ' -DCURL_LIBRARY_DEBUG=' + os.getcwd() + '../../../3rdparty/lib/libcurld_imp.lib'
            
        if(QuickTimeSDK_ON):
            str_ops += ' -DQUICKTIME_INCLUDE_DIR="' + QuickTimeSDK_DIR + '/CIncludes" ';
            str_ops += ' -DQUICKTIME_LIBRARY="'     + QuickTimeSDK_DIR + '/Libraries/QTMLClient.lib" ';
    
        if(Qt_ON):
            str_ops += " -DQt5Widgets_DIR=" + Qt_DIR + "/../Qt5Widgets"
            str_ops += " -DBUILD_OPENTHREADS_WITH_QT=1"
        else:
            str_ops += ' -DQt5Widgets_DIR=""'
            str_ops += " -DBUILD_OPENTHREADS_WITH_QT=0"
            
        if(FBX_ON):
            str_ops += ' -DFBX_INCLUDE_DIR="' + FBX_DIR + '/include/"';
            str_ops += ' -DFBX_LIBRARY="'       + FBX_DIR + '/lib/"' + FBX_LIB_PROFILE + '/release/libfbxsdk-md';
            str_ops += ' -DFBX_LIBRARY_DEBUG="' + FBX_DIR + '/lib/"' + FBX_LIB_PROFILE + '/debug/libfbxsdk-md';
        
        if(VC_PPAPI_MODE):
            str_ops += " -DBUILD_OSG_APPLICATIONS=0";
            str_ops += " -DBUILD_OSG_EXAMPLES=0"
            str_ops += " -DOPENGL_PROFILE=\"GLES2\""
            str_ops += " -DOSG_GL_LIBRARY_STATIC=ON"
            str_ops += ' -DOSG_WINDOWING_SYSTEM="none"'
            str_ops += ' -DOPENGL_INCLUDE_DIR:PATH="' + NACL_ROOT +'/'+ NACL_PEPPER_DIR+'/include/" '
            str_ops += ' -DOPENGL_gl_LIBRARY=' + NACL_GLES_LIB 
        
    if( str_target == "mingw" ):
        if( BUILD_DYNAMIC_LINK_LIBRARY ):
            str_ops += " -DBUILD_OSG_EXAMPLES=0"
            str_ops += " -DBUILD_OSG_APPLICATIONS=1"
        else:
            str_ops += " -DBUILD_OSG_EXAMPLES=0"
            str_ops += " -DBUILD_OSG_APPLICATIONS=0"
            
        str_ops += ' -DGDAL_INCLUDE_DIR='+os.getcwd()+'/../../../3rdparty/include/gdal';
        str_ops += ' -DGDAL_LIBRARY='+os.getcwd()+'/../../../3rdparty/lib/libgdal31.dll.a';
        str_ops += ' -DGDAL_LIBRARY_DEBUG='+os.getcwd()+'/../../../3rdparty/lib/libgdal31d.dll.a';
        
        
        if(Qt_ON):
            str_ops += " -DQt5Widgets_DIR=" + Qt_DIR + "/../Qt5Widgets"
            str_ops += " -DBUILD_OPENTHREADS_WITH_QT=1"
        else:
            str_ops += ' -DQt5Widgets_DIR=""'
            str_ops += " -DBUILD_OPENTHREADS_WITH_QT=0"
            
        
    if( str_target == "ndk" or str_target == "cw" ):
        # str_ops += " -DOSG_BUILD_PLATFORM_ANDROID=1"
        str_ops += " -DBUILD_OSG_EXAMPLES=1" # test
        str_ops += ' -DJPEG_INCLUDE_DIR="' + os.getcwd() + '/../../../'+ANDROID_ABI+'/include"'
        str_ops += ' -DTIFF_INCLUDE_DIR="' + os.getcwd() + '/../../../'+ANDROID_ABI+'/include"'
        str_ops += ' -DPNG_PNG_INCLUDE_DIR="' + os.getcwd() + '/../../../'+ANDROID_ABI+'/include"'
        str_ops += ' -DCURL_INCLUDE_DIR="' + os.getcwd() + '/../../../'+ANDROID_ABI+'/include"'
        str_ops += ' -DGIFLIB_INCLUDE_DIR="' + os.getcwd() + '/../../../'+ANDROID_ABI+'/include"'
        # str_ops += ' -DFREETYPE_DIR="' + os.getcwd() + '/../../'+ANDROID_ABI+'/"'
        str_ops += ' -DFREETYPE_INCLUDE_DIR_freetype2="' + os.getcwd() + '/../../../'+ANDROID_ABI+'/include/freetype2"'
        str_ops += ' -DFREETYPE_INCLUDE_DIR_ft2build="' + os.getcwd() + '/../../../'+ANDROID_ABI+'/include/freetype2"'
        str_ops += ' -DGDAL_DIR="' + os.getcwd() + '/../../../'+ANDROID_ABI+'/include"'
        str_ops += ' -DGDAL_INCLUDE_DIR=' + os.getcwd() + '/../../../'+ANDROID_ABI+'/include/gdal'
        
        if( BUILD_DYNAMIC_LINK_LIBRARY ):
            str_ops += " -DCURL_IS_STATIC=OFF"
        else:
            str_ops += " -DCURL_IS_STATIC=ON"
            
        str_ops += " -DCURL_LIBRARY=../../../3rdparty/lib/libcurl.a"
        str_ops += " -DCURL_LIBRARY_DEBUG=../../../3rdparty/lib/libcurld.a"
        str_ops += " -DJPEG_LIBRARY=../../../3rdparty/lib/libjpeg.a"
        str_ops += " -DJPEG_LIBRARY_DEBUG=../../../3rdparty/lib/libjpegd.a"
        
            
    if( str_target == "nc" ):
        str_ops += " -DBUILD_OSG_APPLICATIONS=0";
        str_ops += " -DBUILD_OSG_EXAMPLES=0" # test
        str_ops += " -DOPENGL_PROFILE=\"GLES2\""
        str_ops += " -DOSG_GL_LIBRARY_STATIC=ON"
        str_ops += ' -DOSG_WINDOWING_SYSTEM="none"'
        str_ops += ' -DOPENGL_INCLUDE_DIR="' + NACL_ROOT +'/'+ NACL_PEPPER_DIR+'/include" '
        str_ops += ' -DOPENGL_gl_LIBRARY="' + NACL_ROOT +'/'+ NACL_PEPPER_DIR+'/lib/pnacl/Release/libppapi_gles2.a" '
        # str_ops += ' -DJPEG_INCLUDE_DIR="' + os.getcwd() + '/../../../armeabi-v7a/include" '
        # str_ops += ' -DTIFF_INCLUDE_DIR="' + os.getcwd() + '/../../../armeabi-v7a/include" '
        # str_ops += ' -DPNG_INCLUDE_DIR="' + os.getcwd() + '/../../../armeabi-v7a/include" '
        # str_ops += ' -DFREETYPE_DIR="' + os.getcwd() + '/../../../armeabi-v7a/include/freetype2" '
        # str_ops += ' -DGDAL_DIR="' + os.getcwd() + '/../../../armeabi-v7a/include" '
        # str_ops += " -DGDAL_INCLUDE_DIR=../../../armeabi-v7a/include/gdal"


        
    my_configure(str_target , str_ops ,True)
    my_build(str_target)
    

def build_vpb( str_target ):
    print ("build VPB")
    my_make_build_dir( VPB_DIR_NAME , str_target )
    
    str_ops = ""
    # str_ops = " -DBUILD_OSG_APPLICATIONS=1 -DWIN32_USE_MP=1";
    str_ops += ' -D3rdPartyRoot=' + os.getcwd() + '/../../../3rdparty'
    str_ops += ' -DGDAL_INCLUDE_DIR=' + os.getcwd() + '/../../../3rdparty/include/gdal'
    str_ops += ' -DGDAL_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/gdal31.lib'
    str_ops += ' -DGDAL_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/gdal31d.lib'
    
    
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops += " -DDYNAMIC_VIRTUALPLANETBUILDER=1"
    else:
        str_ops += " -DDYNAMIC_VIRTUALPLANETBUILDER=0"

     
    if( GL_VER == 3 ):
        # str_ops += " -DOPENGL_PROFILE=\"GLCORE\""   
        # str_ops += ' -DGLCORE_ROOT=' + os.getcwd() + '/../../../3rdparty/include'   
        str_ops += ' -DGLCORE_INCLUDE_DIR=' + os.getcwd() + '/../../../3rdparty/include'  
        
    # str_ops += ' -DOPENTHREADS_LIBRARY=../../../3rdparty/lib/ot20-OpenThreads.lib'
    # str_ops += ' -DOSG_LIBRARY=../../../3rdparty/lib/osg130-osg.lib'
    # str_ops += ' -DOSGFX_LIBRARY=../../../3rdparty/lib/osg130-osgFX.lib'
    # str_ops += ' -DOSGGA_LIBRARY=../../../3rdparty/lib/osg130-osgGA.lib'
    # str_ops += ' -DOSGTEXT_LIBRARY=../../../3rdparty/lib/osg130-osgText.lib'
    # str_ops += ' -DOSGDB_LIBRARY=../../../3rdparty/lib/osg130-osgDB.lib'
    # str_ops += ' -DOSGVIEWER_LIBRARY=../../../3rdparty/lib/osg130-osgViewer.lib'
    # str_ops += ' -DOSGSIM_LIBRARY=../../../3rdparty/lib/osg130-osgSim.lib'
    # str_ops += ' -DOSGUTIL_LIBRARY=../../../3rdparty/lib/osg130-osgUtil.lib'
    # str_ops += ' -DOSGSHADOW_LIBRARY=../../../3rdparty/lib/osg130-osgShadow.lib'
    # str_ops += ' -DOSGMANIPULATOR_LIBRARY=../../../3rdparty/lib/osg130-osgManipulator.lib'
    # str_ops += ' -DOSGTERRAIN_LIBRARY=../../../3rdparty/lib/osg130-osgTerrain.lib'
    # str_ops += ' -DOSGWIDGET_LIBRARY=../../../3rdparty/lib/osg130-osgWidget.lib'
        
        
    my_configure(str_target , str_ops ,True)
    my_build(str_target)
    

def build_osgqt( str_target ):
    print ("build osgqt")
    my_make_build_dir( OSGQT_DIR_NAME , str_target )
    
    str_ops = ""

    # if( BUILD_DYNAMIC_LINK_LIBRARY ):
        # str_ops += " -DDYNAMIC_VIRTUALPLANETBUILDER=1"
    # else:
        # str_ops += " -DDYNAMIC_VIRTUALPLANETBUILDER=0"
        
    str_ops += " -DQt5_DIR=" + Qt_DIR + "/../Qt5"
    str_ops += " -DQt5Widgets_DIR=" + Qt_DIR + "/../Qt5Widgets"
    # str_ops += " -DBUILD_OPENTHREADS_WITH_QT=1"
    # str_ops += " -DOSG_MAINTAINER=1"
    str_ops += " -DOPENSCENEGRAPH_MAJOR_VERSION=3"
    str_ops += " -DOPENSCENEGRAPH_MINOR_VERSION=6"
    str_ops += " -DOPENSCENEGRAPH_PATCH_VERSION=4"
            
    my_configure(str_target , str_ops)
    my_build(str_target)

    
#main    
def main():

    ALL_TARGET = "vc" #默认vc
    if len(sys.argv) >= 2:
        ALL_TARGET = sys.argv[1]
    print( ALL_TARGET )

        
    #osg
    build_osg(ALL_TARGET)
    
    #osgQT
    if(Qt_ON):
        build_osgqt(ALL_TARGET)
    
    #VPB
    if(ALL_TARGET == "vc" and BUILD_DYNAMIC_LINK_LIBRARY and VC_PPAPI_MODE==False):
        build_vpb(ALL_TARGET)

main()
