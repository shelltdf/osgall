
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("lz4" , list_name,getDependency)
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/madler/zlib.git")
        # return
    
    install_dir = dict_config['install_dir'] + '/' + my_build_and_install_dir(dict_config)
    install_dir = install_dir.replace('\\','/')	
    
    STR_CFG = ''
    STR_CFG += ' -DBUILD_TESTS=0'
    STR_CFG += ' -DBUILD_EXAMPLES=0'
    STR_CFG += ' -DBUILD_PYTHON_BINDINGS=0'
    STR_CFG += ' -DBUILD_MATLAB_BINDINGS=0'
    if(dict_config['static']):
        STR_CFG += " -DCMAKE_BUILD_STATIC_LIBS=1"
    else:
        STR_CFG += " -DCMAKE_BUILD_STATIC_LIBS=0"
    
    STR_CFG += ' -DLZ4_INCLUDE_DIR=' + install_dir + '/include/'
    STR_CFG += ' -DLZ4_LINK_LIBRARIES=' + install_dir + '/lib/lz4.lib'
    # STR_CFG += ' -DLZ4_LIBRARY=' + install_dir + '/lib/lz4.lib'
    # STR_CFG += ' -DLZ4_LIBRARY_DEBUG=' + install_dir + '/lib/lz4.lib'  
    
    source_dir = os.getcwd() + '/../prebuild/flann-master'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    