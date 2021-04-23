
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/castano/nvidia-texture-tools.git")
        # download_source(str_name,"https://github.com/shelltdf/nvidia-texture-tools.git")
        # return
        
    STR_CFG = ''
    if(dict_config['static']):
        STR_CFG += ' -DNVTT_SHARED=0'
    else:
        # STR_CFG += ' -DNVTHREAD_SHARED=1'
        # STR_CFG += ' -DNVCORE_SHARED=1'
        # STR_CFG += ' -DNVMATH_SHARED=1'
        # STR_CFG += ' -DNVIMAGE_SHARED=1'
        STR_CFG += ' -DNVTT_SHARED=1'
        STR_CFG += ' -DNVTT_EXTERN=0'
        
        STR_CFG += ' -DHAVE_JPEG=0'
        STR_CFG += ' -DHAVE_MAYA=0'
        STR_CFG += ' -DHAVE_PNG=0'
        STR_CFG += ' -DHAVE_TIFF=0'
        STR_CFG += ' -DHAVE_OPENEXR=0'
    
    source_dir = os.getcwd() + '/../prebuild/nvidia-texture-tools'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    