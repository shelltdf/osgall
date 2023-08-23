
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("freetype" , list_name,getDependency)
    list_name = addDependency("vtk" , list_name,getDependency)
    list_name = addDependency("tcl" , list_name,getDependency)
    list_name = addDependency("tk" , list_name,getDependency)
    # list_name = addDependency("draco" , list_name,getDependency)
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/madler/zlib.git")
        # return
    
    install_dir = dict_config['install_dir'] + '/' + my_build_and_install_dir(dict_config)
    install_dir = install_dir.replace('\\','/')	
    
    STR_CFG = ''
    STR_CFG += ' -DBUILD_TESTING=0'
    # STR_CFG += ' -DUSE_TCL=0'
    # STR_CFG += ' -DCAN_USE_TCL=0'
    STR_CFG += ' -DUSE_TK=0'
    # STR_CFG += ' -DCAN_USE_TK=0'
    STR_CFG += ' -DUSE_VTK=1'
    # STR_CFG += ' -DCAN_USE_VTK=1'
    STR_CFG += ' -DUSE_DRACO=0'
    if(dict_config['static']):
        # STR_CFG += " -DBUILD_SHARED_LIBS=0"
        STR_CFG += " -DBUILD_LIBRARY_TYPE=Static"
    else:
        # STR_CFG += " -DBUILD_SHARED_LIBS=1"
        STR_CFG += " -DBUILD_LIBRARY_TYPE=Shared"
    
    STR_CFG += ' -D3RDPARTY_DRACO_DIR=' + install_dir + '/lib/draco/cmake/'
    STR_CFG += ' -D3RDPARTY_DRACO_INCLUDE_DIR=' + install_dir + '/include/'
    
    if(dict_config['static']):
        pass
    else:
        STR_CFG += ' -D3RDPARTY_FREETYPE_DLL=' + install_dir + '/bin/freetype.dll'
        STR_CFG += ' -D3RDPARTY_FREETYPE_DLL_DIR=' + install_dir + '/bin/'
    
    if(dict_config['release']==True):    
        STR_CFG += ' -D3RDPARTY_FREETYPE_LIBRARY=' + install_dir + '/lib/freetype.lib'
        STR_CFG += ' -DFREETYPE_NAME=freetype'
    else:
        STR_CFG += ' -D3RDPARTY_FREETYPE_LIBRARY=' + install_dir + '/lib/freetyped.lib'
        STR_CFG += ' -DFREETYPE_NAME=freetyped'
        
    STR_CFG += ' -D3RDPARTY_FREETYPE_LIBRARY_DIR=' + install_dir + '/lib/'
    
    source_dir = os.getcwd() + '/../prebuild/OCCT-master'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    if(dict_config['release']==True):
        source_dir = install_dir + "/win64/vc14/bin/"
        dist_dir = install_dir + "/bin/"
        print(source_dir)
        print(dist_dir)
        movefiles(source_dir,dist_dir,"*.dll")
        movefiles(source_dir,dist_dir,"*.exe")
        
        source_dir = install_dir + "/win64/vc14/lib/"
        dist_dir = install_dir + "/lib/"
        print(source_dir)
        print(dist_dir)
        movefiles(source_dir,dist_dir,"*.lib")
    else:
        source_dir = install_dir + "/win64/vc14/bind/"
        dist_dir = install_dir + "/bin/"
        print(source_dir)
        print(dist_dir)
        movefiles(source_dir,dist_dir,"*.dll")
        movefiles(source_dir,dist_dir,"*.exe")
        
        source_dir = install_dir + "/win64/vc14/libd/"
        dist_dir = install_dir + "/lib/"
        print(source_dir)
        print(dist_dir)
        movefiles(source_dir,dist_dir,"*.lib")
        