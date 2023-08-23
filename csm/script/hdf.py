
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    # list_name = addDependency("zlib" , list_name,getDependency)
    
    return list_name + [str_name]
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):

    # if(b_only_download):
        # download_source(str_name,"https://github.com/curl/curl.git")
    
        # return
        
    install_dir = dict_config['install_dir'] + '/' + my_build_and_install_dir(dict_config)
    
    STR_CFG = ''
    STR_CFG += ' -DBUILD_TESTING=0'
    STR_CFG += ' -DHDF5_BUILD_EXAMPLES=0'
    STR_CFG += ' -DH5_USE_110_API:BOOL=ON'
    
    if(dict_config['static']):
        STR_CFG += " -DBUILD_STATIC_LIBS=1"
        STR_CFG += " -DBUILD_SHARED_LIBS=0"
        STR_CFG += " -DONLY_SHARED_LIBS=0"
        
        STR_CFG += " -DHDF5_BUILD_TOOLS=0"
        
        # STR_CFG += " -DONLY_SHARED_LIBS=0"
        # STR_CFG += ' -DZLIB_LIBRARY=' + install_dir + '/lib/zlibstatic.lib'
        # STR_CFG += ' -DZLIB_LIBRARY_DEBUG=' + install_dir + '/lib/zlibstaticd.lib'
    else:
        STR_CFG += " -DBUILD_STATIC_LIBS=0"
        STR_CFG += " -DBUILD_SHARED_LIBS=1"
        STR_CFG += " -DONLY_SHARED_LIBS=1"
        
        STR_CFG += " -DHDF5_BUILD_TOOLS=0"
        
        # STR_CFG += " -DONLY_SHARED_LIBS=1"
        # STR_CFG += " -DBUILD_CURL_EXE=0"
        # STR_CFG += " -DBUILD_CURL_TESTS=0"
        # STR_CFG += ' -DZLIB_LIBRARY=' + install_dir + '/lib/zlib.lib'
        # STR_CFG += ' -DZLIB_LIBRARY_DEBUG=' + install_dir + '/lib/zlibd.lib'
    

    if(dict_config['arch']=="ndk"):
        return
        
    source_dir = os.getcwd() + '/../prebuild/hdf5-1.12.0'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    