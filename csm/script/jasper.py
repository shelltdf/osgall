
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("libjpeg" , list_name,getDependency)
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/mdadams/jasper.git")
    
        # return
        
    install_dir = os.getcwd() +"/install/"+ my_build_and_install_dir(dict_config)
    
    STR_CFG = ''
    if(dict_config['static']):
        STR_CFG += " -DJAS_ENABLE_SHARED=0"
    else:
        STR_CFG += " -DJAS_ENABLE_SHARED=1"
        
    STR_CFG += ' -DJPEG_INCLUDE_DIR=' + install_dir + '/include'
    
    # STR_CFG += ' -DJPEG_LIBRARY=' + install_dir + '/lib/jpeg.lib'
    # STR_CFG += ' -DJPEG_LIBRARY_DEBUG=' + install_dir + '/lib/jpegd.lib'
    
    if(dict_config['release']):
        STR_CFG += ' -DJPEG_LIBRARY=' + install_dir + '/lib/jpeg.lib'
    else:
        STR_CFG += ' -DJPEG_LIBRARY=' + install_dir + '/lib/jpegd.lib'
    
    
    source_dir = os.getcwd() + '/../prebuild/jasper-2.0.12'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    