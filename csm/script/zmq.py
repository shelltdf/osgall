
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
    STR_CFG += ' -DWITH_PERF_TOOL=0'
    
    if(dict_config['static']):
        STR_CFG += " -DBUILD_SHARED=0"
        STR_CFG += " -DBUILD_STATIC=1"
        STR_CFG += " -DBUILD_TESTS=0"
    else:
        STR_CFG += " -DBUILD_SHARED=1"
        STR_CFG += " -DBUILD_STATIC=0"
        STR_CFG += " -DBUILD_TESTS=0"
        
    if(dict_config['arch']=="ndk"):
        if(dict_config['static']):
            STR_CFG += " -DWITH_LIBSODIUM=0"
            STR_CFG += " -DWITH_LIBSODIUM=0"
        else:
            STR_CFG += " -DWITH_LIBSODIUM=0"
            STR_CFG += ' -DWITH_LIBSODIUM=0'
        
    source_dir = os.getcwd() + '/../prebuild/libzmq-master'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    