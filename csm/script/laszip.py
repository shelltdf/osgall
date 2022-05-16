
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    # list_name = addDependency("boost" , list_name,getDependency)
    # list_name = addDependency("gdal" , list_name,getDependency)
    # list_name = addDependency("proj4" , list_name,getDependency)
    # list_name = addDependency("libgeotiff" , list_name,getDependency)
    # list_name = addDependency("libtiff" , list_name,getDependency)
    # list_name = addDependency("libjpeg" , list_name,getDependency)
    # list_name = addDependency("zlib" , list_name,getDependency)

    return list_name + [str_name]
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):

    # if(b_only_download):
        # download_source(str_name,"https://gitlab.com/libtiff/libtiff.git")
        # download_source(str_name,"https://github.com/LuaDist/libtiff.git")
        # download_source(str_name,"https://github.com/shelltdf/libtiff.git")
        # download_source(str_name,"https://github.com/shelltdf/libtiff.git","branch-3.9") #for TIFFClientOpen()
        
        # return
        
    dir_name = my_build_and_install_dir(dict_config)
    install_dir = dict_config['install_dir'] + '/' + my_build_and_install_dir(dict_config)
    install_dir = install_dir.replace('\\','/')

    STR_CFG = ''
    
    if(dict_config['static']):
        STR_CFG += " -DBUILD_STATIC=1"
        
    else:
        STR_CFG += " -DBUILD_STATIC=0"
        
    source_dir = os.getcwd() + '/../prebuild/LASzip-2.2.0'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    