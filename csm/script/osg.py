#!/usr/bin/python
# -*- coding: utf-8 -*-

from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("nvtt" , list_name,getDependency)
    list_name = addDependency("openexr" , list_name,getDependency)
    list_name = addDependency("gdal" , list_name,getDependency)
    list_name = addDependency("libpng" , list_name,getDependency)
    list_name = addDependency("freetype" , list_name,getDependency)
    list_name = addDependency("jasper" , list_name,getDependency)
    list_name = addDependency("giflib" , list_name,getDependency)
    list_name = addDependency("liblas" , list_name,getDependency)

    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):

    # if(b_only_download):
        # download_source(str_name,"https://github.com/openscenegraph/OpenSceneGraph.git")
        # download_source(str_name,"https://github.com/shelltdf/OpenSceneGraph.git","OpenSceneGraph-3.4")
        
        # return
    
    dir_name = my_build_and_install_dir(dict_config)
    install_dir = os.getcwd() +"/install/"+ my_build_and_install_dir(dict_config)
    
    STR_CFG = ''
    
    
    if(dict_config['arch'][:2]=="vs"):
        STR_CFG += ' -DOSG_USE_UTF8_FILENAME=1'
        
        STR_CFG += " -DGDAL_INCLUDE_DIR='" + install_dir + "/include/gdal'"
        STR_CFG += " -DOPENEXR_INCLUDE_DIR='" + install_dir + "/include'"
        STR_CFG += " -DGIFLIB_INCLUDE_DIR='" + install_dir + "/include'"
    
        # STR_CFG += ' -DNVTT_SHARED=1'
        
        if(dict_config['release']):
            STR_CFG += " -DGDAL_LIBRARY='" + install_dir + "/lib/gdal31.lib'"
            STR_CFG += " -DGIFLIB_LIBRARY='" + install_dir + "/lib/giflib.lib'"
            STR_CFG += " -DLIBLAS_LIBRARY='" + install_dir + "/lib/liblas.lib'"
        else:
            STR_CFG += " -DGDAL_LIBRARY='" + install_dir + "/lib/gdal31d.lib'"
            # STR_CFG += " -DCURL_LIBRARY_RELEASE='" + install_dir + "/lib/libcurld_imp.lib'"
            STR_CFG += " -DCURL_LIBRARY_DEBUG='" + install_dir + "/lib/libcurld_imp.lib'"
            STR_CFG += " -DNVSQUISH_LIBRARY_DEBUG='" + install_dir + "/lib/squishd.lib'"
            STR_CFG += " -DNVTT_LIBRARY_DEBUG='" + install_dir + "/lib/nvttd.lib'"
            STR_CFG += " -DGIFLIB_LIBRARY='" + install_dir + "/lib/giflibd.lib'"
            STR_CFG += " -DLIBLAS_LIBRARY='" + install_dir + "/lib/liblasd.lib'"
    
    if(dict_config['arch']=="em"):
        # STR_CFG += ' -DOSG_USE_UTF8_FILENAME=1'
        
        STR_CFG += ' -DOPENGL_PROFILE=GLES2'
        STR_CFG += ' -DBUILD_OSG_APPLICATIONS=0'
        STR_CFG += ' -DBUILD_OSG_EXAMPLES=0'
        
        STR_CFG += ' -DDYNAMIC_OPENSCENEGRAPH=0'
        STR_CFG += ' -DDYNAMIC_OPENTHREADS=0'
        STR_CFG += ' -DOSG_GL_LIBRARY_STATIC=1'
        STR_CFG += ' -DEGL_LIBRARY=egl' #这里应该无所谓是什么，只要赋值就行了。
    
    
    source_dir = os.getcwd() + '/../prebuild/OpenSceneGraph-3.6.5'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    