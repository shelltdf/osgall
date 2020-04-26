#!/usr/bin/python
# -*- coding: cp936 -*-

import os
import sys
import string
import subprocess
from common import *


#内部路径 升级后可能会替换
crashrpt_DIR_NAME = "crashrpt-code-4616504670be5a425a525376648d912a72ce18f2"
crunch_DIR_NAME = "crunch-master"
pthreads_win32_DIR_NAME = "pthreads-win32.cmake-master"
freeglut_DIR_NAME = "freeglut-3.0.0"
# OSG_DIR_NAME = "OpenSceneGraph-3.2.1-rc3"
# OSG_DIR_NAME = "OpenSceneGraph-3.3.6"
# OSG_DIR_NAME = "OpenSceneGraph-3.4.0"
ZLIB_DIR_NAME = "zlib-1.2.8"
GIFLIB_DIR_NAME = "giflib-5.1.4"
LEVELDB_DIR_NAME = "leveldb-master"
SQLITE3_DIR_NAME = "sqlite-amalgamation-3100200"
GEOS_DIR_NAME = "libgeos-3.5.0"
PROJ4_DIR_NAME = "proj-6.1.1"
# PROJ4_DIR_NAME = "proj.4-4.9.2"
EXPAT_DIR_NAME = "expat-2.1.0"
freetype_DIR_NAME = "freetype-2.5.1"
jpeg_DIR_NAME = "jpeg-9a"
curl_DIR_NAME = "curl-7.33.0"
png_DIR_NAME = "libpng-1.6.15"
tiff_DIR_NAME = "tiff-4.0.3"
nvtt_DIR_NAME = "nvidia-texture-tools"
lua_DIR_NAME = "lua-5.2.3"
# gdal_DIR_NAME = "gdal-svn-cmake4gdal"
gdal_DIR_NAME = "gdal/gdal"
boost_DIR_NAME = "boost-cmake_1_41"
dae_DIR_NAME = "collada-dom-2.4.0"
libxml2_DIR_NAME = "libxml2-2.9.2"
ilmbase_DIR_NAME = "ilmbase-2.2.0"
openexr_DIR_NAME = "openexr-2.2.0"
# geotiff_DIR_NAME = "libgeotiff-1.4.2"
geotiff_DIR_NAME = "libgeotiff"
liblas_DIR_NAME = "libLAS-1.8.1"
jasper_DIR_NAME = "jasper-2.0.12"
eigen_DIR_NAME = "eigen-eigen-5a0156e40feb"
flann_DIR_NAME = "flann-1.8.4-src"
qhull_DIR_NAME = "qhull-master"
rapidxml_DIR_NAME = "rapidxml-1.13"
draco_DIR_NAME = "draco-master" #1.3.5
jsoncpp_DIR_NAME = "jsoncpp-master"
glew_DIR_NAME = "glew-2.1.0/build/cmake"
imgui_DIR_NAME = "imgui-master"
stb_DIR_NAME = "stb-master"
gtest_DIR_NAME = "googletest-master"


#    
#库建造定义
#
def build_crashrpt( str_target ):
    print ("build crashrpt")
    
    my_make_build_dir( crashrpt_DIR_NAME , str_target )

    str_ops = ''

    my_configure( str_target , str_ops)
    my_build(str_target)
    
def build_crunch( str_target ):
    print ("build crunch")
    
    my_make_build_dir( crunch_DIR_NAME , str_target )

    str_ops = ''
    
    # if( BUILD_DYNAMIC_LINK_LIBRARY ):
        # str_ops += " -DCRUNCH_BUILD_SHARED=1";
    # else:
        # str_ops += " -DCRUNCH_BUILD_SHARED=0";

    # 只有static
    str_ops += " -DCRUNCH_BUILD_SHARED=0"
    
    my_configure( str_target , str_ops)
    my_build(str_target)
    
def build_gtest( str_target ):
    print ("build gtest")
    
    my_make_build_dir( gtest_DIR_NAME , str_target )

    str_ops = ''
    
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops += " -DBUILD_SHARED_LIBS=1";
    else:
        str_ops += " -DBUILD_SHARED_LIBS=0";
        
    my_configure( str_target , str_ops)
    my_build(str_target)
    
    
def build_imgui( str_target ):
    print ("build imgui")
    
    my_make_build_dir( imgui_DIR_NAME , str_target )

    str_ops = ''

    my_configure( str_target , str_ops)
    my_build(str_target)
    
def build_stb( str_target ):
    print ("build stb")
    
    cmd = 'copy .\\prebuild\\'+ stb_DIR_NAME +'\\stb.h 3rdparty\\include\\stb.h'
    my_exec( str_target,cmd )
    cmd = 'copy .\\prebuild\\'+ stb_DIR_NAME +'\\stb_c_lexer.h 3rdparty\\include\\stb_c_lexer.h'
    my_exec( str_target,cmd )
    cmd = 'copy .\\prebuild\\'+ stb_DIR_NAME +'\\stb_connected_components.h 3rdparty\\include\\stb_connected_components.h'
    my_exec( str_target,cmd )
    cmd = 'copy .\\prebuild\\'+ stb_DIR_NAME +'\\stb_divide.h 3rdparty\\include\\stb_divide.h'
    my_exec( str_target,cmd )
    cmd = 'copy .\\prebuild\\'+ stb_DIR_NAME +'\\stb_ds.h 3rdparty\\include\\stb_ds.h'
    my_exec( str_target,cmd )
    cmd = 'copy .\\prebuild\\'+ stb_DIR_NAME +'\\stb_dxt.h 3rdparty\\include\\stb_dxt.h'
    my_exec( str_target,cmd )
    cmd = 'copy .\\prebuild\\'+ stb_DIR_NAME +'\\stb_easy_font.h 3rdparty\\include\\stb_easy_font.h'
    my_exec( str_target,cmd )
    cmd = 'copy .\\prebuild\\'+ stb_DIR_NAME +'\\stb_herringbone_wang_tile.h 3rdparty\\include\\stb_herringbone_wang_tile.h'
    my_exec( str_target,cmd )
    cmd = 'copy .\\prebuild\\'+ stb_DIR_NAME +'\\stb_image.h 3rdparty\\include\\stb_image.h'
    my_exec( str_target,cmd )
    cmd = 'copy .\\prebuild\\'+ stb_DIR_NAME +'\\stb_image_resize.h 3rdparty\\include\\stb_image_resize.h'
    my_exec( str_target,cmd )
    cmd = 'copy .\\prebuild\\'+ stb_DIR_NAME +'\\stb_image_write.h 3rdparty\\include\\stb_image_write.h'
    my_exec( str_target,cmd )
    cmd = 'copy .\\prebuild\\'+ stb_DIR_NAME +'\\stb_include.h 3rdparty\\include\\stb_include.h'
    my_exec( str_target,cmd )
    cmd = 'copy .\\prebuild\\'+ stb_DIR_NAME +'\\stb_leakcheck.h 3rdparty\\include\\stb_leakcheck.h'
    my_exec( str_target,cmd )
    cmd = 'copy .\\prebuild\\'+ stb_DIR_NAME +'\\stb_perlin.h 3rdparty\\include\\stb_perlin.h'
    my_exec( str_target,cmd )
    cmd = 'copy .\\prebuild\\'+ stb_DIR_NAME +'\\stb_rect_pack.h 3rdparty\\include\\stb_rect_pack.h'
    my_exec( str_target,cmd )
    cmd = 'copy .\\prebuild\\'+ stb_DIR_NAME +'\\stb_sprintf.h 3rdparty\\include\\stb_sprintf.h'
    my_exec( str_target,cmd )
    cmd = 'copy .\\prebuild\\'+ stb_DIR_NAME +'\\stb_textedit.h 3rdparty\\include\\stb_textedit.h'
    my_exec( str_target,cmd )
    cmd = 'copy .\\prebuild\\'+ stb_DIR_NAME +'\\stb_tilemap_editor.h 3rdparty\\include\\stb_tilemap_editor.h'
    my_exec( str_target,cmd )
    cmd = 'copy .\\prebuild\\'+ stb_DIR_NAME +'\\stb_truetype.h 3rdparty\\include\\stb_truetype.h'
    my_exec( str_target,cmd )
    cmd = 'copy .\\prebuild\\'+ stb_DIR_NAME +'\\stb_voxel_render.h 3rdparty\\include\\stb_voxel_render.h'
    my_exec( str_target,cmd )

    
    
