#!/usr/bin/python

import os
import sys
import string
import subprocess
from common import *


#    
#
#

    
def build_project( str_target , str_project_name ):
    print "build " + str_project_name
    my_make_build_dir( "../projects/"+str_project_name , str_target )
    
    str_ops = ""
    
    if( str_target == "vc" ):
    
        if(VC_PPAPI_MODE):
            str_ops += " -DPPAPI=1";
            # str_ops += " -DBUILD_OSG_APPLICATIONS=0";
            # str_ops += " -DBUILD_OSG_EXAMPLES=0"
            # str_ops += " -DOPENGL_PROFILE=\"GLES2\""
            # str_ops += " -DOSG_GL_LIBRARY_STATIC=ON"
            # str_ops += ' -DOSG_WINDOWING_SYSTEM="none"'
            str_ops += ' -DOPENGL_INCLUDE_DIR=' + NACL_ROOT +'/'+ NACL_PEPPER_DIR+'/include'
            # str_ops += ' -DOPENGL_gl_LIBRARY=' + NACL_ROOT +'/'+ NACL_PEPPER_DIR+'/lib/win_x86_32_host/Release/ppapi_gles2.lib'
            str_ops += ' -DOPENGL_gl_LIBRARY=' + NACL_GLES_LIB
            
        if( BUILD_DYNAMIC_LINK_LIBRARY ):   
            str_ops += ' -DWOSG_USE_STATIC_LIBRARY=0'
            
            str_ops += ' -DZLIB_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/zlib.lib'
            str_ops += ' -DZLIB_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/zlibd.lib'
                
            str_ops += ' -DPROJ_INCLUDE_DIR=' + os.getcwd() + '/../../../3rdparty/include'
            str_ops += ' -DPROJ_LIBRARY=' + os.getcwd() + '/../../../3rdparty/local/lib/proj_4_9.lib'
            str_ops += ' -DPROJ_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/local/lib/proj_4_9_d.lib'
            
            str_ops += ' -DGEOS_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/libgeos.lib'
            str_ops += ' -DGEOS_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/libgeosd.lib'
            str_ops += ' -DGEOS_C_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/geos_c.lib'
            str_ops += ' -DGEOS_C_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/geos_cd.lib'
            
            str_ops += ' -DGDAL_INCLUDE_DIR=' + os.getcwd() + '/../../../3rdparty/include'
            str_ops += ' -DGDAL_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/gdal111.lib'
            str_ops += ' -DGDAL_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/gdal111d.lib'
            
            str_ops += ' -DGEOTIFF_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/geotiff_i.lib'
            str_ops += ' -DGEOTIFF_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/geotiff_d_i.lib'
            
            str_ops += ' -DSQLITE3_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/sqlite.lib'
            str_ops += ' -DSQLITE3_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/sqlited.lib'
            
            str_ops += ' -DPNG_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/libpng16.lib'
            str_ops += ' -DPNG_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/libpng16d.lib'
            
            str_ops += ' -DCURL_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/libcurl_imp.lib'
            str_ops += ' -DCURL_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/libcurld_imp.lib'
            
        else:
            str_ops += " -DWOSG_USE_STATIC_LIBRARY=1";
            
            # str_ops += ' -DZLIB_LIBRARY_RELEASE:PATH=""'
            str_ops += ' -DZLIB_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/zlibstatic.lib'
            str_ops += ' -DZLIB_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/zlibstaticd.lib'

            str_ops += ' -DPROJ_INCLUDE_DIR=' + os.getcwd() + '/../../../3rdparty/include'
            str_ops += ' -DPROJ_LIBRARY=' + os.getcwd() + '/../../../3rdparty/local/lib/proj_4_9.lib'
            str_ops += ' -DPROJ_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/local/lib/proj_4_9_d.lib'
            
            str_ops += ' -DGEOS_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/libgeos.lib'
            str_ops += ' -DGEOS_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/libgeosd.lib'
            str_ops += ' -DGEOS_C_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/geos_c.lib'
            str_ops += ' -DGEOS_C_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/geos_cd.lib'
            
            str_ops += ' -DGDAL_INCLUDE_DIR=' + os.getcwd() + '/../../../3rdparty/include'
            str_ops += ' -DGDAL_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/gdal111.lib'
            str_ops += ' -DGDAL_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/gdal111d.lib'
            
            str_ops += ' -DGEOTIFF_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/geotiff.lib'
            str_ops += ' -DGEOTIFF_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/geotiff_d.lib'
            
            str_ops += ' -DTIFF_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/tiff.lib'
            str_ops += ' -DTIFF_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/tiffd.lib'

            
            str_ops += ' -DXTIFF_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/xtiff.lib'
            str_ops += ' -DXTIFF_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/xtiffd.lib'
            
            str_ops += ' -DSQLITE3_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/sqlite.lib'
            str_ops += ' -DSQLITE3_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/sqlited.lib'
            
            str_ops += ' -DLIBGIF_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/giflib.lib'
            str_ops += ' -DLIBGIF_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/giflibd.lib'   

            str_ops += ' -DLIBXML_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/xml2.lib'
            str_ops += ' -DLIBXML_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/xml2d.lib'    

            str_ops += ' -DEXPAT_LIBRARY=' + os.getcwd() + '/../../../3rdparty/lib/expat.lib'
            str_ops += ' -DEXPAT_LIBRARY_DEBUG=' + os.getcwd() + '/../../../3rdparty/lib/expatd.lib'
            
            str_ops += ' -DCURL_LIBRARY="../../../3rdparty/lib/libcurl.lib"'
            str_ops += ' -DCURL_LIBRARY_DEBUG="../../../3rdparty/lib/libcurld.lib"'
            
            
    if( str_target == "ndk" or str_target == "cw" ):
        
        # str_ops += " -DWOSG_INCLUDE_DIR=../../../"+ANDROID_ABI+"/include"
        # str_ops += " -DWOSG_INCLUDE_STATIC_BUILD_DIR=../../../"+ANDROID_ABI+"/include"
        # str_ops += " -DWOSG_INCLUDE_DYNAMIC_BUILD_DIR=../../../"+ANDROID_ABI+"/include"
        
        # str_ops += " -DOSG_DIR=../../../"+ANDROID_ABI+"/"
        
        str_ops += " -DOPENTHREADS_INCLUDE_DIR=../../../"+ANDROID_ABI+"/include"
        str_ops += " -DOPENTHREADS_LIBRARY=../../../"+ANDROID_ABI+"/lib/libOpenThreads.a"
        
        str_ops += " -DOSG_INCLUDE_DIR=../../../"+ANDROID_ABI+"/include"
        str_ops += " -DOSG_LIBRARY=../../../"+ANDROID_ABI+"/lib/libosg.a"
        str_ops += " -DOSGDB_INCLUDE_DIR=../../../"+ANDROID_ABI+"/include"
        str_ops += " -DOSGDB_LIBRARY=../../../"+ANDROID_ABI+"/lib/libosgDB.a"
        str_ops += " -DOSGGA_INCLUDE_DIR=../../../"+ANDROID_ABI+"/include"
        str_ops += " -DOSGGA_LIBRARY=../../../"+ANDROID_ABI+"/lib/libosgGA.a"
        str_ops += " -DOSGUTIL_INCLUDE_DIR=../../../"+ANDROID_ABI+"/include"
        str_ops += " -DOSGUTIL_LIBRARY=../../../"+ANDROID_ABI+"/lib/libosgUtil.a"
        str_ops += " -DOSGVIEWER_INCLUDE_DIR=../../../"+ANDROID_ABI+"/include"
        str_ops += " -DOSGVIEWER_LIBRARY=../../../"+ANDROID_ABI+"/lib/libosgViewer.a"
        
        str_ops += " -DOSGTEXT_INCLUDE_DIR=../../../"+ANDROID_ABI+"/include"
        str_ops += " -DOSGTEXT_LIBRARY=../../../"+ANDROID_ABI+"/lib/libosgText.a"
        str_ops += " -DOSGANIMATION_INCLUDE_DIR=../../../"+ANDROID_ABI+"/include"
        str_ops += " -DOSGANIMATION_LIBRARY=../../../"+ANDROID_ABI+"/lib/libosgAnimation.a"
        
        
        # str_ops += " -DMAKE_ANDROID_API="+str(ANDROID_API_LEVEL)
        # str_ops += " -DCMAKE_ANDROID_API_MIN="+str(ANDROID_API_LEVEL)
        str_ops += " -DCMAKE_ANDROID_GUI=1"
        
        
    my_configure(str_target , str_ops )
    my_build(str_target)
    
#main    
def main():

    ALL_TARGET = "vc" # default vc
    PROJECT_NAME = ""
    if len(sys.argv) >= 2:
        ALL_TARGET = sys.argv[1]
        PROJECT_NAME = sys.argv[2]
    print( ALL_TARGET +" >> "+ PROJECT_NAME )

    build_project( ALL_TARGET , PROJECT_NAME )

main()
