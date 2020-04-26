
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("sqlite3" , list_name,getDependency)
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/OSGeo/proj.4.git")
        # download_source(str_name,"https://github.com/shelltdf/proj.4.git")
        
        # return
        
    STR_CFG = " -DPROJ_TESTS=0";
    STR_CFG += ' -DSQLITE3_LIBRARY=' +os.getcwd() +"/install/"+ my_build_and_install_dir(dict_config) + '/lib/sqlite.lib'
    STR_CFG += ' -DSQLITE3_LIBRARY_DEBUG=' +os.getcwd() +"/install/"+ my_build_and_install_dir(dict_config) + '/lib/sqlited.lib'
    if(dict_config['static']):
        STR_CFG += " -DBUILD_LIBPROJ_SHARED=0"
    else:
        STR_CFG += " -DBUILD_LIBPROJ_SHARED=1"
        
    source_dir = os.getcwd() + '/../prebuild/proj-6.1.1'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    