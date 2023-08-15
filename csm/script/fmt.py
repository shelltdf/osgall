
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
    STR_CFG += ' -DFMT_TEST=0'
    if(dict_config['static']):
        STR_CFG += " -DBUILD_SHARED_LIBS=0"
    else:
        STR_CFG += " -DBUILD_SHARED_LIBS=1"
        
    source_dir = os.getcwd() + '/../prebuild/fmt-master'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    