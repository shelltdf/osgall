
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/madler/zlib.git")
        # return
        
    STR_CFG = ''
    STR_CFG += ' -DBUILD_TESTING=0'
    if(dict_config['static']):
        STR_CFG += " -DBUILD_SHARED_LIBS=0"
        STR_CFG += " -DBUILD_STATIC_LIBS=1"
        
        STR_CFG += " -DLZ4_BUILD_CLI=0"
        STR_CFG += " -DLZ4_BUILD_LEGACY_LZ4C=0"
    else:
        STR_CFG += " -DBUILD_SHARED_LIBS=1"
        STR_CFG += " -DBUILD_STATIC_LIBS=0"
        
    source_dir = os.getcwd() + '/../prebuild/lz4-dev/build/cmake'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    