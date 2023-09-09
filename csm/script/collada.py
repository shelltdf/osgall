
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("boost" , list_name,getDependency)
    list_name = addDependency("bzip2" , list_name,getDependency)
    list_name = addDependency("libxml2" , list_name,getDependency)
    list_name = addDependency("zlib" , list_name,getDependency)
    list_name = addDependency("minizip" , list_name,getDependency)
    list_name = addDependency("lz4" , list_name,getDependency)
    # list_name = addDependency("libpcre" , list_name,getDependency) #这个无法支持
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/winlibs/freetype.git")
        # download_source(str_name,"https://github.com/shelltdf/freetype.git")
        
        # return
        
    dir_name = my_build_and_install_dir(dict_config)
    install_dir = dict_config['install_dir'] + '/' + my_build_and_install_dir(dict_config)
    
    STR_CFG = ''
    if(dict_config['static']):
        STR_CFG += " -DBUILD_SHARED_LIBS=0"
    else:
        STR_CFG += " -DBUILD_SHARED_LIBS=1"
        
    
    STR_CFG += " -Dminizip_INCLUDE_DIRS='" + install_dir + "/include'"
    
    if(dict_config['release'] or dict_config['debuginfo']):
        STR_CFG += " -DLIBXML2_LIBRARY='" + install_dir + "/lib/libxml2.lib'"
    if(dict_config['debug']):
        STR_CFG += " -DLIBXML2_LIBRARY='" + install_dir + "/lib/libxml2d.lib'"
        
    # if(dict_config['arch'][:2]=="vs"):
        # STR_CFG += '-DFREEGLUT_BUILD_DEMOS=0 -DINSTALL_PDB=0'
        # if(dict_config['static']):
            # STR_CFG += " -DFREEGLUT_BUILD_STATIC_LIBS=1"
            # STR_CFG += " -DFREEGLUT_BUILD_SHARED_LIBS=0"
        # else:
            # STR_CFG += " -DFREEGLUT_BUILD_SHARED_LIBS=1"
            # STR_CFG += " -DFREEGLUT_BUILD_STATIC_LIBS=0"
    
    # if(dict_config['arch']=="ndk"):
        # return
        
    # if(dict_config['arch']=="em"):
        # STR_CFG += '-DFREEGLUT_BUILD_DEMOS=0 -DINSTALL_PDB=0'
        # STR_CFG += " -DFREEGLUT_BUILD_STATIC_LIBS=1"
        # STR_CFG += " -DFREEGLUT_BUILD_SHARED_LIBS=0"
    
    source_dir = os.getcwd() + '/../prebuild/collada-dom-2.4.0'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    