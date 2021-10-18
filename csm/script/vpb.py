
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("osg" , list_name,getDependency)
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/madler/zlib.git")
        # return
        
    install_dir = os.getcwd() +"/install/"+ my_build_and_install_dir(dict_config)
        
    STR_CFG = ''

    if(dict_config['arch'][:2]=="vs"):
        STR_CFG += ' -D3rdPartyRoot=' + install_dir + '/'
        STR_CFG += ' -DGDAL_INCLUDE_DIR=' + install_dir + '/include/gdal'
        STR_CFG += ' -DGDAL_LIBRARY=' + install_dir + '/lib/gdal31.lib'
        STR_CFG += ' -DGDAL_LIBRARY_DEBUG=' + install_dir + '/lib/gdal31d.lib'
        
        if(dict_config['static']):
            STR_CFG += " -DDYNAMIC_VIRTUALPLANETBUILDER=0"
            return
        else:
            STR_CFG += " -DDYNAMIC_VIRTUALPLANETBUILDER=1"

    if(dict_config['arch']=="unix"):
        STR_CFG += ' -D3rdPartyRoot=' + install_dir + '/'
        STR_CFG += ' -DGDAL_INCLUDE_DIR=' + install_dir + '/include/gdal'
        STR_CFG += ' -DGDAL_LIBRARY=' + install_dir + '/lib/libgdal31.so'
        STR_CFG += ' -DGDAL_LIBRARY_DEBUG=' + install_dir + '/lib/libgdal31.so'
        
        if(dict_config['static']):
            STR_CFG += " -DDYNAMIC_VIRTUALPLANETBUILDER=0"
            return
        else:
            STR_CFG += " -DDYNAMIC_VIRTUALPLANETBUILDER=1"
        
    source_dir = os.getcwd() + '/../prebuild/VirtualPlanetBuilder-master'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    