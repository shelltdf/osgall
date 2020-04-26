
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/madler/zlib.git")
        # return
        
    STR_CFG = ' -DNAMESPACE_VERSIONING=OFF -DPACK_BINARY_NSIS=OFF'
    # if(dict_config['static']):
        # str_ops = " -DBUILD_SHARED_LIBS=0"
    # else:
        # str_ops = " -DBUILD_SHARED_LIBS=1"
        
    source_dir = os.getcwd() + '/../prebuild/ilmbase-2.2.0'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    