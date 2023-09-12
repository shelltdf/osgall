
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("openexr" , list_name,getDependency)
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/madler/zlib.git")
        # return
    
    install_dir = dict_config['install_dir'] + '/' + my_build_and_install_dir(dict_config)
    
    STR_CFG = ''
    if(dict_config['static']):
        STR_CFG += " -DALEMBIC_SHARED_LIBS=0"
    else:
        STR_CFG += " -DALEMBIC_SHARED_LIBS=1"
        
    source_dir = os.getcwd() + '/../prebuild/alembic-master'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    if( not dict_config['static']):
        source_dir = install_dir + "/lib/"
        dist_dir = install_dir + '/bin/'
        print(source_dir)
        print(dist_dir)
        movefiles(source_dir,dist_dir+"/","*.dll")
        
        