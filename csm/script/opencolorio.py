
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("expat" , list_name,getDependency)
    list_name = addDependency("imath" , list_name,getDependency)
    list_name = addDependency("zlib" , list_name,getDependency)
    list_name = addDependency("openexr" , list_name,getDependency)
    list_name = addDependency("pystring" , list_name,getDependency)
    list_name = addDependency("minizip-ng" , list_name,getDependency)
    list_name = addDependency("yaml-cpp" , list_name,getDependency)
    list_name = addDependency("lcms2" , list_name,getDependency)
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/madler/zlib.git")
        # return
    
    install_dir = dict_config['install_dir'] + '/' + my_build_and_install_dir(dict_config)
    install_dir = install_dir.replace('\\','/')	
    
    STR_CFG = ''
    STR_CFG += ' -DOCIO_BUILD_APPS=0'
    STR_CFG += ' -DOCIO_BUILD_PYTHON=0'
    STR_CFG += ' -DOCIO_BUILD_TESTS=0'
    STR_CFG += ' -DOCIO_BUILD_GPU_TESTS=0'
    STR_CFG += ' -DOCIO_INSTALL_EXT_PACKAGES="NONE"'
    
    if(dict_config['static']):
        STR_CFG += " -DOCIO_BUILD_SHARED=0"
        STR_CFG += " -DOCIO_BUILD_STATIC=1"
    else:
        STR_CFG += " -DOCIO_BUILD_SHARED=1"
        STR_CFG += " -DOCIO_BUILD_STATIC=0"
        
    STR_CFG += ' -Dminizip-ng_INCLUDE_DIR=' + install_dir + '/include/minizip/'
    
    STR_CFG += ' -Dyaml-cpp_LIBRARY=' + install_dir + '/lib/yaml-cpp.lib'
    
    # STR_CFG += ' -Dlcms2_INCLUDE_DIR=' + install_dir + '/include/'
    # STR_CFG += ' -Dlcms2_LIBRARY=' + install_dir + '/lib/lcms2.lib'
        
    source_dir = os.getcwd() + '/../prebuild/OpenColorIO-1.1.0'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    