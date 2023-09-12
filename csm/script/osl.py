
from ._common import *
from ._config import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("zlib" , list_name,getDependency)
    list_name = addDependency("boost" , list_name,getDependency)
    list_name = addDependency("zstd" , list_name,getDependency)
    list_name = addDependency("openexr" , list_name,getDependency)
    list_name = addDependency("openimageio" , list_name,getDependency)
    list_name = addDependency("pugixml" , list_name,getDependency)
    list_name = addDependency("partio" , list_name,getDependency)
    list_name = addDependency("winflexbison" , list_name,getDependency)
    list_name = addDependency("pybind" , list_name,getDependency)
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/madler/zlib.git")
        # return
    
    install_dir = dict_config['install_dir'] + '/' + my_build_and_install_dir(dict_config)
    install_dir = install_dir.replace('\\','/')	
    
    STR_CFG = ''
    STR_CFG += ' -DOSL_BUILD_TESTS=0'
    STR_CFG += ' -DBOOST_CUSTOM=1'
    STR_CFG += ' -DUSE_LLVM=0'
    
    if(dict_config['static']):
        STR_CFG += " -DBUILD_SHARED_LIBS=0"
    else:
        STR_CFG += " -DBUILD_SHARED_LIBS=1"
    
    # Boost_VERSION, Boost_INCLUDE_DIRS, Boost_LIBRARY_DIRS, Boost_LIBRARIES.
    STR_CFG += ' -DBoost_INCLUDE_DIRS=' + install_dir + '/include/boost-1_82/'
    STR_CFG += ' -DBoost_LIBRARY_DIRS=' + install_dir + '/lib/'
    
    # llvm-project-llvmorg-15.0.7
    STR_CFG += ' -DLLVM_ROOT=' + LLVM_ROOT
    # STR_CFG += ' -DLLVM_ROOT=C:/LLVM/lib/cmake/llvm'
    # STR_CFG += ' -DLLVM_INCLUDES=C:/LLVM/include/'
    # STR_CFG += ' -DLLVM_DIRECTORY=C:/LLVM/'
    # STR_CFG += ' -DLLVM_LIB_DIR=C:/LLVM/lib/'
    
    # STR_CFG += ' -DLLVM_INCLUDES=C:/LLVM/include/'
    # STR_CFG += ' -DLLVM_LIBRARIES=C:/LLVM/lib/libclang.lib'
    
    source_dir = os.getcwd() + '/../prebuild/OpenShadingLanguage-main'
    # source_dir = os.getcwd() + '/../prebuild/OpenShadingLanguage-1.12.13.0'
    
    if(dict_config['release']==True):
        if(dict_config['dynamic']):
            if(ENABLE_LLVM and os.path.isdir(LLVM_ROOT)):
                configure(str_name,dict_config,STR_CFG,"",source_dir)
                build(str_name,dict_config)
                install(str_name,dict_config)
            
    if( not dict_config['static']):
        source_dir = install_dir + "/lib/"
        dist_dir = install_dir + '/bin/'
        print(source_dir)
        print(dist_dir)
        movefiles(source_dir,dist_dir+"/","*.dll")
    