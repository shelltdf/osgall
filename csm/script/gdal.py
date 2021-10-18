
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("libpng" , list_name,getDependency)
    list_name = addDependency("libgeotiff" , list_name,getDependency)
    list_name = addDependency("libiconv" , list_name,getDependency)
    list_name = addDependency("curl" , list_name,getDependency)
    list_name = addDependency("libjpeg" , list_name,getDependency)
    list_name = addDependency("proj4" , list_name,getDependency)
    list_name = addDependency("geos" , list_name,getDependency)
    list_name = addDependency("expat" , list_name,getDependency)

    return list_name + [str_name]
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):

    # if(b_only_download):
        # download_source(str_name,"https://github.com/aashish24/gdal-svn.git","cmake4gdal")
        # download_source(str_name,"https://github.com/shelltdf/gdal-svn.git","cmake4gdal")
        
        # return
        
        
    dir_name = my_build_and_install_dir(dict_config)
    install_dir = os.getcwd() +"/install/"+ my_build_and_install_dir(dict_config)
        
    STR_CFG = ''
    
    # STR_CFG += ' -DGDAL_USE_ICONV=1'
    STR_CFG += ' -DGDAL_USE_CURL=OFF'
    STR_CFG += " -DGDAL_USE_OPENCL=0 "
    
    STR_CFG += " -DGDAL_ENABLE_FRMT_BMP=1 "
    STR_CFG += " -DGDAL_ENABLE_FRMT_DIMAP=0 "
    STR_CFG += " -DGDAL_ENABLE_FRMT_GIF=0 "
    STR_CFG += " -DGDAL_ENABLE_FRMT_JPEG=0 "
    STR_CFG += " -DGDAL_ENABLE_FRMT_MEM=1 "
    STR_CFG += " -DGDAL_ENABLE_FRMT_OZI=0 "
    STR_CFG += " -DGDAL_ENABLE_FRMT_OZIMAP=0 "
    STR_CFG += " -DGDAL_ENABLE_FRMT_PNG=0 "
    STR_CFG += " -DGDAL_ENABLE_FRMT_POSTGISRASTER=0 "
    STR_CFG += " -DGDAL_ENABLE_FRMT_RAW=1 "
    STR_CFG += " -DGDAL_ENABLE_FRMT_SAGA=0 "
    STR_CFG += " -DGDAL_ENABLE_FRMT_TIL=0 "
    STR_CFG += " -DGDAL_ENABLE_FRMT_WMS=0 "
    STR_CFG += " -DOGR_ENABLE_CSV=1 "
    STR_CFG += " -DOGR_ENABLE_DFX=1 "
    STR_CFG += " -DOGR_ENABLE_GPX=1 "
    STR_CFG += " -DOGR_ENABLE_LIBKML=0 "
    STR_CFG += " -DOGR_ENABLE_MEM=1 "
    STR_CFG += " -DOGR_ENABLE_MYSQL=0 "
    STR_CFG += " -DOGR_ENABLE_PG=0 "
    STR_CFG += " -DOGR_ENABLE_S57=0 "
    STR_CFG += " -DOGR_ENABLE_SHP=1 "
    STR_CFG += " -DOGR_ENABLE_SQLITE=0 "
    STR_CFG += " -DOGR_ENABLE_SXF=1 "
    STR_CFG += " -DOGR_ENABLE_VRT=1 "
    STR_CFG += " -DOGR_ENABLE_WFS=1 "
    

    
    # STR_CFG += " -DCURL_INCLUDE_DIR='../../../install/" + dir_name + "/include/'"
    
    if(dict_config['arch'][:2]=="vs"):
        if(dict_config['static']):
            STR_CFG += " -DGDAL_BUILD_SHARED=0"
            STR_CFG += " -DGDAL_BUILD_APPS=0"
        else:
            STR_CFG += " -DGDAL_BUILD_SHARED=1"
            STR_CFG += ' -DGDAL_BUILD_APPS=1'
            
            
        STR_CFG += ' -DGDAL_USE_ICONV=0'

        # STR_CFG += " -DCURL_LIBRARY='" + install_dir + "/lib/libcurl_imp.lib'"
        # STR_CFG += " -DCURL_LIBRARY_DEBUG='" + install_dir + "/lib/libcurl-d_imp.lib'"

        STR_CFG += " -DGEOTIFF_LIBRARY='" + install_dir + "/lib/geotiff_i.lib'"
        STR_CFG += " -DGEOTIFF_LIBRARY_DEBUG='" + install_dir + "/lib/geotiff_d_i.lib'"
        
        STR_CFG += " -DTIFF_LIBRARY='" + install_dir + "/lib/tiff.lib'"
        STR_CFG += " -DTIFF_LIBRARY_DEBUG='" + install_dir + "/lib/tiffd.lib'"
        
        STR_CFG += " -DGEOS_LIBRARY='" + install_dir + "/lib/geos_c.lib'"
        STR_CFG += " -DGEOS_LIBRARY_DEBUG='" + install_dir + "/lib/geos_cd.lib'"
        
        STR_CFG += " -DSQLITE3_LIBRARY='" + install_dir + "/lib/sqlite.lib'"
        STR_CFG += " -DSQLITE3_LIBRARY_DEBUG='" + install_dir + "/lib/sqlited.lib'"
        
        STR_CFG += " -DPROJ_INCLUDE_DIR='" + install_dir + "/include'"
        STR_CFG += " -DPROJ_LIBRARY='" + install_dir + "/lib/proj.lib'"
        STR_CFG += " -DPROJ_LIBRARY_DEBUG='" + install_dir + "/lib/proj_d.lib'"

        STR_CFG += " -DEXPAT_LIBRARY='" + install_dir + "/lib/expat.lib'"
        STR_CFG += " -DEXPAT_LIBRARY_DEBUG='" + install_dir + "/lib/expatd.lib'"
        
        STR_CFG += " -DZLIB_LIBRARY='" + install_dir + "/lib/zlib.lib'"
        STR_CFG += " -DZLIB_LIBRARY_DEBUG='" + install_dir + "/lib/zlibd.lib'"
        
        # STR_CFG += ' -DICONV_INCLUDE_DIR="' + install_dir + '"/include'
        # STR_CFG += " -DICONV_LIBRARIES='" + install_dir + "/lib/libiconv.lib'"
    
    if(dict_config['arch']=="unix"):

        if(dict_config['static']):
            STR_CFG += " -DGDAL_BUILD_SHARED=0"
            STR_CFG += " -DGDAL_BUILD_APPS=0"
        else:
            STR_CFG += " -DGDAL_BUILD_SHARED=1"
            STR_CFG += ' -DGDAL_BUILD_APPS=0'
            
        STR_CFG += ' -DGDAL_USE_ICONV=1'
        
        STR_CFG += ' -DGEOTIFF_INCLUDE_DIR="' + install_dir + '"/include'
        STR_CFG += " -DGEOTIFF_LIBRARY='" + install_dir + "/lib/libgeotiff.so'"        
        STR_CFG += " -DGEOTIFF_LIBRARY_DEBUG='" + install_dir + "/lib/libgeotiff.so'"    
        
        STR_CFG += ' -DJPEG_INCLUDE_DIR="' + install_dir + '"/include'
        STR_CFG += " -DJPEG_LIBRARY='" + install_dir + "/lib/libjpeg.so'"        
        STR_CFG += " -DJPEG_LIBRARY_DEBUG='" + install_dir + "/lib/libjpegd.so'"        
        
        STR_CFG += ' -DTIFF_INCLUDE_DIR="' + install_dir + '"/include'
        STR_CFG += " -DTIFF_LIBRARY='" + install_dir + "/lib/libtiff.so'"
        STR_CFG += " -DTIFF_LIBRARY_DEBUG='" + install_dir + "/lib/libtiff.so'"
        
        STR_CFG += ' -DGEOS_INCLUDE_DIR="' + install_dir + '"/include'
        STR_CFG += " -DGEOS_LIBRARY='" + install_dir + "/lib/libgeos_c.so'"
        STR_CFG += " -DGEOS_LIBRARY_DEBUG='" + install_dir + "/lib/libgeos_cd.so'"
        
        STR_CFG += ' -DSQLITE3_INCLUDE_DIR="' + install_dir + '"/include'
        STR_CFG += " -DSQLITE3_LIBRARY='" + install_dir + "/lib/libsqlite.so'"
        STR_CFG += " -DSQLITE3_LIBRARY_DEBUG='" + install_dir + "/lib/libsqlite.so'"
        
        STR_CFG += ' -DPROJ_INCLUDE_DIR="' + install_dir + '"/include'
        STR_CFG += " -DPROJ_LIBRARY='" + install_dir + "/lib/libproj.so'"
        STR_CFG += " -DPROJ_LIBRARY_DEBUG='" + install_dir + "/lib/libproj.so'"
        
        STR_CFG += ' -DEXPAT_INCLUDE_DIR="' + install_dir + '"/include'
        STR_CFG += " -DEXPAT_LIBRARY='" + install_dir + "/lib/libexpat.so'"
        STR_CFG += " -DEXPAT_LIBRARY_DEBUG='" + install_dir + "/lib/libexpatd.so'"
        
        STR_CFG += ' -DZLIB_INCLUDE_DIR="' + install_dir + '"/include'
        STR_CFG += " -DZLIB_LIBRARY='" + install_dir + "/lib/libz.so'"
        STR_CFG += " -DZLIB_LIBRARY_DEBUG='" + install_dir + "/lib/libz.so'"
        
        STR_CFG += ' -DICONV_INCLUDE_DIR="' + install_dir + '"/include'
        STR_CFG += " -DICONV_LIBRARIES='" + install_dir + "/lib/liblibiconv.so'"
        
        
    if(dict_config['arch']=="em"):
        STR_CFG += " -DGDAL_BUILD_SHARED=0"
        STR_CFG += " -DGDAL_BUILD_APPS=0"
            
        STR_CFG += ' -DGDAL_USE_ICONV=1'
        
        STR_CFG += ' -DGEOTIFF_INCLUDE_DIR="' + install_dir + '"/include'
        STR_CFG += " -DGEOTIFF_LIBRARY='" + install_dir + "/lib/libgeotiff.a'"        
        STR_CFG += " -DGEOTIFF_LIBRARY_DEBUG='" + install_dir + "/lib/libgeotiffd.a'"    
        
        STR_CFG += ' -DJPEG_INCLUDE_DIR="' + install_dir + '"/include'
        STR_CFG += " -DJPEG_LIBRARY='" + install_dir + "/lib/libjpeg.a'"        
        STR_CFG += " -DJPEG_LIBRARY_DEBUG='" + install_dir + "/lib/libjpegd.a'"        
        
        STR_CFG += ' -DTIFF_INCLUDE_DIR="' + install_dir + '"/include'
        STR_CFG += " -DTIFF_LIBRARY='" + install_dir + "/lib/libtiff.a'"
        STR_CFG += " -DTIFF_LIBRARY_DEBUG='" + install_dir + "/lib/libtiffd.a'"
        
        STR_CFG += ' -DGEOS_INCLUDE_DIR="' + install_dir + '"/include'
        STR_CFG += " -DGEOS_LIBRARY='" + install_dir + "/lib/libgeos-static.a'"
        STR_CFG += " -DGEOS_LIBRARY_DEBUG='" + install_dir + "/lib/libgeos-staticd.a'"
        
        STR_CFG += ' -DSQLITE3_INCLUDE_DIR="' + install_dir + '"/include'
        STR_CFG += " -DSQLITE3_LIBRARY='" + install_dir + "/lib/libsqlite.a'"
        STR_CFG += " -DSQLITE3_LIBRARY_DEBUG='" + install_dir + "/lib/libsqlited.a'"
        
        STR_CFG += ' -DPROJ_INCLUDE_DIR="' + install_dir + '"/include'
        STR_CFG += " -DPROJ_LIBRARY='" + install_dir + "/lib/libproj.a'"
        STR_CFG += " -DPROJ_LIBRARY_DEBUG='" + install_dir + "/lib/libprojd.a'"
        
        STR_CFG += ' -DEXPAT_INCLUDE_DIR="' + install_dir + '"/include'
        STR_CFG += " -DEXPAT_LIBRARY='" + install_dir + "/lib/libexpat.a'"
        STR_CFG += " -DEXPAT_LIBRARY_DEBUG='" + install_dir + "/lib/libexpatd.a'"
        
        STR_CFG += ' -DZLIB_INCLUDE_DIR="' + install_dir + '"/include'
        STR_CFG += " -DZLIB_LIBRARY='" + install_dir + "/lib/libz.a'"
        STR_CFG += " -DZLIB_LIBRARY_DEBUG='" + install_dir + "/lib/libzd.a'"
        
        STR_CFG += ' -DICONV_INCLUDE_DIR="' + install_dir + '"/include'
        STR_CFG += " -DICONV_LIBRARIES='" + install_dir + "/lib/liblibiconv.a'"
        # STR_CFG += " -DICONV_LIBRARY='" + install_dir + "/lib/liblibiconv.a'"
        # STR_CFG += " -DICONV_LIBRARY_DEBUG='" + install_dir + "/lib/liblibiconv.a'"
        
   
    source_dir = os.getcwd() + '/../prebuild/gdal/gdal'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    