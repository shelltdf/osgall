
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/OSGeo/geos.git","3.5.0")
        # download_source(str_name,"https://github.com/shelltdf/geos.git")
        
        # return
        
    STR_CFG = ' -DGEOS_ENABLE_TESTS=0'
    if(dict_config['static']):
        STR_CFG += " -DBUILD_STATIC_LIBRARY=1";
    else:
        STR_CFG += " -DBUILD_STATIC_LIBRARY=0";
        
    source_dir = os.getcwd() + '/../prebuild/libgeos-3.5.0'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    