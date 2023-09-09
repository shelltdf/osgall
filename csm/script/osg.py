#!/usr/bin/python
# -*- coding: utf-8 -*-

from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("nvtt" , list_name,getDependency) #no support ndk
    list_name = addDependency("openexr" , list_name,getDependency) #no support ndk
    list_name = addDependency("gdal" , list_name,getDependency)
    list_name = addDependency("libpng" , list_name,getDependency)
    list_name = addDependency("freetype" , list_name,getDependency)
    list_name = addDependency("jasper" , list_name,getDependency)
    list_name = addDependency("giflib" , list_name,getDependency)
    list_name = addDependency("libtiff" , list_name,getDependency)
    list_name = addDependency("lua" , list_name,getDependency)
    list_name = addDependency("liblas" , list_name,getDependency) #no support ndk
    list_name = addDependency("gl3w" , list_name,getDependency) # support glcore
    list_name = addDependency("dcmtk" , list_name,getDependency)
    list_name = addDependency("itk" , list_name,getDependency)
    list_name = addDependency("occt" , list_name,getDependency)
    # list_name = addDependency("collada" , list_name,getDependency)

    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):

    # if(b_only_download):
        # download_source(str_name,"https://github.com/openscenegraph/OpenSceneGraph.git")
        # download_source(str_name,"https://github.com/shelltdf/OpenSceneGraph.git","OpenSceneGraph-3.4")
        
        # return
    
    dir_name = my_build_and_install_dir(dict_config)
    install_dir = dict_config['install_dir'] + '/' + my_build_and_install_dir(dict_config)
    
    STR_CFG = ''
    
    STR_CFG += ' -DUSE_OSG_PATCH=0'
    
    STR_CFG += ' -DOSG_USE_LOCAL_LUA_SOURCE=0'
    STR_CFG += ' -DOSG_CPP_EXCEPTIONS_AVAILABLE=1'
    
    if(dict_config['static']):
        STR_CFG += ' -DOPENCASCADE_LIBRARY_TYPE=Static'
    else:
        STR_CFG += ' -DOPENCASCADE_LIBRARY_TYPE=Shared'
            
    if( GLES_VER == 1 ):
        STR_CFG += " -DOPENGL_PROFILE=\"GLES1\""
    if( GLES_VER == 2 ):
        STR_CFG += " -DOPENGL_PROFILE=\"GLES2\""
    if( GLES_VER == 3 ):
        STR_CFG += " -DOPENGL_PROFILE=\"GLES3\""
        
    if( GL_VER == 1 ):
        STR_CFG += " -DOPENGL_PROFILE=\"GL1\""   
    if( GL_VER == 2 ):
        STR_CFG += " -DOPENGL_PROFILE=\"GL2\""           
    if( GL_VER == 3 ):
        STR_CFG += " -DOPENGL_PROFILE=\"GLCORE\""   
        # STR_CFG += ' -DGLCORE_ROOT=' + os.getcwd() + '/../../../3rdparty/include'   
        STR_CFG += ' -DGLCORE_INCLUDE_DIR=' + install_dir + '/include/gl3w'   
        
    if(dict_config['arch'][:2]=="vs"):
        STR_CFG += ' -DOSG_USE_UTF8_FILENAME=1'
        
        # STR_CFG += ' -DOPENTHREADS_ATOMIC_USE_MUTEX=1'
        
        STR_CFG += " -DGDAL_INCLUDE_DIR='" + install_dir + "/include/gdal'"
        STR_CFG += " -DOPENEXR_INCLUDE_DIR='" + install_dir + "/include'"
        STR_CFG += " -DGIFLIB_INCLUDE_DIR='" + install_dir + "/include'"
        STR_CFG += " -DLUA_INCLUDE_DIR='" + install_dir + "/include'"
        
        # STR_CFG += ' -DNVTT_SHARED=1'
        
        if(dict_config['static']):
            STR_CFG += ' -DBUILD_OSG_APPLICATIONS=0'
            STR_CFG += ' -DBUILD_OSG_EXAMPLES=0'
            
            STR_CFG += ' -DDYNAMIC_OPENSCENEGRAPH=0'
            STR_CFG += ' -DDYNAMIC_OPENTHREADS=0'
            STR_CFG += ' -DOSG_GL_LIBRARY_STATIC=0'
        else:
            STR_CFG += ' -DBUILD_OSG_APPLICATIONS=1'
            STR_CFG += ' -DBUILD_OSG_EXAMPLES=1'
            
            STR_CFG += ' -DDYNAMIC_OPENSCENEGRAPH=1'
            STR_CFG += ' -DDYNAMIC_OPENTHREADS=1'
            STR_CFG += ' -DOSG_GL_LIBRARY_STATIC=0'
            
        if(dict_config['release'] or dict_config['debuginfo']):
            STR_CFG += " -DGDAL_LIBRARY='" + install_dir + "/lib/gdal31.lib'"
            STR_CFG += " -DGIFLIB_LIBRARY='" + install_dir + "/lib/giflib.lib'"
            STR_CFG += " -DLIBLAS_LIBRARY='" + install_dir + "/lib/liblas.lib'"
            STR_CFG += " -DLUA_LIBRARY='" + install_dir + "/lib/lua.lib'"
            
            STR_CFG += " -DOPENEXR_IlmImf_LIBRARY='" + install_dir + "/lib/IlmImf-2_5.lib'"
            STR_CFG += " -DILMBASE_Half_LIBRARY='" + install_dir + "/lib/Half-2_5.lib'"
            STR_CFG += " -DILMBASE_Iex_LIBRARY='" + install_dir + "/lib/Iex-2_5.lib'"
            STR_CFG += " -DILMBASE_IlmThread_LIBRARY='" + install_dir + "/lib/IlmThread-2_5.lib'"
            
        if(dict_config['debug']):
            STR_CFG += " -DGDAL_LIBRARY='" + install_dir + "/lib/gdal31d.lib'"
            # STR_CFG += " -DCURL_LIBRARY_RELEASE='" + install_dir + "/lib/libcurld_imp.lib'"
            STR_CFG += " -DCURL_LIBRARY_DEBUG='" + install_dir + "/lib/libcurld_imp.lib'"
            STR_CFG += " -DNVSQUISH_LIBRARY_DEBUG='" + install_dir + "/lib/squishd.lib'"
            STR_CFG += " -DNVTT_LIBRARY_DEBUG='" + install_dir + "/lib/nvttd.lib'"
            STR_CFG += " -DGIFLIB_LIBRARY='" + install_dir + "/lib/giflibd.lib'"
            STR_CFG += " -DLIBLAS_LIBRARY='" + install_dir + "/lib/liblasd.lib'"
            STR_CFG += " -DLUA_LIBRARY='" + install_dir + "/lib/luad.lib'"
            
            STR_CFG += " -DOPENEXR_IlmImf_LIBRARY_DEBUG='" + install_dir + "/lib/IlmImf-2_5_d.lib'"
            STR_CFG += " -DILMBASE_Half_LIBRARY_DEBUG='" + install_dir + "/lib/Half-2_5_d.lib'"
            STR_CFG += " -DILMBASE_Iex_LIBRARY_DEBUG='" + install_dir + "/lib/Iex-2_5_d.lib'"
            STR_CFG += " -DILMBASE_IlmThread_LIBRARY_DEBUG='" + install_dir + "/lib/IlmThread-2_5_d.lib'"

    if(dict_config['arch']=="unix"):
        STR_CFG += ' -DOSG_USE_UTF8_FILENAME=0'
        
        STR_CFG += " -DGDAL_INCLUDE_DIR='" + install_dir + "/include/gdal'"
        STR_CFG += " -DOPENEXR_INCLUDE_DIR='" + install_dir + "/include'"
        STR_CFG += " -DGIFLIB_INCLUDE_DIR='" + install_dir + "/include'"
        STR_CFG += " -DLUA_INCLUDE_DIR='" + install_dir + "/include'"
        
        # STR_CFG += ' -DNVTT_SHARED=1'
        
        if(dict_config['static']):
            STR_CFG += ' -DBUILD_OSG_APPLICATIONS=0'
            STR_CFG += ' -DBUILD_OSG_EXAMPLES=0'
            
            STR_CFG += ' -DDYNAMIC_OPENSCENEGRAPH=0'
            STR_CFG += ' -DDYNAMIC_OPENTHREADS=0'
            STR_CFG += ' -DOSG_GL_LIBRARY_STATIC=0'
        else:
            STR_CFG += ' -DBUILD_OSG_APPLICATIONS=1'
            STR_CFG += ' -DBUILD_OSG_EXAMPLES=0'
            
            STR_CFG += ' -DDYNAMIC_OPENSCENEGRAPH=1'
            STR_CFG += ' -DDYNAMIC_OPENTHREADS=1'
            STR_CFG += ' -DOSG_GL_LIBRARY_STATIC=0'
            
        if(dict_config['release'] or dict_config['debuginfo']):
            STR_CFG += " -DGDAL_LIBRARY='" + install_dir + "/lib/libgdal31.so'"
            STR_CFG += " -DGIFLIB_LIBRARY='" + install_dir + "/lib/libgiflib.so'"
            STR_CFG += " -DLIBLAS_LIBRARY='" + install_dir + "/lib/liblas.so'"

            STR_CFG += " -DFREETYPE_INCLUDE_DIR_freetype2='" + install_dir + "/include/freetype2'"
            STR_CFG += " -DFREETYPE_INCLUDE_DIR_ft2build='" + install_dir + "/include/freetype2'"
            STR_CFG += " -DFREETYPE_LIBRARY_RELEASE='" + install_dir + "/lib/libfreetype.so'"

            STR_CFG += " -DZLIB_INCLUDE_DIR='" + install_dir + "/include'"
            STR_CFG += " -DZLIB_LIBRARY_RELEASE='" + install_dir + "/lib/libz.so'"

            STR_CFG += " -DPNG_PNG_INCLUDE_DIR='" + install_dir + "/include'"
            STR_CFG += " -DPNG_LIBRARY_RELEASE='" + install_dir + "/lib/libpng.so'"

            STR_CFG += " -DBoost_DIR=''"
            STR_CFG += " -DBoost_INCLUDE_DIR=''"

        if(dict_config['debug']):
            STR_CFG += " -DGDAL_LIBRARY='" + install_dir + "/lib/libgdal31.so'"
            # STR_CFG += " -DCURL_LIBRARY_RELEASE='" + install_dir + "/lib/libcurld_imp.lib'"
            STR_CFG += " -DCURL_LIBRARY_DEBUG='" + install_dir + "/lib/libcurld.so'"
            #STR_CFG += " -DNVSQUISH_LIBRARY_DEBUG='" + install_dir + "/lib/squishd.so'"
            #STR_CFG += " -DNVTT_LIBRARY_DEBUG='" + install_dir + "/lib/nvttd.so'"
            STR_CFG += " -DGIFLIB_LIBRARY='" + install_dir + "/lib/libgiflibd.so'"
            STR_CFG += " -DLIBLAS_LIBRARY='" + install_dir + "/lib/liblasd.so'"

    if(dict_config['arch']=="ndk"):
    
        STR_CFG += " -DOPENGL_PROFILE=\"GLES1\""
        STR_CFG += " -DCMAKE_INSTALL_COMPONENT=0"
        
        STR_CFG += ' -DOSG_USE_UTF8_FILENAME=0'
        
        STR_CFG += " -DGDAL_INCLUDE_DIR='" + install_dir + "/include/gdal'"
        STR_CFG += " -DOPENEXR_INCLUDE_DIR='" + install_dir + "/include'"
        STR_CFG += " -DGIFLIB_INCLUDE_DIR='" + install_dir + "/include'"
        STR_CFG += " -DLUA_INCLUDE_DIR='" + install_dir + "/include'"
        
        # STR_CFG += ' -DNVTT_SHARED=1'
        
        if(dict_config['static']):
            STR_CFG += ' -DBUILD_OSG_APPLICATIONS=0'
            STR_CFG += ' -DBUILD_OSG_EXAMPLES=0'
            
            STR_CFG += ' -DDYNAMIC_OPENSCENEGRAPH=0'
            STR_CFG += ' -DDYNAMIC_OPENTHREADS=0'
            STR_CFG += ' -DOSG_GL_LIBRARY_STATIC=0'
            
            if(dict_config['release'] or dict_config['debuginfo']):
                STR_CFG += " -DGDAL_LIBRARY='" + install_dir + "/lib/libgdal31.a'"
                
                STR_CFG += " -DGIFLIB_LIBRARY='" + install_dir + "/lib/libgiflib.a'"
                
                # STR_CFG += " -DLIBLAS_LIBRARY='" + install_dir + "/lib/liblas.a'"

                STR_CFG += " -DFREETYPE_INCLUDE_DIR_freetype2='" + install_dir + "/include/freetype2'"
                STR_CFG += " -DFREETYPE_INCLUDE_DIR_ft2build='" + install_dir + "/include/freetype2'"
                STR_CFG += " -DFREETYPE_LIBRARY_RELEASE='" + install_dir + "/lib/libfreetype.a'"

                STR_CFG += " -DZLIB_INCLUDE_DIR='" + install_dir + "/include'"
                STR_CFG += " -DZLIB_LIBRARY_RELEASE='" + install_dir + "/lib/libz.a'"

                STR_CFG += " -DPNG_PNG_INCLUDE_DIR='" + install_dir + "/include'"
                STR_CFG += " -DPNG_LIBRARY_RELEASE='" + install_dir + "/lib/libpng16.a'"
                
                STR_CFG += " -DJASPER_INCLUDE_DIR='" + install_dir + "/include'"
                STR_CFG += " -DJASPER_LIBRARY_RELEASE='" + install_dir + "/lib/jasper.a'"
                
                STR_CFG += " -DJPEG_INCLUDE_DIR='" + install_dir + "/include'"
                STR_CFG += " -DJPEG_LIBRARY_RELEASE='" + install_dir + "/lib/jpeg.a'"
                
                STR_CFG += " -DCURL_INCLUDE_DIR='" + install_dir + "/include/curl'"
                STR_CFG += " -DCURL_LIBRARY_RELEASE='" + install_dir + "/lib/libcurl.a'"
                
                STR_CFG += " -DTIFF_INCLUDE_DIR='" + install_dir + "/include'"
                STR_CFG += " -DTIFF_LIBRARY_RELEASE='" + install_dir + "/lib/libtiff.a'"
                
                # STR_CFG += " -DNVSQUISH_LIBRARY_DEBUG='" + install_dir + "/lib/squishd.a'"
                # STR_CFG += " -DNVTT_LIBRARY_DEBUG='" + install_dir + "/lib/nvttd.a'"

                # STR_CFG += " -DOPENEXR_INCLUDE_DIR='" + install_dir + "/include'"
                # STR_CFG += " -DOPENEXR_IlmImf_LIBRARY_RELEASE='" + install_dir + "/lib/IlmImf.a'"
                
                STR_CFG += " -DBoost_DIR=''"
                STR_CFG += " -DBoost_INCLUDE_DIR=''"

            if(dict_config['debug']):
                STR_CFG += " -DGDAL_LIBRARY='" + install_dir + "/lib/libgdal31.a'"
                
                STR_CFG += " -DGIFLIB_LIBRARY='" + install_dir + "/lib/libgiflib.a'"
                
                # STR_CFG += " -DLIBLAS_LIBRARY='" + install_dir + "/lib/liblas.a'"

                STR_CFG += " -DFREETYPE_INCLUDE_DIR_freetype2='" + install_dir + "/include/freetype2'"
                STR_CFG += " -DFREETYPE_INCLUDE_DIR_ft2build='" + install_dir + "/include/freetype2'"
                STR_CFG += " -DFREETYPE_LIBRARY_RELEASE='" + install_dir + "/lib/libfreetype.a'"

                STR_CFG += " -DZLIB_INCLUDE_DIR='" + install_dir + "/include'"
                STR_CFG += " -DZLIB_LIBRARY_RELEASE='" + install_dir + "/lib/libz.a'"

                STR_CFG += " -DPNG_PNG_INCLUDE_DIR='" + install_dir + "/include'"
                STR_CFG += " -DPNG_LIBRARY_RELEASE='" + install_dir + "/lib/libpng16.a'"
                
                STR_CFG += " -DJASPER_INCLUDE_DIR='" + install_dir + "/include'"
                STR_CFG += " -DJASPER_LIBRARY_RELEASE='" + install_dir + "/lib/jasper.a'"
                
                STR_CFG += " -DJPEG_INCLUDE_DIR='" + install_dir + "/include'"
                STR_CFG += " -DJPEG_LIBRARY_RELEASE='" + install_dir + "/lib/jpeg.a'"
                
                STR_CFG += " -DCURL_INCLUDE_DIR='" + install_dir + "/include/curl'"
                STR_CFG += " -DCURL_LIBRARY_RELEASE='" + install_dir + "/lib/libcurl.a'"
                
                STR_CFG += " -DTIFF_INCLUDE_DIR='" + install_dir + "/include'"
                STR_CFG += " -DTIFF_LIBRARY_RELEASE='" + install_dir + "/lib/libtiff.a'"
                
                # STR_CFG += " -DNVSQUISH_LIBRARY_DEBUG='" + install_dir + "/lib/squishd.a'"
                # STR_CFG += " -DNVTT_LIBRARY_DEBUG='" + install_dir + "/lib/nvttd.a'"

                # STR_CFG += " -DOPENEXR_INCLUDE_DIR='" + install_dir + "/include'"
                # STR_CFG += " -DOPENEXR_IlmImf_LIBRARY_RELEASE='" + install_dir + "/lib/IlmImf.a'"
                
                STR_CFG += " -DBoost_DIR=''"
                STR_CFG += " -DBoost_INCLUDE_DIR=''"
                
        else:
            STR_CFG += ' -DBUILD_OSG_APPLICATIONS=1'
            STR_CFG += ' -DBUILD_OSG_EXAMPLES=0'
            
            STR_CFG += ' -DDYNAMIC_OPENSCENEGRAPH=1'
            STR_CFG += ' -DDYNAMIC_OPENTHREADS=1'
            STR_CFG += ' -DOSG_GL_LIBRARY_STATIC=0'
            
            if(dict_config['release'] or dict_config['debuginfo']):
                STR_CFG += " -DGDAL_LIBRARY='" + install_dir + "/lib/libgdal31.so'"
                STR_CFG += " -DGIFLIB_LIBRARY='" + install_dir + "/lib/libgiflib.so'"
                STR_CFG += " -DLIBLAS_LIBRARY='" + install_dir + "/lib/liblas.so'"

                STR_CFG += " -DFREETYPE_INCLUDE_DIR_freetype2='" + install_dir + "/include/freetype2'"
                STR_CFG += " -DFREETYPE_INCLUDE_DIR_ft2build='" + install_dir + "/include/freetype2'"
                STR_CFG += " -DFREETYPE_LIBRARY_RELEASE='" + install_dir + "/lib/libfreetype.so'"

                STR_CFG += " -DZLIB_INCLUDE_DIR='" + install_dir + "/include'"
                STR_CFG += " -DZLIB_LIBRARY_RELEASE='" + install_dir + "/lib/libz.so'"

                STR_CFG += " -DPNG_PNG_INCLUDE_DIR='" + install_dir + "/include'"
                STR_CFG += " -DPNG_LIBRARY_RELEASE='" + install_dir + "/lib/libpng.so'"

                STR_CFG += " -DBoost_DIR=''"
                STR_CFG += " -DBoost_INCLUDE_DIR=''"

            if(dict_config['debug']):
                STR_CFG += " -DGDAL_LIBRARY='" + install_dir + "/lib/libgdal31.so'"
                # STR_CFG += " -DCURL_LIBRARY_RELEASE='" + install_dir + "/lib/libcurld_imp.lib'"
                STR_CFG += " -DCURL_LIBRARY_DEBUG='" + install_dir + "/lib/libcurld.so'"
                #STR_CFG += " -DNVSQUISH_LIBRARY_DEBUG='" + install_dir + "/lib/squishd.so'"
                #STR_CFG += " -DNVTT_LIBRARY_DEBUG='" + install_dir + "/lib/nvttd.so'"
                STR_CFG += " -DGIFLIB_LIBRARY='" + install_dir + "/lib/libgiflibd.so'"
                STR_CFG += " -DLIBLAS_LIBRARY='" + install_dir + "/lib/liblasd.so'"
            
    if(dict_config['arch']=="em"):
        # STR_CFG += ' -DOSG_USE_UTF8_FILENAME=1'
        
        STR_CFG += ' -DOPENGL_PROFILE=GLES2'
        STR_CFG += ' -DBUILD_OSG_APPLICATIONS=0'
        STR_CFG += ' -DBUILD_OSG_EXAMPLES=0'
        
        STR_CFG += ' -DDYNAMIC_OPENSCENEGRAPH=0'
        STR_CFG += ' -DDYNAMIC_OPENTHREADS=0'
        STR_CFG += ' -DOSG_GL_LIBRARY_STATIC=1'
        STR_CFG += ' -DEGL_LIBRARY=egl' #这里应该无所谓是什么，只要赋值就行了。
    
    
        STR_CFG += " -DGDAL_INCLUDE_DIR='" + install_dir + "/include/gdal'"
        STR_CFG += " -DOPENEXR_INCLUDE_DIR='" + install_dir + "/include'"
        STR_CFG += " -DGIFLIB_INCLUDE_DIR='" + install_dir + "/include'"
        STR_CFG += " -DLUA_INCLUDE_DIR='" + install_dir + "/include'"
        
        STR_CFG += " -DGDAL_LIBRARY='" + install_dir + "/lib/libgdal31.a'"
                    
        STR_CFG += " -DGIFLIB_LIBRARY='" + install_dir + "/lib/libgiflib.a'"
        
        # STR_CFG += " -DLIBLAS_LIBRARY='" + install_dir + "/lib/liblas.a'"

        STR_CFG += " -DFREETYPE_INCLUDE_DIR_freetype2='" + install_dir + "/include/freetype2'"
        STR_CFG += " -DFREETYPE_INCLUDE_DIR_ft2build='" + install_dir + "/include/freetype2'"
        STR_CFG += " -DFREETYPE_LIBRARY_RELEASE='" + install_dir + "/lib/libfreetype.a'"

        STR_CFG += " -DZLIB_INCLUDE_DIR='" + install_dir + "/include'"
        STR_CFG += " -DZLIB_LIBRARY_RELEASE='" + install_dir + "/lib/libz.a'"

        STR_CFG += " -DPNG_PNG_INCLUDE_DIR='" + install_dir + "/include'"
        STR_CFG += " -DPNG_LIBRARY_RELEASE='" + install_dir + "/lib/libpng16.a'"
        
        STR_CFG += " -DJASPER_INCLUDE_DIR='" + install_dir + "/include'"
        STR_CFG += " -DJASPER_LIBRARY_RELEASE='" + install_dir + "/lib/jasper.a'"
        
        STR_CFG += " -DJPEG_INCLUDE_DIR='" + install_dir + "/include'"
        STR_CFG += " -DJPEG_LIBRARY_RELEASE='" + install_dir + "/lib/jpeg.a'"
        
        # STR_CFG += " -DCURL_INCLUDE_DIR='" + install_dir + "/include/curl'"
        # STR_CFG += " -DCURL_LIBRARY_RELEASE='" + install_dir + "/lib/libcurl.a'"
        
        STR_CFG += " -DTIFF_INCLUDE_DIR='" + install_dir + "/include'"
        STR_CFG += " -DTIFF_LIBRARY_RELEASE='" + install_dir + "/lib/libtiff.a'"
        
        # STR_CFG += " -DNVSQUISH_LIBRARY_DEBUG='" + install_dir + "/lib/squishd.a'"
        # STR_CFG += " -DNVTT_LIBRARY_DEBUG='" + install_dir + "/lib/nvttd.a'"

        # STR_CFG += " -DOPENEXR_INCLUDE_DIR='" + install_dir + "/include'"
        # STR_CFG += " -DOPENEXR_IlmImf_LIBRARY_RELEASE='" + install_dir + "/lib/IlmImf.a'"
        
    
    source_dir = os.getcwd() + '/../prebuild/OpenSceneGraph-3.6.5'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    