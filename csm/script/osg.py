
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("nvtt" , list_name,getDependency)
    list_name = addDependency("openexr" , list_name,getDependency)
    list_name = addDependency("gdal" , list_name,getDependency)
    list_name = addDependency("libpng" , list_name,getDependency)
    list_name = addDependency("freetype" , list_name,getDependency)
    list_name = addDependency("jasper" , list_name,getDependency)

    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):

    # if(b_only_download):
        # download_source(str_name,"https://github.com/openscenegraph/OpenSceneGraph.git")
        # download_source(str_name,"https://github.com/shelltdf/OpenSceneGraph.git","OpenSceneGraph-3.4")
        
        # return
    
    dir_name = my_build_and_install_dir(dict_config)
    install_dir = os.getcwd() +"/install/"+ my_build_and_install_dir(dict_config)
    
    STR_CFG = " -DGDAL_INCLUDE_DIR='" + install_dir + "/include/gdal'"
    STR_CFG += " -DOPENEXR_INCLUDE_DIR='" + install_dir + "/include'"
    
    if(dict_config['release']):
        STR_CFG += " -DGDAL_LIBRARY='" + install_dir + "/lib/gdal31.lib'"
    else:
        STR_CFG += " -DGDAL_LIBRARY='" + install_dir + "/lib/gdal31d.lib'"
    
    source_dir = os.getcwd() + '/../prebuild/OpenSceneGraph-3.6.4'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    