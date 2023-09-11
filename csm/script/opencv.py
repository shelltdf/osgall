
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("eigen" , list_name,getDependency)
    list_name = addDependency("vtk" , list_name,getDependency)
    list_name = addDependency("openexr" , list_name,getDependency)
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/madler/zlib.git")
        # return
    
    install_dir = dict_config['install_dir'] + '/' + my_build_and_install_dir(dict_config)
    install_dir = install_dir.replace('\\','/')	
    
    STR_CFG = ''
    # STR_CFG += ' -DOPENCV_FORCE_3RDPARTY_BUILD=1'
    STR_CFG += ' -DBUILD_TESTS=0'
    STR_CFG += ' -DBUILD_opencv_python3=0'
    STR_CFG += ' -DWITH_IPP=0'
    STR_CFG += ' -DWITH_FFMPEG=0'
    
    if(dict_config['static']):
        STR_CFG += " -DBUILD_SHARED_LIBS=0"
    else:
        STR_CFG += " -DBUILD_SHARED_LIBS=1"
        
    # STR_CFG += ' -Dminizip-ng_INCLUDE_DIR=' + install_dir + '/include/minizip/'
    
    # STR_CFG += ' -Dyaml-cpp_LIBRARY=' + install_dir + '/lib/yaml-cpp.lib'
    
    # STR_CFG += ' -Dlcms2_INCLUDE_DIR=' + install_dir + '/include/'
    # STR_CFG += ' -Dlcms2_LIBRARY=' + install_dir + '/lib/lcms2.lib'
        
    source_dir = os.getcwd() + '/../prebuild/opencv-4.x'
    # source_dir = os.getcwd() + '/../prebuild/opencv-2.4.13'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    source_dir = install_dir + "/x64/vc16/bin/"
    dist_dir = install_dir + "/bin/"
    print(source_dir)
    print(dist_dir)
    movefiles(source_dir,dist_dir,"*.dll")
    movefiles(source_dir,dist_dir,"*.exe")
    
    source_dir = install_dir + "/x64/vc16/lib/"
    dist_dir = install_dir + "/lib/"
    print(source_dir)
    print(dist_dir)
    movefiles(source_dir,dist_dir,"*.lib")
    movefiles(source_dir,dist_dir+"/cmake/opencv","*.cmake")
    