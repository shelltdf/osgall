
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/madler/zlib.git")
        # return
        
    STR_CGG = ''
    STR_CGG += ' -DFREEGLUT_BUILD_DEMOS=0 -DINSTALL_PDB=0'
    if(dict_config['static']):
        STR_CGG += " -DGIFLIB_BUILD_STATIC=1"
    else:
        STR_CGG += " -DGIFLIB_BUILD_STATIC=0"
        
    source_dir = os.getcwd() + '/../prebuild/giflib-5.1.4'
    
    configure(str_name,dict_config,STR_CGG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    