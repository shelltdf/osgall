
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("zlib" , list_name,getDependency)
    list_name = addDependency("libjpeg" , list_name,getDependency)

    return list_name + [str_name]
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):

    # if(b_only_download):
        # download_source(str_name,"https://gitlab.com/libtiff/libtiff.git")
        # download_source(str_name,"https://github.com/LuaDist/libtiff.git")
        # download_source(str_name,"https://github.com/shelltdf/libtiff.git")
        # download_source(str_name,"https://github.com/shelltdf/libtiff.git","branch-3.9") #for TIFFClientOpen()
        
        # return
        
    STR_CFG = ''
    STR_CFG += ' -Dtiff-tests=0'
    if(dict_config['static']):
        # STR_CFG += " -DTIFF_BUILD_STATIC=1"
        STR_CFG += " -DBUILD_STATIC_LIBRARY=1"
    else:
        # STR_CFG += " -DTIFF_BUILD_STATIC=0"
        STR_CFG += " -DBUILD_STATIC_LIBRARY=0"
        
    # source_dir = os.getcwd() + '/../prebuild/tiff-4.0.3'
    source_dir = os.getcwd() + '/../prebuild/libtiff-master'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    