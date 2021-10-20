
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/heyuqi/pthreads-w32-cmake.git")
        # return
        
    STR_CFG = ''
    if(dict_config['arch'][:2]=="vs"):
        STR_CFG += ' -DHAVE_STRUCT_TIMESPEC=1'
        if(dict_config['static']):
            STR_CFG += " -DPTHREADS_BUILD_STATIC=1"
        else:
            STR_CFG += " -DPTHREADS_BUILD_STATIC=0"
        
    if(dict_config['arch']=="unix"):
        return
    
    if(dict_config['arch']=="ndk"):
        return
        
    source_dir = os.getcwd() + '/../prebuild/pthreads-win32.cmake-master'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    