
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("zlib" , list_name,getDependency)
    list_name = addDependency("ilmbase" , list_name,getDependency)
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/madler/zlib.git")
        # return
        
    install_dir = os.getcwd() +"/install/"+ my_build_and_install_dir(dict_config)
    
    
    STR_CFG = ''
    STR_CFG += ' -DNAMESPACE_VERSIONING=OFF'
    STR_CFG += ' -DILMBASE_PACKAGE_PREFIX=' + install_dir
    STR_CFG += ' -DPACK_BINARY_NSIS=OFF'
    
    if(dict_config['static']):
        # STR_CFG = " -DBUILD_SHARED_LIBS=0"
        STR_CFG += ' -DZLIB_LIBRARY=' + install_dir + '/lib/zlibstatic.lib'
        STR_CFG += ' -DZLIB_LIBRARY_DEBUG=' + install_dir + '/lib/zlibstaticd.lib'
        # if(dict_config['debug']==True):
            # STR_CFG += ' -DZLIB_LIBRARY_DEBUG=' + install_dir + '/lib/zlibstaticd.lib'
        # else:
            # STR_CFG += ' -DZLIB_LIBRARY_DEBUG=' + install_dir + '/lib/zlibstatic.lib'
    else:
        # STR_CFG = " -DBUILD_SHARED_LIBS=1"
        STR_CFG += ' -DZLIB_LIBRARY=' + install_dir + '/lib/zlib.lib'
        STR_CFG += ' -DZLIB_LIBRARY_DEBUG=' + install_dir + '/lib/zlibd.lib'
        # if(dict_config['debug']==True):
            # STR_CFG += ' -DZLIB_LIBRARY_DEBUG=' + install_dir + '/lib/zlibd.lib'
        # else:
            # STR_CFG += ' -DZLIB_LIBRARY_DEBUG=' + install_dir + '/lib/zlib.lib'
            
            
    source_dir = os.getcwd() + '/../prebuild/openexr-2.2.0'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    