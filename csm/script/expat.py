
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/libexpat/libexpat.git")
    
        # return
        
    # STR_CFG = " -DBUILD_examples=0 -DBUILD_tests=0 -DBUILD_tools=0"
    STR_CFG = " -DEXPAT_BUILD_EXAMPLES=0 -DEXPAT_BUILD_TESTS=0 -DEXPAT_BUILD_TOOLS=0"
    if(dict_config['static']):
        # STR_CFG += " -DBUILD_shared=0"
        STR_CFG += " -DEXPAT_SHARED_LIBS=0"
    else:
        # STR_CFG += " -DBUILD_shared=1"
        STR_CFG += " -DEXPAT_SHARED_LIBS=1"
    
    # source_dir = os.getcwd() + '/../prebuild/expat-2.1.0'
    source_dir = os.getcwd() + '/../prebuild/expat-2.5.0'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    