
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("libtiff" , list_name,getDependency)
    list_name = addDependency("proj4" , list_name,getDependency)
    list_name = addDependency("zlib" , list_name,getDependency)

    return list_name + [str_name]
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):

    # if(b_only_download):
        # download_source(str_name,"https://github.com/smanders/libgeotiff.git")
        # download_source(str_name,"https://github.com/shelltdf/libgeotiff.git")
        
        # return
        
    install_dir = os.getcwd().replace('\\','/') +"/install/"+ my_build_and_install_dir(dict_config)
    
    STR_CFG = ''
    
    if(dict_config['arch'][:2]=="vs"):
        if(dict_config['static']):
            STR_CFG += " -DBUILD_STATIC_LIBRARY=1"
            STR_CFG += " -DWITH_UTILITIES=0";
            STR_CFG += ' -DPROJ_LIBRARY=' + install_dir + '/lib/proj_6_1.lib'
            STR_CFG += ' -DPROJ_LIBRARY_DEBUG=' + install_dir + '/lib/proj_6_1_d.lib'
            STR_CFG += ' -DZLIB_LIBRARY=' + install_dir + '/lib/zlibstatic.lib'
            STR_CFG += ' -DZLIB_LIBRARY_DEBUG=' + install_dir + '/lib/zlibstaticd.lib'
        else:
            STR_CFG += " -DBUILD_STATIC_LIBRARY=0"
            STR_CFG += ' -DPROJ_LIBRARY=' + install_dir + '/lib/proj_6_1.lib'
            STR_CFG += ' -DPROJ_LIBRARY_DEBUG=' + install_dir + '/lib/proj_6_1_d.lib'
            STR_CFG += ' -DZLIB_LIBRARY=' + install_dir + '/lib/zlib.lib'
            STR_CFG += ' -DZLIB_LIBRARY_DEBUG=' + install_dir + '/lib/zlibd.lib'
            
    if(dict_config['arch']=="em"):
        STR_CFG += " -DBUILD_STATIC_LIBRARY=1"
        STR_CFG += " -DWITH_UTILITIES=0";
        STR_CFG += ' -DPROJ_INCLUDE_DIR="' + install_dir + '"/include'
        STR_CFG += ' -DPROJ_LIBRARY=' + install_dir + '/lib/libproj.a'
        STR_CFG += ' -DTIFF_INCLUDE_DIR="' + install_dir + '"/include'
        STR_CFG += ' -DTIFF_LIBRARY=' + install_dir + '/lib/libtiff.a'
        
    source_dir = os.getcwd() + '/../prebuild/libgeotiff'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    