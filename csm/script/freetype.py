
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("libpng" , list_name,getDependency)
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/winlibs/freetype.git")
        # download_source(str_name,"https://github.com/shelltdf/freetype.git")
        
        # return
        
    STR_CFG = ''
    if(dict_config['arch'][:2]=="vs"):
        STR_CFG += '-DFREEGLUT_BUILD_DEMOS=0 -DINSTALL_PDB=0'
        if(dict_config['static']):
            STR_CFG += " -DFREEGLUT_BUILD_STATIC_LIBS=1"
            STR_CFG += " -DFREEGLUT_BUILD_SHARED_LIBS=0"
        else:
            STR_CFG += " -DFREEGLUT_BUILD_SHARED_LIBS=1"
            STR_CFG += " -DFREEGLUT_BUILD_STATIC_LIBS=0"
    
    if(dict_config['arch']=="em"):
        STR_CFG += '-DFREEGLUT_BUILD_DEMOS=0 -DINSTALL_PDB=0'
        STR_CFG += " -DFREEGLUT_BUILD_STATIC_LIBS=1"
        STR_CFG += " -DFREEGLUT_BUILD_SHARED_LIBS=0"
    
    source_dir = os.getcwd() + '/../prebuild/freetype-2.5.1'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    