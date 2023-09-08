
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("eigen" , list_name,getDependency)
    list_name = addDependency("flann" , list_name,getDependency)
    list_name = addDependency("boost" , list_name,getDependency)
    list_name = addDependency("qhull" , list_name,getDependency)
    
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
    if(dict_config['static']):
        STR_CFG += " -DPCL_SHARED_LIBS=0"
    else:
        STR_CFG += " -DPCL_SHARED_LIBS=1"
    
    # STR_CFG += ' -DFLANN_LIBRARY=' + install_dir + '/lib/flann.lib'
    # STR_CFG += ' -DFLANN_LIBRARY_DEBUG=' + install_dir + '/lib/flann.lib'  
    
    # STR_CFG += ' -Dboost_system_DIR=' + install_dir + '/lib/cmake/boost_system-1.82.0'
    
    # STR_CFG += ' -DBoost_INCLUDE_DIRS=' + install_dir + '/include/boost-1_82/'
    # STR_CFG += ' -DBoost_LIBRARY_DIRS=' + install_dir + '/lib/'
    
    # system filesystem thread date_time iostreams
    # STR_CFG += ' -DBoost_SYSTEM_LIBRARY_RELEASE=' + install_dir + '/lib/flann.lib'
    # STR_CFG += ' -DBoost_SYSTEM_LIBRARY_DEBUG=' + install_dir + '/lib/flann.lib'  
    
    # STR_CFG += ' -DBoost_FILESYSTEM_LIBRARY_RELEASE=' + install_dir + '/lib/boost_filesystem-vc142-mt-x64-1_82.lib'
    # STR_CFG += ' -DBoost_FILESYSTEM_LIBRARY_DEBUG=' + install_dir + '/lib/boost_filesystem-vc142-mt-x64-1_82.lib'  
    
    # STR_CFG += ' -DBoost_THREAD_LIBRARY_RELEASE=' + install_dir + '/lib/boost_thread-vc142-mt-x64-1_82.lib'
    # STR_CFG += ' -DBoost_THREAD_LIBRARY_DEBUG=' + install_dir + '/lib/boost_thread-vc142-mt-x64-1_82.lib'  
    
    # STR_CFG += ' -DBoost_DATE_TIME_LIBRARY_RELEASE=' + install_dir + '/lib/boost_date_time-vc142-mt-x64-1_82.lib'
    # STR_CFG += ' -DBoost_DATE_TIME_LIBRARY_DEBUG=' + install_dir + '/lib/boost_date_time-vc142-mt-x64-1_82.lib'  
    
    # STR_CFG += ' -DBoost_IOSTREAMS_LIBRARY_RELEASE=' + install_dir + '/lib/boost_iostreams-vc142-mt-x64-1_82.lib'
    # STR_CFG += ' -DBoost_IOSTREAMS_LIBRARY_DEBUG=' + install_dir + '/lib/boost_iostreams-vc142-mt-x64-1_82.lib'  
    
    # source_dir = os.getcwd() + '/../prebuild/pcl-pcl-1.8.1'
    source_dir = os.getcwd() + '/../prebuild/pcl-master'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    