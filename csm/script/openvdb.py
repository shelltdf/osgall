
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("boost" , list_name,getDependency)
    list_name = addDependency("tbb" , list_name,getDependency)
    list_name = addDependency("blosc" , list_name,getDependency)
    # list_name = addDependency("ilmbase" , list_name,getDependency)
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/madler/zlib.git")
        # return
    
    
    
    STR_CFG = ''
    if(dict_config['static']):
        STR_CFG += " -DBUILD_SHARED_LIBS=0"
        STR_CFG += " -DUSE_STATIC_DEPENDENCIES=1"
    else:
        STR_CFG += " -DBUILD_SHARED_LIBS=1"
        STR_CFG += " -DUSE_STATIC_DEPENDENCIES=0"

    # source_dir = os.getcwd() + '/../prebuild/openvdb-master'
    # source_dir = os.getcwd() + '/../prebuild/openvdb-7.1.0'
    source_dir = os.getcwd() + '/../prebuild/openvdb-8.1.0'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    