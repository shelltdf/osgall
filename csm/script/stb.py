
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/nothings/stb.git")
        # return
        
    # STR_CGG = ''
    # if(dict_config['static']):
        # STR_CGG += ''
    # else:
        # STR_CGG += ''
    
    # configure(str_name,dict_config)
    # build(str_name,dict_config)
    # install(str_name,dict_config)
    
    install_dir = dict_config['install_dir'] + '/' + my_build_and_install_dir(dict_config)
    # dist_dir = my_build_and_install_dir(dict_config)
    # dist_dir = os.getcwd() + '/install/' + dist_dir
    # source_dir = my_build_and_install_dir(dict_config)
    # source_dir = os.getcwd() + "/source/" + str_name
    source_dir = os.getcwd() + '/../prebuild/stb-master'
    print (source_dir)
    print (install_dir)
    copyfiles(source_dir,install_dir+"/include","*.h")
    
    