def build_glew( str_target ):
    print ("build glew")
    my_make_build_dir( glew_DIR_NAME , str_target )

    str_ops = ''

    my_configure( str_target , str_ops)
    my_build(str_target)
    
def build_eigen( str_target ):
    print ("build eigen")
    my_make_build_dir( eigen_DIR_NAME , str_target )

    str_ops = ''

    my_configure( str_target , str_ops)
    my_build(str_target)
    
def build_draco( str_target ):
    print ("build draco")
    my_make_build_dir( draco_DIR_NAME , str_target )

    str_ops = ''

    my_configure( str_target , str_ops)
    my_build(str_target)

def build_jsoncpp( str_target ):
    print ("build jsoncpp")
    my_make_build_dir( jsoncpp_DIR_NAME , str_target )

    str_ops = ''
    str_ops += ' -DBUILD_TESTING=0'
    str_ops += ' -DJSONCPP_WITH_TESTS=0'

    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops += " -DBUILD_SHARED_LIBS=1";
    else:
        str_ops += " -DBUILD_SHARED_LIBS=0";
        
        
    my_configure( str_target , str_ops)
    my_build(str_target)
    

def build_rapidxml( str_target ):
    print ("build rapidxml")
    
    cmd = 'copy .\\prebuild\\'+ rapidxml_DIR_NAME +'\\rapidxml.hpp 3rdparty\\include\\rapidxml.hpp'
    my_exec( str_target,cmd )
    
    cmd = 'copy .\\prebuild\\'+ rapidxml_DIR_NAME +'\\rapidxml_iterators.hpp 3rdparty\\include\\rapidxml_iterators.hpp'
    my_exec( str_target,cmd )
    
    cmd = 'copy .\\prebuild\\'+ rapidxml_DIR_NAME +'\\rapidxml_print.hpp 3rdparty\\include\\rapidxml_print.hpp'
    my_exec( str_target,cmd )
    
    cmd = 'copy .\\prebuild\\'+ rapidxml_DIR_NAME +'\\rapidxml_utils.hpp 3rdparty\\include\\rapidxml_utils.hpp'
    my_exec( str_target,cmd )
    
    
def build_qhull( str_target ):
    print ("build qhull")
    my_make_build_dir( qhull_DIR_NAME , str_target )

    str_ops = ''

    my_configure( str_target , str_ops)
    my_build(str_target)
    
def build_flann( str_target ):
    print ("build flann")
    my_make_build_dir( flann_DIR_NAME , str_target )

    str_ops = ''
    str_ops += ' -DBUILD_MATLAB_BINDINGS=0'
    str_ops += ' -DBUILD_PYTHON_BINDINGS=0'

    my_configure( str_target , str_ops)
    my_build(str_target)
    
    
def build_jasper( str_target ):
    print ("build jasper")

    my_make_build_dir( jasper_DIR_NAME , str_target )

    str_ops = ''
    
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops += " -DJAS_ENABLE_SHARED=1";
    else:
        str_ops += " -DJAS_ENABLE_SHARED=0";
        
    str_ops += ' -DJPEG_INCLUDE_DIR=' + os.getcwd() + '../../../3rdparty/include'
    str_ops += ' -DJPEG_LIBRARY=' + os.getcwd() + '../../../3rdparty/lib/jpeg.lib'
    str_ops += ' -DJPEG_LIBRARY_DEBUG=' + os.getcwd() + '../../../3rdparty/lib/jpegd.lib'
    
    my_configure( str_target , str_ops,True)
    my_build(str_target)
    
    
