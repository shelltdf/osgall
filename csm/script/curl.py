
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("zlib" , list_name,getDependency)
    
    return list_name + [str_name]
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):

    # if(b_only_download):
        # download_source(str_name,"https://github.com/curl/curl.git")
    
        # return
        
    install_dir = os.getcwd() +"/install/"+ my_build_and_install_dir(dict_config)
    
    STR_CFG = ''
    if(dict_config['static']):
        STR_CFG = " -DCURL_STATICLIB=1"
        STR_CFG += ' -DZLIB_LIBRARY=' + install_dir + '/lib/zlibstatic.lib'
        STR_CFG += ' -DZLIB_LIBRARY_DEBUG=' + install_dir + '/lib/zlibstaticd.lib'
    else:
        STR_CFG = " -DCURL_STATICLIB=0"
        STR_CFG += ' -DZLIB_LIBRARY=' + install_dir + '/lib/zlib.lib'
        STR_CFG += ' -DZLIB_LIBRARY_DEBUG=' + install_dir + '/lib/zlibd.lib'
        
    source_dir = os.getcwd() + '/../prebuild/curl-7.33.0'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    