
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/hunter-packages/jpeg.git")
        # download_source(str_name,"https://github.com/LuaDist/libjpeg.git")
        # download_source(str_name,"https://github.com/stohrendorf/libjpeg-cmake.git")
        # download_source(str_name,"https://github.com/shelltdf/libjpeg-cmake.git")
        
        # return
    
    STR_CFG = ''
    if(dict_config['static']):
        STR_CFG += " -DJPEG_BUILD_STATIC=1"
    else:
        STR_CFG += " -DJPEG_BUILD_STATIC=0"
        
    source_dir = os.getcwd() + '/../prebuild/jpeg-9a'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    