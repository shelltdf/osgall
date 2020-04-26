
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("zlib" , list_name,getDependency)

    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):

    # if(b_only_download):
        # download_source(str_name,"https://github.com/glennrp/libpng.git")
    
        # return
        
    STR_CFG = ''
    if(dict_config['static']):
        STR_CFG = " -DPNG_SHARED=0 -DPNG_STATIC=1";
    else:
        STR_CFG = " -DPNG_SHARED=1 -DPNG_STATIC=0";
        
    STR_CFG += ' -DZLIB_LIBRARY=' +os.getcwd() +"/install/"+ my_build_and_install_dir(dict_config) + '/lib/zlib.lib'
    STR_CFG += ' -DZLIB_LIBRARY_DEBUG=' +os.getcwd() +"/install/"+ my_build_and_install_dir(dict_config) + '/lib/zlibd.lib'
  
  
    source_dir = os.getcwd() + '/../prebuild/libpng-1.6.15'

    configure(str_name,dict_config,STR_CFG,"",source_dir)    
    configure(str_name,dict_config)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    