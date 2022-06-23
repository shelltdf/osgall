
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("sqlite3" , list_name,getDependency)
    list_name = addDependency("libtiff" , list_name,getDependency)
    list_name = addDependency("curl" , list_name,getDependency)
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/OSGeo/proj.4.git")
        # download_source(str_name,"https://github.com/shelltdf/proj.4.git")
        
        # return
        
    dir_name = my_build_and_install_dir(dict_config)
    install_dir = dict_config['install_dir'] + '/' + my_build_and_install_dir(dict_config)
        
    STR_CFG = ""
    
    if(dict_config['arch'][:2]=="vs"):
        STR_CFG += " -DPROJ_TESTS=0"
        STR_CFG += " -DBUILD_TESTING=0"
        
        STR_CFG += ' -DSQLITE3_INCLUDE_DIR=' + install_dir + '/include'
        STR_CFG += ' -DSQLITE3_LIBRARY=' + install_dir + '/lib/sqlite.lib'
        STR_CFG += ' -DSQLITE3_LIBRARY_DEBUG=' + install_dir + '/lib/sqlited.lib'
        
        if(dict_config['static']):
            STR_CFG += " -DBUILD_LIBPROJ_SHARED=0"
            STR_CFG += " -DBUILD_SHARED_LIBS=0"
            
            STR_CFG += " -DBUILD_CCT=0"
            STR_CFG += " -DBUILD_CS2CS=0"
            STR_CFG += " -DBUILD_GEOD=0"
            STR_CFG += " -DBUILD_GIE=0"
            STR_CFG += " -DBUILD_PROJ=0"
            STR_CFG += " -DBUILD_PROJINFO=0"
            STR_CFG += " -DBUILD_PROJSYNC=0"
            
            STR_CFG += ' -DCURL_LIBRARY=' + install_dir + '/lib/libcurl.lib'
            STR_CFG += ' -DCURL_LIBRARY_DEBUG=' + install_dir + '/lib/libcurld.lib'
            
        else:
            STR_CFG += " -DBUILD_LIBPROJ_SHARED=1"
            STR_CFG += " -DBUILD_SHARED_LIBS=1"
            
            STR_CFG += ' -DCURL_LIBRARY=' + install_dir + '/lib/libcurl_imp.lib'
            STR_CFG += ' -DCURL_LIBRARY_DEBUG=' + install_dir + '/lib/libcurld_imp.lib'
        
    
    if(dict_config['arch']=="unix"):
        STR_CFG += " -DPROJ_TESTS=0"
        STR_CFG += " -DBUILD_TESTING=0"
        
        if(dict_config['static']):
            STR_CFG += " -DBUILD_LIBPROJ_SHARED=0"
            STR_CFG += " -DBUILD_SHARED_LIBS=0"
            # STR_CFG += " -DPROJ_TESTS=0"
            
            STR_CFG += " -DBUILD_CCT=0"
            STR_CFG += " -DBUILD_CS2CS=0"
            STR_CFG += " -DBUILD_GEOD=0"
            STR_CFG += " -DBUILD_GIE=0"
            STR_CFG += " -DBUILD_PROJ=0"
            STR_CFG += " -DBUILD_PROJINFO=0"
            
            STR_CFG += ' -DSQLITE3_LIBRARY=' + install_dir + '/lib/libsqlite.a'
            STR_CFG += ' -DSQLITE3_LIBRARY_DEBUG=' + install_dir + '/lib/libsqlite.a'
            
        else:
            STR_CFG += " -DBUILD_LIBPROJ_SHARED=1"
            STR_CFG += " -DBUILD_SHARED_LIBS=1"
            # STR_CFG += " -DPROJ_TESTS=0"
            
            STR_CFG += ' -DSQLITE3_LIBRARY=' + install_dir + '/lib/libsqlite.so'
            STR_CFG += ' -DSQLITE3_LIBRARY_DEBUG=' + install_dir + '/lib/libsqlite.so'
            
            STR_CFG += ' -DCURL_LIBRARY=' + install_dir + '/lib/libcurl.so'
            STR_CFG += ' -DCURL_LIBRARY_DEBUG=' + install_dir + '/lib/libcurld.so'
    
    if(dict_config['arch']=="ndk"):
        STR_CFG += " -DPROJ_TESTS=0"
        STR_CFG += " -DBUILD_TESTING=0"
        
        if(dict_config['static']):
            STR_CFG += " -DBUILD_LIBPROJ_SHARED=0"
            STR_CFG += " -DBUILD_SHARED_LIBS=0"
            # STR_CFG += " -DPROJ_TESTS=0"
            
            STR_CFG += " -DBUILD_CCT=0"
            STR_CFG += " -DBUILD_CS2CS=0"
            STR_CFG += " -DBUILD_GEOD=0"
            STR_CFG += " -DBUILD_GIE=0"
            STR_CFG += " -DBUILD_PROJ=0"
            STR_CFG += " -DBUILD_PROJINFO=0"
            STR_CFG += " -DBUILD_PROJSYNC=0"
            
            STR_CFG += ' -DSQLITE3_INCLUDE_DIR=' + install_dir + '/include'
            STR_CFG += ' -DSQLITE3_LIBRARY=' + install_dir + '/lib/libsqlite.a'
            STR_CFG += ' -DSQLITE3_LIBRARY_DEBUG=' + install_dir + '/lib/libsqlite.a'
            
            STR_CFG += ' -DCURL_INCLUDE_DIR=' + install_dir + '/include'
            STR_CFG += ' -DCURL_LIBRARY=' + install_dir + '/lib/libcurl.so'
            STR_CFG += ' -DCURL_LIBRARY_DEBUG=' + install_dir + '/lib/libcurld.so'
            
            STR_CFG += ' -DTIFF_INCLUDE_DIR=' + install_dir + '/include'
            STR_CFG += ' -DTIFF_LIBRARY=' + install_dir + '/lib/libtiff.so'
            STR_CFG += ' -DTIFF_LIBRARY_DEBUG=' + install_dir + '/lib/libtiffd.so'
            
        else:
            STR_CFG += " -DBUILD_LIBPROJ_SHARED=1"
            STR_CFG += " -DBUILD_SHARED_LIBS=1"
            # STR_CFG += " -DPROJ_TESTS=0"
            
            STR_CFG += " -DBUILD_CCT=0"
            STR_CFG += " -DBUILD_CS2CS=0"
            STR_CFG += " -DBUILD_GEOD=0"
            STR_CFG += " -DBUILD_GIE=0"
            STR_CFG += " -DBUILD_PROJ=0"
            STR_CFG += " -DBUILD_PROJINFO=0"
            STR_CFG += " -DBUILD_PROJSYNC=0"
            
            STR_CFG += " -DCMAKE_USE_PTHREADS_INIT=0"
            
            STR_CFG += ' -DSQLITE3_INCLUDE_DIR=' + install_dir + '/include'
            STR_CFG += ' -DSQLITE3_LIBRARY=' + install_dir + '/lib/libsqlite.so'
            STR_CFG += ' -DSQLITE3_LIBRARY_DEBUG=' + install_dir + '/lib/libsqlite.so'
            
            STR_CFG += ' -DCURL_INCLUDE_DIR=' + install_dir + '/include'
            STR_CFG += ' -DCURL_LIBRARY=' + install_dir + '/lib/libcurl.so'
            STR_CFG += ' -DCURL_LIBRARY_DEBUG=' + install_dir  + '/lib/libcurld.so'
            
            STR_CFG += ' -DTIFF_INCLUDE_DIR=' + install_dir + '/include'
            STR_CFG += ' -DTIFF_LIBRARY=' + install_dir + '/lib/libtiff.so'
            STR_CFG += ' -DTIFF_LIBRARY_DEBUG=' + install_dir + '/lib/libtiffd.so'
            
    if(dict_config['arch']=="em"):
    
        STR_CFG += " -DBUILD_SHARED_LIBS=0"
        STR_CFG += " -DPROJ_TESTS=0"
        STR_CFG += " -DBUILD_TESTING=0"
        
        # STR_CFG += " -DENABLE_TIFF=0"
        STR_CFG += " -DENABLE_CURL=0"
        
        STR_CFG += " -DBUILD_CCT=0"
        STR_CFG += " -DBUILD_CS2CS=0"
        STR_CFG += " -DBUILD_GEOD=0"
        STR_CFG += " -DBUILD_GIE=0"
        STR_CFG += " -DBUILD_PROJ=0"
        STR_CFG += " -DBUILD_PROJINFO=0"
        STR_CFG += " -DBUILD_PROJSYNC=0"
        
        STR_CFG += ' -DSQLITE3_INCLUDE_DIR="' + install_dir + '"/include'
        STR_CFG += ' -DSQLITE3_LIBRARY=' + install_dir + '/lib/libsqlite.a'
        STR_CFG += ' -DSQLITE3_LIBRARY_DEBUG=' + install_dir + '/lib/libsqlite.a'
        
        STR_CFG += ' -DTIFF_INCLUDE_DIR=' + install_dir + '/include'
        STR_CFG += ' -DTIFF_LIBRARY=' + install_dir + '/lib/libtiff.a'
        STR_CFG += ' -DTIFF_LIBRARY_DEBUG=' + install_dir + '/lib/libtiffd.a'
        
        # STR_CFG += ' -DCURL_INCLUDE_DIR=' + install_dir + '/include'
        # STR_CFG += ' -DCURL_LIBRARY=' + install_dir + '/lib/libcurl.a'
        # STR_CFG += ' -DCURL_LIBRARY_DEBUG=' + install_dir  + '/lib/libcurld.a'

    # source_dir = os.getcwd() + '/../prebuild/proj-6.1.1'
    source_dir = os.getcwd() + '/../prebuild/proj-8.0.1'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    