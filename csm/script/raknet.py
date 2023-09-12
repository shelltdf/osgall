
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
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
        STR_CFG += ' -DRAKNET_ENABLE_SAMPLES=0'
        STR_CFG += ' -DRAKNET_ENABLE_STATIC=1'
        STR_CFG += ' -DRAKNET_ENABLE_DLL=0'
    else:
        STR_CFG += ' -DRAKNET_ENABLE_SAMPLES=0'
        STR_CFG += ' -DRAKNET_ENABLE_STATIC=0'
        STR_CFG += ' -DRAKNET_ENABLE_DLL=1'
        
    if(dict_config['arch']=="ndk"):
        if(dict_config['static']):
            pass
            # STR_CFG += " -DGDAL_BUILD_SHARED=0"
            # STR_CFG += " -DGDAL_BUILD_APPS=0"
        else:
            pass
            # STR_CFG += " -DGDAL_BUILD_SHARED=1"
            # STR_CFG += ' -DGDAL_BUILD_APPS=0'
        
    source_dir = os.getcwd() + '/../prebuild/RakNet-master'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    if( not dict_config['static']):
        source_dir = install_dir + "/lib/"
        dist_dir = install_dir + '/bin/'
        print(source_dir)
        print(dist_dir)
        movefiles(source_dir,dist_dir+"/","*.dll")
        