
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("zlib" , list_name,getDependency)
    list_name = addDependency("boost" , list_name,getDependency)
    list_name = addDependency("tbb" , list_name,getDependency)
    list_name = addDependency("hdf" , list_name,getDependency)
    list_name = addDependency("openexr" , list_name,getDependency)
    list_name = addDependency("alembic" , list_name,getDependency)
    list_name = addDependency("draco" , list_name,getDependency)
    list_name = addDependency("materialx" , list_name,getDependency)
    list_name = addDependency("ptex" , list_name,getDependency)
    list_name = addDependency("opensubdiv" , list_name,getDependency)
    list_name = addDependency("blosc" , list_name,getDependency)
    list_name = addDependency("openvdb" , list_name,getDependency)
    list_name = addDependency("libjpeg" , list_name,getDependency)
    list_name = addDependency("libtiff" , list_name,getDependency)
    list_name = addDependency("libpng" , list_name,getDependency)
    list_name = addDependency("openimageio" , list_name,getDependency)
    list_name = addDependency("opencolorio" , list_name,getDependency)
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/madler/zlib.git")
        # return
    
    install_dir = dict_config['install_dir'] + '/' + my_build_and_install_dir(dict_config)
    install_dir = install_dir.replace('\\','/')	
    
    STR_CFG = ''
    if(dict_config['static']):
        STR_CFG += " -DBUILD_SHARED_LIBS=0"
        STR_CFG += " -DPXR_ENABLE_PYTHON_SUPPORT=0"
    else:
        STR_CFG += " -DBUILD_SHARED_LIBS=1"
    

    if(dict_config['debug']==True):
        # STR_CFG += ' -DTBB_tbb_debug_LIBRARY_DEBUG=' + install_dir + '/lib/tbb_debug.lib'
        # STR_CFG += ' -DTBB_tbb_LIBRARY_RELEASE=' + install_dir + '/lib/tbb_debug.lib'
        STR_CFG += ' -DTBB_tbb_LIBRARY=' + install_dir + '/lib/tbb_debug.lib'
        STR_CFG += " -DTBB_USE_DEBUG_BUILD=1"
    else:
        # STR_CFG += ' -DTBB_tbb_debug_LIBRARY_RELEASE=' + install_dir + '/lib/tbb.lib'
        STR_CFG += ' -DTBB_tbb_LIBRARY=' + install_dir + '/lib/tbb.lib'
        
    source_dir = os.getcwd() + '/../prebuild/OpenUSD-release'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    