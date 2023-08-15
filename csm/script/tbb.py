
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
    
    # STR_CFG = ''
    # if(dict_config['static']):
        # STR_CFG += " -DBUILD_SHARED_LIBS=0"
    # else:
        # STR_CFG += " -DBUILD_SHARED_LIBS=1"
        
    # source_dir = os.getcwd() + '/../prebuild/oneTBB-master'
    source_dir = os.getcwd() + '/../prebuild/oneTBB-2019_U6'
    
    # configure(str_name,dict_config,STR_CFG,"",source_dir)
    # build(str_name,dict_config)
    # install(str_name,dict_config)
    
    # cmd = "cd " + source_dir
    os.chdir( source_dir )
    
    cmd = "python build/build.py --msbuild --vs=2019 --install-devel --prefix=" + install_dir
    my_exec( cmd )
    
    os.chdir( CWD )
    
    