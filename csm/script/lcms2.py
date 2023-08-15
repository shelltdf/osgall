
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
    # STR_CFG += ' -DBUILD_TESTS=0'
    # if(dict_config['static']):
        # STR_CFG += " -DBUILD_STATIC=1"
        # STR_CFG += " -DBUILD_SHARED=0"
    # else:
        # STR_CFG += " -DBUILD_STATIC=0"
        # STR_CFG += " -DBUILD_SHARED=1"
        
    source_dir = os.getcwd() + '/../prebuild/Little-CMS-master'
    
    # configure(str_name,dict_config,STR_CFG,"",source_dir)
    # build(str_name,dict_config)
    # install(str_name,dict_config)
    
    # cmd = "python build/build.py --msbuild --vs=2019 --install-devel --prefix=" + install_dir
    # my_exec( cmd )
    
    os.chdir( dict_config['install_dir'] )
    if not os.path.isdir(my_build_and_install_dir(dict_config)):
        os.mkdir( my_build_and_install_dir(dict_config) );
    
    os.chdir( source_dir )
    
    if(dict_config['static']):
        vcxproj = "Projects/VC2019/lcms2_static/lcms2_static.vcxproj"
    else:
        vcxproj = "Projects/VC2019/lcms2_DLL/lcms2_DLL.vcxproj"
    
    system_ret = ""
    if(dict_config['arch'][:2]=="vs"):
        if(dict_config['debug']==True):
            system_ret = os.system('msbuild ' + vcxproj + ' -maxcpucount:16 /p:Configuration=Debug')
        if(dict_config['debuginfo']==True):
            system_ret = os.system('msbuild ' + vcxproj + ' -maxcpucount:16 /p:Configuration=RelWithDebInfo')
        if(dict_config['release']==True):
            system_ret = os.system('msbuild ' + vcxproj + ' -maxcpucount:16 /p:Configuration=Release')
    
    if(system_ret == 0):
        # pass
        
        # source_dir = my_build_and_install_dir(dict_config)
        source_dir = os.getcwd() + "/bin/"
        dist_dir = install_dir + '/bin/'
        print(source_dir)
        print(dist_dir)
        copyfiles(source_dir,dist_dir+"/","*.dll")
        
        source_dir = os.getcwd() + "/bin/"
        dist_dir = install_dir + '/lib/'
        print(source_dir)
        print(dist_dir)
        copyfiles(source_dir,dist_dir+"/","*.lib")
        
        source_dir = os.getcwd() + "/include/"
        dist_dir = install_dir + '/include/'
        print(source_dir)
        print(dist_dir)
        copyfiles(source_dir,dist_dir+"/","*.h")
        
    os.chdir( CWD )

    if(system_ret != 0):
        # print("error = " + CWD)
        print("error = " + source_dir)
        sys.exit(1)
        