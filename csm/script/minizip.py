
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("zlib" , list_name,getDependency)
    # list_name = addDependency("bzip2" , list_name,getDependency)
    # list_name = addDependency("xz" , list_name,getDependency)
    # list_name = addDependency("zstd" , list_name,getDependency)
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/madler/zlib.git")
        # return
        
    STR_CFG = ''
    if(dict_config['static']):
        STR_CFG += " -DMINIZIP_BUILD_STATIC=1"
    else:
        STR_CFG += " -DMINIZIP_BUILD_STATIC=0"
            
    source_dir = os.getcwd() + '/../prebuild/minizip'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    