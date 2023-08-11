
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("zlib" , list_name,getDependency)
    # list_name = addDependency("ilmbase" , list_name,getDependency)
    list_name = addDependency("imath" , list_name,getDependency)
    list_name = addDependency("libdeflate" , list_name,getDependency)
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/madler/zlib.git")
        # return
        
    if(dict_config['arch']=="em"):
        return
        
    install_dir = dict_config['install_dir'] + '/' + my_build_and_install_dir(dict_config)
    
    
    STR_CFG = ''
    STR_CFG += ' -DOPENEXR_INSTALL_EXAMPLES=0'
    STR_CFG += ' -DBUILD_TESTING=0'
    STR_CFG += ' -DOPENEXR_INSTALL_EXAMPLES=0'
    
    STR_CFG += ' -Ddeflate_FOUND=1'
    
    if(dict_config['arch'][:2]=="vs"):
        STR_CFG += ' -DNAMESPACE_VERSIONING=OFF'
        STR_CFG += ' -DILMBASE_PACKAGE_PREFIX=' + install_dir
        STR_CFG += ' -DPACK_BINARY_NSIS=OFF'
        
        if(dict_config['static']):
            # STR_CFG = " -DBUILD_SHARED_LIBS=0"
            STR_CFG += ' -DZLIB_LIBRARY=' + install_dir + '/lib/zlibstatic.lib'
            STR_CFG += ' -DZLIB_LIBRARY_DEBUG=' + install_dir + '/lib/zlibstaticd.lib'
            # if(dict_config['debug']==True):
                # STR_CFG += ' -DZLIB_LIBRARY_DEBUG=' + install_dir + '/lib/zlibstaticd.lib'
            # else:
                # STR_CFG += ' -DZLIB_LIBRARY_DEBUG=' + install_dir + '/lib/zlibstatic.lib'
        else:
            # STR_CFG = " -DBUILD_SHARED_LIBS=1"
            STR_CFG += ' -DZLIB_LIBRARY=' + install_dir + '/lib/zlib.lib'
            STR_CFG += ' -DZLIB_LIBRARY_DEBUG=' + install_dir + '/lib/zlibd.lib'
            # if(dict_config['debug']==True):
                # STR_CFG += ' -DZLIB_LIBRARY_DEBUG=' + install_dir + '/lib/zlibd.lib'
            # else:
                # STR_CFG += ' -DZLIB_LIBRARY_DEBUG=' + install_dir + '/lib/zlib.lib'
                
            STR_CFG += ' -DEXR_DEFLATE_LIB=' + install_dir + '/lib/deflate.lib'
            # STR_CFG += ' -DOPENEXR_LIBRARY=' + install_dir + '/lib/deflate.lib'

    if(dict_config['arch']=="unix"):
        STR_CFG += ' -DNAMESPACE_VERSIONING=ON'
        STR_CFG += ' -DILMBASE_PACKAGE_PREFIX=' + install_dir
        STR_CFG += ' -DPACK_BINARY_NSIS=OFF'
        
        if(dict_config['static']):
            # STR_CFG = " -DBUILD_SHARED_LIBS=0"
            STR_CFG += ' -DZLIB_LIBRARY=' + install_dir + '/lib/zlibstatic.lib'
            STR_CFG += ' -DZLIB_LIBRARY_DEBUG=' + install_dir + '/lib/zlibstaticd.lib'
        else:
            # STR_CFG = " -DBUILD_SHARED_LIBS=1"
            if(dict_config['release']==True):
                STR_CFG += ' -DZLIB_LIBRARY=' + install_dir + '/lib/libz.so'
                STR_CFG += ' -DZLIB_LIBRARY_DEBUG=' + install_dir + '/lib/lib.so'
            else:
                STR_CFG += ' -DZLIB_LIBRARY=' + install_dir + '/lib/libzd.so'
                STR_CFG += ' -DZLIB_LIBRARY_DEBUG=' + install_dir + '/lib/libz.so'
    
    if(dict_config['arch']=="ndk"):
        return
        
    if(dict_config['arch']=="em"):
        STR_CFG += ' -DNAMESPACE_VERSIONING=OFF'
        STR_CFG += ' -DILMBASE_PACKAGE_PREFIX=' + install_dir
        STR_CFG += ' -DPACK_BINARY_NSIS=OFF'

    
    # source_dir = os.getcwd() + '/../prebuild/openexr-2.2.0'
    source_dir = os.getcwd() + '/../prebuild/openexr-main'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    