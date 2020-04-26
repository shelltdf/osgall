
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/madler/zlib.git")
        # return
        
    
    dist_dir = my_build_and_install_dir(dict_config)
    dist_dir = os.getcwd() + '/install/' + dist_dir
    # source_dir = my_build_and_install_dir(dict_config)
    # source_dir = os.getcwd() + "/source/" + str_name
    source_dir = os.getcwd() + '/../prebuild/rapidxml-1.13'
    print (source_dir)
    print (dist_dir)
    copyfiles(source_dir,dist_dir+"/include","*.hpp")
    