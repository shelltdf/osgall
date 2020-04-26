
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/google/draco.git")
        # return
        
    STR_CFG = ''
    # if(dict_config['static']):
        # STR_CGG += ''
    # else:
        # STR_CGG += ''
    
    # dict_config['static'] = True
    # dict_config['dynamic'] = False
    
    source_dir = os.getcwd() + '/../prebuild/draco-master'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir,True)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    