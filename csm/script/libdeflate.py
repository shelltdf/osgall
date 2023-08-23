
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    # list_name = addDependency("expat" , list_name,getDependency)
    # list_name = addDependency("imath" , list_name,getDependency)
    # list_name = addDependency("zlib" , list_name,getDependency)
    # list_name = addDependency("openexr" , list_name,getDependency)
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/madler/zlib.git")
        # return
        
    dir_name = my_build_and_install_dir(dict_config)
    install_dir = dict_config['install_dir'] + '/' + my_build_and_install_dir(dict_config)
    
    STR_CFG = ''
    
    if(dict_config['static']):
        STR_CFG += " -DLIBDEFLATE_BUILD_SHARED_LIB=0"
        STR_CFG += " -DLIBDEFLATE_BUILD_STATIC_LIB=1"
        STR_CFG += " -DLIBDEFLATE_USE_SHARED_LIB=0"
    else:
        STR_CFG += " -DLIBDEFLATE_BUILD_SHARED_LIB=1"
        STR_CFG += " -DLIBDEFLATE_BUILD_STATIC_LIB=0"
        STR_CFG += " -DLIBDEFLATE_USE_SHARED_LIB=1"
        
    source_dir = os.getcwd() + '/../prebuild/libdeflate-master'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    