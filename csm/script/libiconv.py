
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/LuaDist/libiconv.git")
        # download_source(str_name,"https://github.com/vovythevov/libiconv-cmake.git")
        
        # return
        
    STR_CFG = ''
    
    
    if(dict_config['arch']=="em"):
        STR_CFG += " -DBUILD_SHARED_LIBS=0"
    
    source_dir = os.getcwd() + '/../prebuild/libiconv-cmake-master'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    