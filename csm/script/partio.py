
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("freeglut" , list_name,getDependency)
    list_name = addDependency("zlib" , list_name,getDependency)
    
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
    # if(dict_config['static']):
        # STR_CFG += " -DPARTIO_BUILD_SHARED_LIBS=0"
    # else:
        # STR_CFG += " -DPARTIO_BUILD_SHARED_LIBS=1"
    
    STR_CFG += " -DPARTIO_BUILD_SHARED_LIBS=0" # only staitc
    
    # STR_CFG += ' -DGLUT_INCLUDE_DIR=' + install_dir + '/include/'
    # STR_CFG += ' -DGLUT_glut_LIBRARY_RELEASE=' + install_dir + '/lib/freeglut_static.lib'
    # STR_CFG += ' -DGLUT_glut_LIBRARY_DEBUG=' + install_dir + '/lib/freeglut_static.lib'
    
    # STR_CFG += ' -DGLUT_FOUND=0'
    # STR_CFG += ' -DGLUT_INCLUDE_DIR=NOTFOUND'
    # STR_CFG += ' -DGLUT_glut_LIBRARY_RELEASE=NOTFOUND'
    # STR_CFG += ' -DGLUT_glut_LIBRARY_DEBUG=NOTFOUND'
    
    source_dir = os.getcwd() + '/../prebuild/partio-main'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    