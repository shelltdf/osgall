
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("zlib" , list_name,getDependency)
    list_name = addDependency("pugixml" , list_name,getDependency)
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/madler/zlib.git")
        # return
    
    install_dir = dict_config['install_dir'] + '/' + my_build_and_install_dir(dict_config)
    
    STR_CFG = ''
    if(dict_config['static']):
        STR_CFG += ' -DBUILD_SHARED_LIBS=0'
    else:
        STR_CFG += ' -DBUILD_SHARED_LIBS=1'
    
    STR_CFG += " -DASSIMP_INSTALL_PDB=0"
    STR_CFG += " -DASSIMP_BUILD_TESTS=0"
    # STR_CFG += " -DASSIMP_HUNTER_ENABLED=0"
    
    if(dict_config['arch'][:2]=="vs"):
        if(dict_config['static']):
            STR_CFG += ' -DZLIB_LIBRARY_REL=' + install_dir + '/lib/zlibstatic.lib'
            STR_CFG += ' -DZLIB_LIBRARY_DBG=' + install_dir + '/lib/zlibstaticd.lib'
        else:
            STR_CFG += ' -DZLIB_LIBRARY_REL=' + install_dir + '/lib/zlib.lib'
            STR_CFG += ' -DZLIB_LIBRARY_DBG=' + install_dir + '/lib/zlibd.lib'
    
    if(dict_config['arch']=="ndk"):
        if(dict_config['static']):
            pass
            # STR_CFG += " -DGDAL_BUILD_SHARED=0"
            # STR_CFG += " -DGDAL_BUILD_APPS=0"
        else:
            pass
            # STR_CFG += " -DGDAL_BUILD_SHARED=1"
            # STR_CFG += ' -DGDAL_BUILD_APPS=0'
    
    # STR_CFG += ' -Dpugixml_DIR=' + install_dir + '/lib/cmake/pugixml'
    
    source_dir = os.getcwd() + '/../prebuild/assimp'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    