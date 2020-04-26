
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/nigels-com/glew.git")
        # download_source(str_name,"https://github.com/Perlmint/glew-cmake.git")
        # return
        
    STR_CFG = ' -DBUILD_UTILS=0'
    # if(dict_config['static']):
        # STR_CGG += ''
    # else:
        # STR_CGG += ''
    
    source_dir = os.getcwd() + '/../prebuild/glew-2.1.0/build/cmake'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    