def build_pthreads_win32( str_target ):
    print ("build pthreads")

    my_make_build_dir( pthreads_win32_DIR_NAME , str_target )

    str_ops  = PTHREADS_BUILD_PROFILE + '=1'
    
    if( not BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops += " -DPTHREADS_BUILD_STATIC=1";
        
    my_configure( str_target , str_ops)

    my_build(str_target)
    
def build_freeglut( str_target ):
    print ("build freeglut")
    my_make_build_dir( freeglut_DIR_NAME , str_target )

    str_ops  = '-DFREEGLUT_BUILD_DEMOS=0 -DINSTALL_PDB=0'
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops += " -DFREEGLUT_BUILD_SHARED_LIBS=1";
        str_ops += " -DFREEGLUT_BUILD_STATIC_LIBS=0";
    else:
        str_ops += " -DFREEGLUT_BUILD_STATIC_LIBS=1";
        str_ops += " -DFREEGLUT_BUILD_SHARED_LIBS=0";
    
    if(str_target == "nc"):
        str_ops += ' -DFREEGLUT_GLES=1 '
    else:
        str_ops += ' -DFREEGLUT_GLES=0 '
        
    my_configure( str_target , str_ops)
    my_build(str_target)    

    
def build_zlib( str_target ):
    print ("build zlib")
    # 同时生成dynamic和static 不需要区别配置
    my_make_build_dir( ZLIB_DIR_NAME , str_target )
    
    if(str_target == "ndk" or str_target == "cw"):
        str  = ' -DINSTALL_BIN_DIR="../../../' + ANDROID_ABI + '/bin"'
        str += ' -DINSTALL_INC_DIR="../../../' + ANDROID_ABI + '/include"'
        str += ' -DINSTALL_LIB_DIR="../../../' + ANDROID_ABI + '/lib"'
        str += ' -DINSTALL_MAN_DIR="../../../' + ANDROID_ABI + '/share/man"'
        str += ' -DINSTALL_PKGCONFIG_DIR="../../../' + ANDROID_ABI + '/share/pkgconfig"'
        my_configure( str_target , str)
        
    elif(str_target == "nc"):
        str  = ' -DINSTALL_BIN_DIR="../../../' + NACL_INSTALL_DIR + '/bin"'
        str += ' -DINSTALL_INC_DIR="../../../' + NACL_INSTALL_DIR + '/include"'
        str += ' -DINSTALL_LIB_DIR="../../../' + NACL_INSTALL_DIR + '/lib"'
        str += ' -DINSTALL_MAN_DIR="../../../' + NACL_INSTALL_DIR + '/share/man"'
        str += ' -DINSTALL_PKGCONFIG_DIR="../../../' + NACL_INSTALL_DIR + '/share/pkgconfig"'
        str += ' -DBUILD_EXECUTABLE=OFF'
        str += ' -DBUILD_STAITC_LIBRARY=ON'
        str += ' -DBUILD_DYNAMIC_LIBRARY=OFF'
        my_configure( str_target , str)
        
    else:
        str = ""
        if( BUILD_DYNAMIC_LINK_LIBRARY ):
            str += ' -DBUILD_STAITC_LIBRARY=OFF'
            str += ' -DBUILD_DYNAMIC_LIBRARY=ON'
        else:
            str += ' -DBUILD_STAITC_LIBRARY=ON'
            str += ' -DBUILD_DYNAMIC_LIBRARY=OFF'
        
        my_configure( str_target , str + ' -DINSTALL_BIN_DIR="../../../3rdparty/bin" -DINSTALL_INC_DIR="../../../3rdparty/include" -DINSTALL_LIB_DIR="../../../3rdparty/lib" -DINSTALL_MAN_DIR="../../../3rdparty/share/man" -DINSTALL_PKGCONFIG_DIR="../../../3rdparty/share/pkgconfig"')
    my_build(str_target)
    
def build_giflib( str_target ):
    print ("build giflib")
    # 只有static
    my_make_build_dir( GIFLIB_DIR_NAME , str_target )
    
    str_ops  = '-DFREEGLUT_BUILD_DEMOS=0 -DINSTALL_PDB=0'
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops += " -DGIFLIB_BUILD_STATIC=0";
    else:
        str_ops += " -DGIFLIB_BUILD_STATIC=1";
        
    # if(str_target == "ndk"):
        # pass
    # else:
    
    my_configure( str_target , str_ops)
        
    my_build(str_target)
    
def build_leveldb( str_target ):
    print ("build leveldb")
    # 同时生成dynamic和static 不需要区别配置
    my_make_build_dir( LEVELDB_DIR_NAME , str_target )
    # if(str_target == "ndk"):
        # my_configure( str_target , '') #因为缺少必要的系统api所以无法支持android
        # pass
    # else:
    
    str_ops  = ''
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops += " -DLEVELDB_BUILD_STATIC=0";
    else:
        str_ops += " -DLEVELDB_BUILD_STATIC=1";
    
    my_configure( str_target , '')
    my_build(str_target)

def build_sqlite3( str_target ):
    print ("build sqlite3")
    my_make_build_dir( SQLITE3_DIR_NAME , str_target )
    
    str_ops  = ''
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops += " -DSQLITE_BUILD_STATIC=0";
    else:
        str_ops += " -DSQLITE_BUILD_STATIC=1";
        
        
    if(str_target == "ndk"):
        my_configure( str_target , str_ops,True)
        my_build(str_target)
    else:
        my_configure( str_target , str_ops)
        my_build(str_target)
    
def build_geos( str_target ):
    print ("build geos")
    my_make_build_dir( GEOS_DIR_NAME , str_target )
    
    str_ops = "";
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops += " -DBUILD_STATIC_LIBRARY=0";
    else:
        str_ops += " -DBUILD_STATIC_LIBRARY=1";
        
    # str_ops += ' -DCMAKE_BUILD_TYPE=""';
    
    if(str_target == "nc"):
        str_ops += ' -DCMAKE_AR="${PLATFORM_PREFIX}/bin/${PLATFORM_TRIPLET}-ar"';
        str_ops += ' -DCMAKE_RANLIB="${PLATFORM_PREFIX}/bin/${PLATFORM_TRIPLET}-ranlib"';
    
    if(str_target == "ndk"):
        my_configure( str_target , str_ops + ' -DBUILD_TESTING=OFF -DGEOS_ENABLE_TESTS=OFF -DProject_WC_REVISION=4084' ,True)  #因为目前的版本是4084
        pass
    else:
        my_configure( str_target , str_ops + ' -DBUILD_TESTING=OFF -DGEOS_ENABLE_TESTS=OFF' ,True)
    my_build(str_target)
    
def build_proj4( str_target ):
    print ("build proj4")
    my_make_build_dir( PROJ4_DIR_NAME , str_target )
    
    # str_ops = " -DBUILD_TESTING=0";
    str_ops = " -DPROJ_TESTS=0";
    
    str_ops += ' -DSQLITE3_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/sqlite.lib'
    str_ops += ' -DSQLITE3_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/sqlited.lib'
    
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops += " -DBUILD_LIBPROJ_SHARED=1";
    else:
        str_ops += " -DBUILD_LIBPROJ_SHARED=0";
        
    if(str_target == "ndk" or str_target == "cw"):
        str_ops += " -DBUILD_CS2CS=0";
        str_ops += " -DBUILD_GEOD=0";
        str_ops += " -DBUILD_NAD2BIN=0";
        pass
        
    if(str_target == "nc"):
        str_ops += " -DBUILD_CS2CS=0";
        str_ops += " -DBUILD_GEOD=0";
        str_ops += " -DBUILD_NAD2BIN=0";
        str_ops += " -DBUILD_PROJ=0";
        pass
    
    my_configure( str_target , str_ops ,True)
    my_build(str_target)
    
def build_expat( str_target ):
    print ("build expat")
    my_make_build_dir( EXPAT_DIR_NAME , str_target )
    
    str_ops = " -DBUILD_examples=0 -DBUILD_tests=0 -DBUILD_tools=0";
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops += " -DBUILD_shared=1";
    else:
        str_ops += " -DBUILD_shared=0";
        
    if(str_target == "ndk"):
        #str_ops += " -DBUILD_shared=0";
        pass

    my_configure( str_target , str_ops ,True)
    my_build(str_target)
    
def build_freetype( str_target ):
    print ("build freetype")
    my_make_build_dir( freetype_DIR_NAME , str_target )
    
    str_ops = ""
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops = " -DFT2_BUILD_STATIC=0";
    else:
        str_ops = " -DFT2_BUILD_STATIC=1";
        
    my_configure(str_target ,str_ops)
    my_build(str_target)

def build_jpeg( str_target ):
    print ("build jpeg")
    my_make_build_dir( jpeg_DIR_NAME , str_target )
    
    str_ops = ""
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops = " -DJPEG_BUILD_STATIC=0";
    else:
        str_ops = " -DJPEG_BUILD_STATIC=1";
        
    my_configure(str_target ,str_ops)
    my_build(str_target)

def build_curl( str_target ):
    print ("build curl")
    my_make_build_dir( curl_DIR_NAME , str_target )
    
    str_ops = ""
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops = " -DCURL_STATICLIB=0";
    else:
        str_ops = " -DCURL_STATICLIB=1";
        
    if(str_target == "vc"):
        str_ops += ' -DZLIB_INCLUDE_DIR="../../../3rdparty/include"'
        if( BUILD_DYNAMIC_LINK_LIBRARY ):
            str_ops += ' -DZLIB_LIBRARY=' + os.getcwd() + '/"../../../3rdparty/lib/zlib.lib"'
            str_ops += ' -DZLIB_LIBRARY_DEBUG=' + os.getcwd() + '/"../../../3rdparty/lib/zlibd.lib"'
            str_ops += ' -DZLIB_LIBRARY_RELEASE=' + os.getcwd() + '/"../../../3rdparty/lib/zlib.lib"'
        else:
            # str_ops += ' -DZLIB_LIBRARY=' + os.getcwd() + '/"../../../3rdparty/lib/zlibstatic.lib"'
            # str_ops += ' -DZLIB_LIBRARY_DEBUG=' + os.getcwd() + '/"../../../3rdparty/lib/zlibstaticd.lib"'
            # str_ops += ' -DZLIB_LIBRARY_RELEASE=' + os.getcwd() + '/"../../../3rdparty/lib/zlibstatic.lib"'
            pass
        my_configure(str_target ,str_ops )
        
    elif(str_target == "mingw"):
        str_ops += ' -DZLIB_INCLUDE_DIR=' + os.getcwd() + '/"../../../3rdparty/include"'
        str_ops += ' -DZLIB_LIBRARY=' + os.getcwd() + '/"../../../3rdparty/lib/libzlib.dll.a"'
        str_ops += ' -DZLIB_LIBRARY_DEBUG=' + os.getcwd() + '/"../../../3rdparty/lib/libzlibd.dll.a"'
        
        my_configure(str_target ,str_ops)
        
    elif(str_target == "ndk" or str_target == "cw"):
        str_ops += ' -DCURL_STATICLIB="ON"'
        str_ops += ' -DZLIB_INCLUDE_DIR="../../../armeabi-v7a/include"'
        str_ops += ' -DZLIB_LIBRARY="../../../armeabi-v7a/lib/zlib.a"'
        str_ops += ' -DZLIB_LIBRARY_DEBUG="../../../armeabi-v7a/lib/zlib.a"'
        str_ops += ' -DHAVE_STRERROR_R=0'
        str_ops += ' -DBUILD_CURL_EXE=OFF'
        str_ops += ' -DBUILD_CURL_TESTS=OFF'
        
        my_configure(str_target ,str_ops ,True)
        
    elif(str_target == "nc"):
        str_ops += ' -DCURL_STATICLIB="ON"'
        str_ops += ' -DZLIB_INCLUDE_DIR="../../../'+NACL_INSTALL_DIR+'/include"'
        str_ops += ' -DZLIB_LIBRARY_RELEASE="../../../'+NACL_INSTALL_DIR+'/lib/libz.a"'
        str_ops += ' -DHAVE_STRERROR_R=0'
        str_ops += ' -DBUILD_CURL_EXE=OFF'
        str_ops += ' -DBUILD_CURL_TESTS=OFF'
        my_configure(str_target,str_ops)
    else:
        pass
    my_build(str_target)

def build_png( str_target ):
    print ("build png")
    my_make_build_dir( png_DIR_NAME , str_target )
    
    str_ops = ""
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops = " -DPNG_SHARED=1 -DPNG_STATIC=0";
    else:
        str_ops = " -DPNG_SHARED=0 -DPNG_STATIC=1";
        
    if(str_target == "vc"):
        my_configure(str_target ,str_ops + ' -DZLIB_INCLUDE_DIR="../../../3rdparty/include" -DZLIB_LIBRARY="../../../3rdparty/lib/zlib" -DZLIB_LIBRARY_DEBUG:FILEPATH="../../../3rdparty/lib/zlibd" -DPNG_DEBUG=ON ')
    elif(str_target == "mingw"):
        str_ops += ' -DZLIB_INCLUDE_DIR="../../../3rdparty/include"'
        str_ops += ' -DZLIB_LIBRARY=' + os.getcwd() + '/"../../../3rdparty/lib/libzlib.dll.a"'
        str_ops += ' -DZLIB_LIBRARY_DEBUG=' + os.getcwd() + '/"../../../3rdparty/lib/libzlibd.dll.a"'
        str_ops += ' -DPNG_DEBUG=ON'
        # str_ops += ' -DSKIP_INSTALL_ALL=1'
        str_ops += ' -DSKIP_INSTALL_FILES=1'
        # str_ops += ' -DSKIP_INSTALL_EXPORT=1'
        str_ops += ' -DSKIP_INSTALL_EXECUTABLES=1'
        my_configure(str_target ,str_ops)
    elif(str_target == "ndk" or str_target == "cw"):
        my_configure(str_target ,' -DZLIB_INCLUDE_DIR="../../../armeabi-v7a/include" -DZLIB_LIBRARY="../../../armeabi-v7a/lib/zlib" -DZLIB_LIBRARY_DEBUG:FILEPATH="../../../armeabi-v7a/lib/zlibd" -DPNG_DEBUG=ON -DPNG_SHARED=OFF -DPNG_TESTS=OFF -DSKIP_INSTALL_FILES=1 -DSKIP_INSTALL_EXPORT=1 -DSKIP_INSTALL_EXECUTABLES=1')
    elif(str_target == "nc"):
        str_ops += ' -DZLIB_INCLUDE_DIR="../../../'+NACL_INSTALL_DIR+'/include"'
        str_ops += ' -DZLIB_LIBRARY="../../../'+NACL_INSTALL_DIR+'/lib/zlib"'
        str_ops += ' -DZLIB_LIBRARY_DEBUG:FILEPATH="../../../'+NACL_INSTALL_DIR+'/lib/zlibd"'
        str_ops += ' -DM_LIBRARY='+NACL_ROOT+'/'+NACL_PEPPER_DIR+'/toolchain/win_pnacl/le32-nacl/lib/libm.a';
        str_ops += ' -DPNG_DEBUG=ON'
        str_ops += ' -DPNG_SHARED=OFF'
        str_ops += ' -DPNG_TESTS=OFF'
        str_ops += ' -DSKIP_INSTALL_FILES=1'
        str_ops += ' -DSKIP_INSTALL_EXPORT=1'
        str_ops += ' -DSKIP_INSTALL_EXECUTABLES=1'
        my_configure(str_target,str_ops)
    else:
        pass
        
    my_build(str_target)
    
def build_openexr( str_target ):

    # 编译过程中有一些自动化工具存在 所以目前没法交叉编译
    
    str_ops = ""
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops = " -DBUILD_SHARED_LIBS=1";
    else:
        str_ops = " -DBUILD_SHARED_LIBS=0";
        
    print ("build ilmbase")
    my_make_build_dir( ilmbase_DIR_NAME , str_target )
    my_configure(str_target ,str_ops + ' -DNAMESPACE_VERSIONING=OFF -DPACK_BINARY_NSIS=OFF')
    my_build(str_target)
    
    
    
    print ("build openexr")
    my_make_build_dir( openexr_DIR_NAME , str_target )
   
    str_ops += ' -DNAMESPACE_VERSIONING=OFF'
    str_ops += ' -DILMBASE_PACKAGE_PREFIX=../../../3rdparty/'
    str_ops += ' -DPACK_BINARY_NSIS=OFF'
    
    if(str_target == "vc"):
        if( BUILD_DYNAMIC_LINK_LIBRARY ):
            str_ops += ' -DZLIB_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/zlib.lib'
            str_ops += ' -DZLIB_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/zlibd.lib'
        else:
            str_ops += ' -DZLIB_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/zlibstatic.lib'
            str_ops += ' -DZLIB_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/zlibstaticd.lib'
    
    if(str_target == "mingw"):
        if( BUILD_DYNAMIC_LINK_LIBRARY ):
            str_ops += ' -DZLIB_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/libzlib.dll.a'
            str_ops += ' -DZLIB_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/libzlibd.dll.a'
        else:
            str_ops += ' -DZLIB_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/libzlib.a'
            str_ops += ' -DZLIB_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/libzlibd.a'
            
    my_configure(str_target ,str_ops)
    my_build(str_target)
    
    
def build_tiff( str_target ):
    print ("build tiff")
    my_make_build_dir( tiff_DIR_NAME , str_target )
    
    str_ops = ""
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops = " -DTIFF_BUILD_STATIC=0";
    else:
        str_ops = " -DTIFF_BUILD_STATIC=1";
        
    # if(str_target == "mingw"):
        # str_ops += ' -DCMAKE_C_COMPILER=g++'
        
    my_configure(str_target ,str_ops)
    my_build(str_target)
    
def build_nvtt( str_target ):
    print ("build nvtt")
    my_make_build_dir( nvtt_DIR_NAME , str_target )
    
    str_ops = ""
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops = " -DNVTT_SHARED=1";
    else:
        str_ops = " -DNVTT_SHARED=0";
        
    my_configure(str_target ,str_ops + ' -DHAVE_JPEG="OFF" -DHAVE_MAYA="OFF" -DHAVE_PNG="OFF" -DHAVE_TIFF="OFF" -DHAVE_OPENEXR="OFF"')
    my_build(str_target)
    
def build_lua( str_target ):
    print ("build lua")
    my_make_build_dir( lua_DIR_NAME , str_target )
    
    str_ops = ""
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops = " -DLUA_BUILD_STATIC=0";
    else:
        str_ops = " -DLUA_BUILD_STATIC=1";
        
    my_configure(str_target , str_ops)
    
    #' -D"getlocaledecpoint()=\'.\'" '
    #这里的宏不起作用 所以要在lua源码出错的地方追加以下代码
    #if ANDROID
    #define getlocaledecpoint()	'.'
    my_build(str_target) 

    
def build_geotiff( str_target ):
    print ("build geotiff")
    my_make_build_dir( geotiff_DIR_NAME , str_target )
    
    str = ''
    
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str += " -DBUILD_STATIC_LIBRARY=0";
    else:
        str += " -DBUILD_STATIC_LIBRARY=1";
        str += " -DWITH_UTILITIES=0";
        
    str += ' -DPROJ_DIR="../../../3rdparty"'
    str += ' -DPROJ_INCLUDE_DIR="../../../3rdparty/include"'
    str += ' -DPROJ_LIBRARY=../../../3rdparty/lib/proj_6_1.lib'
    str += ' -DPROJ_LIBRARY_DEBUG=../../../3rdparty/lib/proj_6_1_d.lib'
    
    if(str_target == "nc"):
        str += " -DWITH_UTILITIES=0";
        str += ' -DTIFF_INCLUDE_DIR=../../../'+NACL_INSTALL_DIR+'/include '
        str += ' -DTIFF_LIBRARY=../../../'+NACL_INSTALL_DIR+'/lib/libtiff.a'
    
    if(str_target == "ndk"):
        str += " -DWITH_UTILITIES=0";
        str += ' -DTIFF_INCLUDE_DIR=../../../'+ANDROID_ABI+'/include '
        str += ' -DTIFF_LIBRARY=../../../'+ANDROID_ABI+'/lib/libtiff.a'

        
    my_configure(str_target ,str ,True)
    my_build(str_target)
    
def build_liblas( str_target ):
    print ("build liblas")
    my_make_build_dir( liblas_DIR_NAME , str_target )
    
    str = ''
    
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str += ' -DBoost_USE_STATIC_LIBS=OFF';
    else:
        str += ' -DBoost_USE_STATIC_LIBS=ON';
        if(str_target == "vc"):
            str += ' -DGEOTIFF_LIBRARY=../../../3rdparty/lib/geotiff.lib'
    
    str += ' -DZLIB_INCLUDE_DIR="../../../3rdparty/include"'
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str += ' -DZLIB_LIBRARY="../../../../3rdparty/lib/zlib"'
    else:
        str += ' -DZLIB_LIBRARY="../../../../3rdparty/lib/zlibstatic"'
            
    str += ' -DWITH_TESTS=OFF'
    str += ' -DWITH_UTILITIES=OFF'
    str += ' -DBUILD_OSGEO4W=OFF'
    
    my_configure(str_target , str)
    my_build(str_target)    
    
def build_gdal( str_target ):
    print ("build gdal")
    my_make_build_dir( gdal_DIR_NAME , str_target )

    str = ' -DGDAL_USE_ICONV=OFF'
    str += ' -DGDAL_USE_CURL=OFF'
    str += ' -DOGR_ENABLE_CSV=1'
    str += ' -DOGR_ENABLE_DFX=1'
    str += ' -DOGR_ENABLE_GPX=1'
    str += ' -DOGR_ENABLE_MEM=1'
    str += ' -DOGR_ENABLE_SHP=1'
    str += ' -DOGR_ENABLE_SXF=1'
    str += ' -DOGR_ENABLE_VRT=1'
    str += ' -DOGR_ENABLE_WFS=1'
    str += ' -DGDAL_ENABLE_FRMT_MEM=1'
    

    
    if(str_target == "vc"):
        if( BUILD_DYNAMIC_LINK_LIBRARY ):
            str += " -DGDAL_BUILD_SHARED=1"
            str += ' -DGDAL_BUILD_APPS=1'
            
            str += ' -DGEOTIFF_INCLUDE_DIR=../../../../3rdparty/include'
            str += ' -DGEOTIFF_LIBRARY=' + os.getcwd() + '/../../../../3rdparty/lib/geotiff_i.lib'
            str += ' -DGEOTIFF_LIBRARY_DEBUG=' + os.getcwd() + '/../../../../3rdparty/lib/geotiff_d_i.lib'
   
        else:
            str += " -DGDAL_BUILD_SHARED=0"
            str += " -DGDAL_BUILD_APPS=0"
            
            str += ' -DGEOTIFF_INCLUDE_DIR=../../../../3rdparty/include'
            str += ' -DGEOTIFF_LIBRARY=' + os.getcwd() + '/../../../../3rdparty/lib/geotiff.lib'
            str += ' -DGEOTIFF_LIBRARY_DEBUG=' + os.getcwd() + '/../../../../3rdparty/lib/geotiff_d.lib'
            str += ' -DXTIFF_LIBRARY=' + os.getcwd() + '/../../../../3rdparty/lib/xtiff.lib'
            str += ' -DXTIFF_LIBRARY_DEBUG=' + os.getcwd() + '/../../../../3rdparty/lib/xtiffd.lib'
            
        str += ' -DPROJ_INCLUDE_DIR=../../../../3rdparty/include'
        str += ' -DPROJ_LIBRARY=' + os.getcwd() + '/../../../../3rdparty/lib/proj_6_1.lib'
        str += ' -DPROJ_LIBRARY_DEBUG=' + os.getcwd() + '/../../../../3rdparty/lib/proj_6_1_d.lib'
        
        # str += ' -DSQLITE3_INCLUDE_DIR=../../../3rdparty/include/curl'
        
        str += ' -DGEOS_INCLUDE_DIR=../../../../3rdparty/include'
        str += ' -DGEOS_LIBRARY=' + os.getcwd() + '/../../../../3rdparty/lib/geos_c.lib'
        str += ' -DGEOS_LIBRARY_DEBUG=' + os.getcwd() + '/../../../../3rdparty/lib/geos_cd.lib'
        
        str += ' -DJPEG_INCLUDE_DIR=' + os.getcwd() + '../../../../3rdparty/include'
        str += ' -DJPEG_LIBRARY=' + os.getcwd() + '../../../../3rdparty/lib/jpeg.lib'
        str += ' -DJPEG_LIBRARY_DEBUG=' + os.getcwd() + '../../../../3rdparty/lib/jpegd.lib'
                
        str += ' -DSQLITE3_LIBRARY=' + os.getcwd() + '/../../../../3rdparty/lib/sqlite.lib'
        str += ' -DSQLITE3_LIBRARY_DEBUG=' + os.getcwd() + '/../../../../3rdparty/lib/sqlited.lib'
        
        str += ' -DTIFF_INCLUDE_DIR=../../../../3rdparty/include'
        str += ' -DTIFF_LIBRARY=' + os.getcwd() + '/../../../../3rdparty/lib/tiff.lib'
        str += ' -DTIFF_LIBRARY_DEBUG=' + os.getcwd() + '/../../../../3rdparty/lib/tiffd.lib'
        
        
        
        str += ' -DEXPAT_INCLUDE_DIR=../../../../3rdparty/include'
        str += ' -DEXPAT_LIBRARY=' + os.getcwd() + '/../../../../3rdparty/lib/expat.lib'
        str += ' -DEXPAT_LIBRARY_DEBUG=' + os.getcwd() + '/../../../../3rdparty/lib/expatd.lib'
        
        str += ' -DZLIB_INCLUDE_DIR=' + os.getcwd() + '/../../../../3rdparty/include'
        str += ' -DZLIB_LIBRARY=' + os.getcwd() + '/../../../../3rdparty/lib/zlib.lib'
        str += ' -DZLIB_LIBRARY_DEBUG=' + os.getcwd() + '/../../../../3rdparty/lib/zlibd.lib'
        str += ' -DZLIB_LIBRARY_RELEASE=""'
        
        # str += ' -DJSONC_INCLUDE_DIR=../../../../3rdparty/include/json-c'
        # str += ' -DJSONC_LIBRARY=' + os.getcwd() + '/../../../../3rdparty/lib/json-c.lib'
        # str += ' -DJSONC_LIBRARY_DEBUG=' + os.getcwd() + '/../../../../3rdparty/lib/json-cd.lib'

        
        #强制禁止openCL
        str += ' -DOPENCL_INCLUDE_DIRS=""'
        str += ' -DOPENCL_LIBRARIES=""'
        str += ' -D_OPENCL_CPP_INCLUDE_DIRS=""'
    
    if(str_target == "mingw"):
        if( BUILD_DYNAMIC_LINK_LIBRARY ):
            str += " -DGDAL_BUILD_SHARED=1"
            str += ' -DGDAL_BUILD_APPS=1'
        else:
            str += " -DGDAL_BUILD_SHARED=0"
            str += " -DGDAL_BUILD_APPS=0"
        
        str += ' -DPROJ_INCLUDE_DIR=../../../3rdparty/local/include'
        str += ' -DPROJ_LIBRARY=' + os.getcwd() + '/../../../3rdparty/local/lib/libproj_4_9.dll.a'
        str += ' -DPROJ_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/local/lib/libproj_4_9d.dll.a'
        
        str += ' -DGEOS_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/libgeos_c.dll.a'
        str += ' -DGEOS_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/libgeos_cd.dll.a'
        
        str += ' -DJPEG_INCLUDE_DIR=' + os.getcwd() + '../../../3rdparty/include'
        str += ' -DJPEG_LIBRARY=' + os.getcwd() + '../../../3rdparty/lib/libjpeg.dll.a'
        str += ' -DJPEG_LIBRARY_DEBUG=' + os.getcwd() + '../../../3rdparty/lib/libjpegd.dll.a'
                
        str += ' -DSQLITE3_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/libsqlite.dll.a'
        str += ' -DSQLITE3_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/libsqlited.dll.a'
        
        str += ' -DTIFF_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/libtiff.dll.a'
        str += ' -DTIFF_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/libtiffd.dll.a'
        str += ' -DGEOTIFF_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/libgeotiff.dll.a'
        str += ' -DGEOTIFF_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/libgeotiffd.dll.a'
        str += ' -DXTIFF_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/libxtiff.a'
        str += ' -DXTIFF_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/libxtiffd.a'
        
        str += ' -DEXPAT_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/libexpat.dll.a'
        str += ' -DEXPAT_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/libexpatd.dll.a'
        
        str += ' -DZLIB_INCLUDE_DIR=' + os.getcwd() + '/../../../3rdparty/include'
        str += ' -DZLIB_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/libzlib.dll.a'
        str += ' -DZLIB_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/libzlibd.dll.a'
        str += ' -DZLIB_LIBRARY_RELEASE=""'
        
        #强制禁止openCL
        str += ' -DOPENCL_INCLUDE_DIRS=""'
        str += ' -DOPENCL_LIBRARIES=""'
        str += ' -D_OPENCL_CPP_INCLUDE_DIRS=""'
        
    if(str_target == "ndk" or str_target == "cw"):
        str += ' -DGDAL_BUILD_SHARED=OFF'
        # str += ' -DCPL_DISABLE_STDCALL=1'
        str += ' -DGDAL_USE_CURL=0'
        str += ' -DGDAL_USE_CPL=0'
        str += ' -DCURL_INCLUDE_DIR=../../../'+ANDROID_ABI+'/include/curl'
        
        
        str += ' -DTIFF_INCLUDE_DIR=../../../'+ANDROID_ABI+'/include -DTIFF_LIBRARY=../../../'+ANDROID_ABI+'/lib/libtiff.a -DTIFF_LIBRARY_DEBUG=../../../'+ANDROID_ABI+'/lib/libtiff.a'
        str += ' -DJPEG_INCLUDE_DIR=../../../'+ANDROID_ABI+'/include -DJPEG_LIBRARY=../../../'+ANDROID_ABI+'/lib/libjpeg.a -DJPEG_LIBRARY_DEBUG=../../../'+ANDROID_ABI+'/lib/libjpeg.a'
        str += ' -DZLIB_INCLUDE_DIR=../../../'+ANDROID_ABI+'/include -DZLIB_LIBRARY=../../../'+ANDROID_ABI+'/lib/zlib.a -DZLIB_LIBRARY_DEBUG=../../../'+ANDROID_ABI+'/lib/zlib.a'
        str += ' -DGEOS_INCLUDE_DIR=../../../'+ANDROID_ABI+'/include -DGEOS_LIBRARY=../../../'+ANDROID_ABI+'/lib/zlib.a -DGEOS_LIBRARY_DEBUG=../../../'+ANDROID_ABI+'/lib/zlib.a'
        str += ' -DEXPAT_INCLUDE_DIR=../../../'+ANDROID_ABI+'/include -DEXPAT_LIBRARY=../../../'+ANDROID_ABI+'/lib/zlib.a -DEXPAT_LIBRARY_DEBUG=../../../'+ANDROID_ABI+'/lib/zlib.a'
        str += ' -DPROJ_INCLUDE_DIR=../../../'+ANDROID_ABI+'/include -DPROJ_LIBRARY=../../../'+ANDROID_ABI+'/lib/zlib.a -DPROJ_LIBRARY_DEBUG=../../../'+ANDROID_ABI+'/lib/zlib.a'
  
        str += ' -DSQLITE3_LIBRARY=' + os.getcwd() + '/../../../'+ANDROID_ABI+'/lib/libsqlite.a'
        str += ' -DSQLITE3_LIBRARY_DEBUG=' + os.getcwd() + '/../../../'+ANDROID_ABI+'/lib/libsqlite.a'
        
    if(str_target == "nc"):
    
        str += ' -DCMAKE_AR="${PLATFORM_PREFIX}/bin/${PLATFORM_TRIPLET}-ar"';
        str += ' -DCMAKE_RANLIB="${PLATFORM_PREFIX}/bin/${PLATFORM_TRIPLET}-ranlib"';
        
        # str += ' -Dgeotiff_DIR=../../../'+NACL_INSTALL_DIR+'/share/cmake/GeoTIFF'
        str += " -DGDAL_BUILD_SHARED=0"
        
        str += ' -DZLIB_INCLUDE_DIR=../../../'+NACL_INSTALL_DIR+'/include'
        str += ' -DZLIB_LIBRARY_RELEASE=../../../'+NACL_INSTALL_DIR+'/lib/zlib.a'
        str += ' -DTIFF_INCLUDE_DIR=../../../'+NACL_INSTALL_DIR+'/include'
        str += ' -DTIFF_LIBRARY_RELEASE=../../../'+NACL_INSTALL_DIR+'/lib/libtiff.a'
        str += ' -DJPEG_INCLUDE_DIR=../../../'+NACL_INSTALL_DIR+'/include'
        str += ' -DJPEG_LIBRARY=../../../'+NACL_INSTALL_DIR+'/lib/libjpeg.a'
        
        str += " -Dgeotiff_LIBRARY=../../../3rdparty/lib/libgeotiffd.lib"
        str += " -Dxtiff_LIBRARY=../../../3rdparty/lib/libxtiffd.lib"
        
        str += ' -DGEOS_INCLUDE_DIR=../../../'+NACL_INSTALL_DIR+'/include'
        str += ' -DGEOS_LIBRARY_RELEASE=../../../'+NACL_INSTALL_DIR+'/lib/libgeos.a'
        
        
    my_configure(str_target , str,True)
    my_build(str_target)  

def build_boost( str_target ):
    print ("build boost")
    my_make_build_dir( boost_DIR_NAME , str_target )
    
    str = ' -DWITH_BZIP2=0 -DWITH_DOXYGEN=0 -DWITH_EXPAT=0 -DWITH_ICU=0 -DWITH_MPI=0 -DWITH_PYTHON=0 -DWITH_VALGRIND=0 -DWITH_XSLTPROC=0 -DWITH_ZLIB=0 -DWINMANGLE_LIBNAMES=0 '
    
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str += ' -DENABLE_SHARED=1 -DENABLE_STATIC=0 -DENABLE_STATIC_RUNTIME=0'
    else:
        str += ' -DENABLE_SHARED=0 -DENABLE_STATIC=1 -DENABLE_STATIC_RUNTIME=0'
    
    if(str_target == "vc"):
        my_configure(str_target ,str)
    elif(str_target == "mingw"):
        my_configure(str_target ,str)
    elif(str_target == "ndk"):
        my_configure(str_target ,str + ' -DINSTALL_VERSIONED=0 ',True)
        
    my_build(str_target , True)
    
def build_dae( str_target ):
    print ("build dae")
    #修改 COLLADA_COMPILE_FLAGS 取消dll相关 增加 LIBXML_READER_ENABLED LIBXML_OUTPUT_ENABLED  
    #修改add_library为static
    my_make_build_dir( dae_DIR_NAME , str_target )
    
    str = ''
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str += ' -DBUILD_SHARED_LIBS=1'
    else:
        str += ' -DBUILD_SHARED_LIBS=0'
        
    if(str_target == "vc"):
        my_configure(str_target ,str + ' -DBoost_USE_STATIC_LIBS=1 -DBOOST_ROOT="%(1)s/../../../3rdparty/" -DBoost_DIR="%(1)s/../../../3rdparty/include/" -DBoost_INCLUDE_DIR="%(1)s/../../../3rdparty/include/" -DBOOST_LIBRARYDIR="%(1)s/../../../3rdparty/lib/" -DLIBXML2_INCLUDE_DIR="%(1)s/../../../3rdparty/include/" -DBoost_SYSTEM_LIBRARY_DEBUG="%(1)s/../../../3rdparty/lib/libboost_system-mt-gd.lib" -DBoost_SYSTEM_LIBRARY_RELEASE="%(1)s/../../../3rdparty/lib/libboost_system-mt.lib" -DBoost_FILESYSTEM_LIBRARY_DEBUG="%(1)s/../../../3rdparty/lib/libboost_filesystem-mt-gd.lib" -DBoost_FILESYSTEM_LIBRARY_RELEASE="%(1)s/../../../3rdparty/lib/libboost_filesystem-mt.lib" ' % {"1": os.getcwd()} )
    elif(str_target == "mingw"):
        my_configure(str_target ,str + ' -DBoost_USE_STATIC_LIBS=1 -DBOOST_ROOT="%(1)s/../../../3rdparty/" -DBoost_DIR="%(1)s/../../../3rdparty/include/" -DBoost_INCLUDE_DIR="%(1)s/../../../3rdparty/include/" -DBOOST_LIBRARYDIR="%(1)s/../../../3rdparty/lib/" -DLIBXML2_INCLUDE_DIR="%(1)s/../../../3rdparty/include/" -DBoost_SYSTEM_LIBRARY_DEBUG="%(1)s/../../../3rdparty/lib/libboost_system-mt-gd.lib" -DBoost_SYSTEM_LIBRARY_RELEASE="%(1)s/../../../3rdparty/lib/libboost_system-mt.lib" -DBoost_FILESYSTEM_LIBRARY_DEBUG="%(1)s/../../../3rdparty/lib/libboost_filesystem-mt-gd.lib" -DBoost_FILESYSTEM_LIBRARY_RELEASE="%(1)s/../../../3rdparty/lib/libboost_filesystem-mt.lib" ' % {"1": os.getcwd()} )
    elif(str_target == "ndk"):
        my_configure(str_target , ' -DBoost_USE_STATIC_LIBS=1 -DBOOST_ROOT="%(1)s/../../../armeabi-v7a/" -DBoost_DIR="%(1)s/../../../armeabi-v7a/include/" -DBoost_INCLUDE_DIR="%(1)s/../../../armeabi-v7a/include/" -DBOOST_LIBRARYDIR="%(1)s/../../../armeabi-v7a/lib/"  -DBoost_CFLAGS=""  -DLIBXML2_INCLUDE_DIR="%(1)s/../../../armeabi-v7a/include/" -DLIBXML2_LIBRARIES="%(1)s/../../../armeabi-v7a/lib/libxml.a" -DBoost_SYSTEM_LIBRARY_DEBUG="%(1)s/../../../armeabi-v7a/lib/libboost_system-vc-mt-d-1_41.a" -DBoost_SYSTEM_LIBRARY_RELEASE="%(1)s/../../../armeabi-v7a/lib/libboost_system-vc-mt-1_41.a" -DBoost_FILESYSTEM_LIBRARY_DEBUG="%(1)s/../../../armeabi-v7a/lib/libboost_filesystem-vc-mt-d-1_41.a" -DBoost_FILESYSTEM_LIBRARY_RELEASE="%(1)s/../../../armeabi-v7a/lib/libboost_filesystem-vc-mt-1_41.a" ' % {"1": os.getcwd()} )
    my_build(str_target)

def build_libxml2( str_target ):
    print ("build libxml2")
    # 完全没法支持dll编译
    my_make_build_dir( libxml2_DIR_NAME , str_target )
    
    str = ''
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str += ' -DLIBXML_BUILD_STATIC=0'
    else:
        str += ' -DLIBXML_BUILD_STATIC=1'
        
    my_configure(str_target ,str)
    my_build(str_target)

    
ONLY_BUILD = ""
def only( str_build ):
    if ONLY_BUILD == "":
        return True
    if ONLY_BUILD == str_build:
        return True
    return False
    

#main    
def main():

    global ONLY_BUILD
    ALL_TARGET = "vc" #默认vc
    if len(sys.argv) >= 2:
        ALL_TARGET = sys.argv[1]
    if len(sys.argv) >= 3:
        ONLY_BUILD = sys.argv[2]
    print( ALL_TARGET )

    #独立工具
    # [cmake] crashrpt
    if only("crashrpt"):
        build_crashrpt( ALL_TARGET )

    #无依赖
    # [cmake] gtest
    if only("gtest"):
        build_gtest( ALL_TARGET )
    # [cmake] crunch
    if only("crunch"):
        build_crunch( ALL_TARGET )
    # [cmake] glew
    if only("glew"):
        build_glew( ALL_TARGET )
    # [cmake] stb
    if only("stb"):
        build_stb( ALL_TARGET )
    # [cmake] imgui
    if only("imgui"):
        build_imgui( ALL_TARGET )
    # [cmake] eigen
    if only("eigen"):
        build_eigen( ALL_TARGET )
    # [cmake] draco
    if only("draco"):
        build_draco( ALL_TARGET )        
    # [cmake] jsoncpp
    if only("jsoncpp"):
        build_jsoncpp( ALL_TARGET )        
    # [cmake] qhull
    if only("qhull"):
        build_qhull( ALL_TARGET )    
    # [cmake] rapidxml
    if only("rapidxml"):
        build_rapidxml( ALL_TARGET )
    # [cmake] flann : HDF5(VTK)
    # if only("flann"):
        # build_flann( ALL_TARGET )
        
    # [cmake] pthreads_win32
    if only("pthreads") and ALL_TARGET != "ndk" and ALL_TARGET != "nc" and ALL_TARGET != "mingw":
        build_pthreads_win32( ALL_TARGET )
    # [cmake]giflib
    if only("giflib") and ALL_TARGET != "ndk":
        build_giflib( ALL_TARGET )       #永远是static修改 #ndk未通过
    # [cmake]freeglut
    if only("freeglut") and ALL_TARGET != "nc" and ALL_TARGET != "ndk":
        build_freeglut( ALL_TARGET )    #ndk未通过 nc未通过 
    # [cmake]zlib 
    if only("zlib"):
        build_zlib( ALL_TARGET )
    # [cmake]jpeglib-9 
    if only("jpeg"):
        build_jpeg( ALL_TARGET )
    # [cmake]freetype 
    if only("freetype"):
        build_freetype(ALL_TARGET)
    # [cmake]lua
    if only("lua"):
        build_lua( ALL_TARGET )
    # [cmake]libxml2
    if only("libxml2"):
        build_libxml2( ALL_TARGET )     #永远是static修改 #ndk未通过
    # [cmake]leveldb
    if only("leveldb") and ALL_TARGET != "nc" and ALL_TARGET != "nc":
        build_leveldb( ALL_TARGET )     #默认双输出static shared #ndk未通过 #nc未通过 
    # [cmake]sqlite3
    if only("sqlite3") and ALL_TARGET != "nc":
        build_sqlite3( ALL_TARGET )    #永远是static修改 nc未通过 头文件和函数的问题
    # [cmake]geos
    if only("geos"):
        build_geos( ALL_TARGET )       #默认双输出static shared
    # [cmake]proj4
    if only("proj4"):
        build_proj4( ALL_TARGET )
    # [cmake]expat
    if only("expat") and ALL_TARGET != "nc":
        build_expat( ALL_TARGET )          #nc未通过
    
    
    
    #一级依赖
    # [cmake] jasper : jpeg
    if only("jasper"):
        build_jasper( ALL_TARGET )
    # [cmake]libpng : zlib
    if only("libpng"):
        build_png( ALL_TARGET )
    # [cmake]openexr : zlib
    if only("openexr") and ALL_TARGET != "nc":
        build_openexr( ALL_TARGET )    #ndk未通过 #nc未通过
    # [cmake]libtiff : zlib jpeglib
    if only("libtiff"):
        build_tiff( ALL_TARGET )        #永远是static修改
    # [cmake]libCURL : zlib
    if only("curl"):
        build_curl( ALL_TARGET )
    # [cmake]collada-DOM pcre mimizip : zlib boost-cmake libxml2
    # if only("dae") and ALL_TARGET != "nc":
        # build_dae( ALL_TARGET )       #ndk未通过 因为boost #nc未通过
    
    #二级依赖
    # [cmake]geotiff : zlib proj4 jpeg tiff
    if only("geotiff"):
        build_geotiff( ALL_TARGET )
    # [cmake]gdal : zlib geos proj4 curl jpeg tiff
    if only("gdal"):
        build_gdal( ALL_TARGET )
    
    #三级依赖
    # [cmake]liblas : Boost geotiff jpeg tiff zlib
    # if only("liblas") and ALL_TARGET != "ndk" and ALL_TARGET != "nc":
        # build_liblas( ALL_TARGET ) #ndk未通过
    # [cmake]nvidia-texture-tools : zlib png tiff openexr jpeg
    if only("nvtt") and ALL_TARGET != "ndk" and ALL_TARGET != "nc" and ALL_TARGET != "mingw":
        build_nvtt( ALL_TARGET )


main()
