
# import git
# from git import Repo
import os
import sys
import glob
import shutil
from ._config import * 

# Debug;Release;MinSizeRel;RelWithDebInfo

CWD = ""
TITLE = ""

def my_exec( str_cmd ):
    print ("exec - "  + str_cmd)
    
    global TITLE
    
    if(True):
        if(len(TITLE)>0):
            os.system("title "+ TITLE)
        os.system(str_cmd)
    else:
        ps = subprocess.Popen(str_cmd)
        ps.wait()

def my_build_and_install_dir(dict_config):
    dir_name = ""
    dir_name += dict_config['arch']
    
    if(dict_config['static']==True):
        dir_name += '_static'
    if(dict_config['dynamic']==True):
        dir_name += '_dynamic'
        
    # if(dict_config['arch'][:2]=="vs"):
        # return dir_name
        
    if(dict_config['debug']==True):
        dir_name += '_debug'
    if(dict_config['debuginfo']==True):
        dir_name += '_debuginfo'
    if(dict_config['release']==True):
        dir_name += '_release'
    return dir_name
    
        
def my_into_build_dir( str_name , dict_config ):

    if(os.path.isdir("build")==False):
        os.system( "mkdir build" )
    os.chdir( "build" )
    
    dir_name = my_build_and_install_dir(dict_config)
        
    if(os.path.isdir(dir_name)==False):
        os.system( "mkdir " + dir_name)
    os.chdir( dir_name )
    
    if(os.path.isdir(str_name)==False):
        os.system( "mkdir " + str_name )
    os.chdir( str_name )
    
    
def my_out_build_dir( str_name ):
    # os.chdir( ".." )
    # os.chdir( ".." )
    # os.chdir( ".." )
    os.chdir( CWD )
    
    
def my_into_source_dir( str_name ):

    if(os.path.isdir("source")==False):
        os.system( "mkdir source" )
    os.chdir( "source" )
    
    if(os.path.isdir(str_name)==False):
        os.system( "mkdir " + str_name)
    os.chdir( str_name )
    
def my_out_source_dir():
    # os.chdir( ".." )
    # os.chdir( ".." )
    os.chdir( CWD )
    
    
def addDependency( str_name , list_name ,getDependency ):

    list_sn = []        
    list_sn += getDependency(str_name) 
    
    list_sn += [str_name]
    list_name = list_sn + list_name
    
    for a in range(0, len(list_name)):
        for b in range(a+1, len(list_name)):
            if list_name[a] == list_name[b] :
                list_name[b] = ""
    
    list_ret = []
    for a in range(0, len(list_name)):
        if list_name[a] != "":
            list_ret += [list_name[a]]
            
    return list_ret
    

def download_source(str_name , str_git_url , str_branch='master', str_tag=''):
    e = os.path.isdir("./source/" + str_name + "/.git") 
    if(e):
        print (str_name + " git is exist")
        
        os.chdir( "source/"+str_name )
        my_exec( "git fetch" )
        os.chdir( ".." )
        os.chdir( ".." )
        
    else:
        print ("clone git : " + str_git_url)
        # git.Git("./source/" + str_name).clone(str_git_url)
        # Repo.clone_from(str_git_url, "./source/" + str_name ,branch=str_branch)
        
        # my_into_source_dir( str_name )
        
        cmd = "git clone --recursive --branch " + str_branch + " " + str_git_url +" ./source/"+str_name 
        my_exec( cmd )
        
        # my_out_source_dir()
    

    os.chdir( "source/"+str_name )
    
    # git checkout tag_name
    if(str_tag!=''):
        cmd = "git checkout " + str_tag
        my_exec( cmd) 
    
    my_exec( "git submodule update --init" )
    os.chdir( ".." )
    os.chdir( ".." )
        
    
def configure(str_name ,dict_config, str_config = "",str_subdir="",str_local_dir="" , force_static = False):
    my_into_build_dir( str_name ,dict_config )
    dir_name = my_build_and_install_dir(dict_config)
    
    global CWD

    BUILD_TYPE = ""
    BUILD_STATIC_LIB = ""
    if(dict_config['static']==True):
        BUILD_STATIC_LIB = " -DBUILD_SHARED_LIBS=0"
    if(dict_config['dynamic']==True):
        BUILD_STATIC_LIB = " -DBUILD_SHARED_LIBS=1"
        
    if(force_static):
        BUILD_STATIC_LIB = " -DBUILD_SHARED_LIBS=0"
    
    if(dict_config['arch'][:2]!="vs"):
        if(dict_config['debug']==True):
            BUILD_TYPE = ' -DCMAKE_BUILD_TYPE=Debug'
        if(dict_config['debuginfo']==True):
            BUILD_TYPE = ' -DCMAKE_BUILD_TYPE=RelWithDebInfo'
        if(dict_config['release']==True):
            BUILD_TYPE = ' -DCMAKE_BUILD_TYPE=Release'
        
    source_dir = str_local_dir
    if(source_dir == ""):
        source_dir = "../../../source/"
    
    # e = os.path.isfile("CMakeCache.txt") 
    # if(e):
        # print str_name + "configure is exist"
    # else:
    # my_exec( "cmake "+source_dir+"/" + str_name + "/" + str_subdir +
    
    if(dict_config['arch']=="ndk"):
        # pass
        cmake_string = "cmake " + source_dir
        cmake_string += ' -DCMAKE_TOOLCHAIN_FILE=' + ANDROID_NDK_PATH + '/build/cmake/android.toolchain.cmake'+ dict_config['cmake_cfg']
        cmake_string += ' -DCMAKE_INSTALL_PREFIX="' + dict_config['install_dir'] + '/' + dir_name + '"'
        cmake_string += ' -DANDROID_ABI=' + ANDROID_ABI
        cmake_string += ' -DANDROID_NATIVE_API_LEVEL=' + str(ANDROID_API_LEVEL)
        
        # cmake_string += ' -DANDROID'
        cmake_string += ' -DJ=8' #+ str(CPU_NUM)
        # cmake_string += " -DCMAKE_ANDROID_API="+str(ANDROID_API_LEVEL)
        # cmake_string += " -DCMAKE_ANDROID_API_MIN="+str(ANDROID_API_LEVEL)
        # cmake_string += ' -DANDROID_NDK=' + ANDROID_NDK_PATH
        
        if(dict_config['dynamic']==True):
            # cmake_string += ' -DANDROID_STL=gnustl_shared '
            cmake_string += ' -DANDROID_STL=c++_shared '
        else:
            # cmake_string += ' -DANDROID_STL=gnustl_static '
            cmake_string += ' -DANDROID_STL=c++_static '
        
        if(dict_config['debug']==True):
            cmake_string += ' -DCMAKE_BUILD_TYPE=Debug'        
        if(dict_config['debuginfo']==True):
            cmake_string += ' -DCMAKE_BUILD_TYPE=RelWithDebInfo'
        if(dict_config['release']==True):
            cmake_string += ' -DCMAKE_BUILD_TYPE=Release'
            
        cmake_string += str_config
        my_exec(cmake_string)
        
    elif(dict_config['arch']=="em"):
        my_exec( "emcmake cmake "+source_dir+
        " -DCMAKE_USE_RELATIVE_PATHS=1 -DCMAKE_INSTALL_PREFIX='" + dict_config['install_dir'] + '/' + dir_name + "' " +
        dict_config['cmake_cfg'] + BUILD_TYPE + BUILD_STATIC_LIB + str_config )
    else:
        my_exec( "cmake "+source_dir+
        " -DCMAKE_USE_RELATIVE_PATHS=1" +
        # " -DCMAKE_CODEBLOCKS_MAKE_ARGUMENTS=-j2" +
        # " -DCMAKE_SYSROOT=" + CWD + "'/rootfs'" +
        " -DCMAKE_INSTALL_PREFIX='" + dict_config['install_dir'] + '/' + dir_name + "' " +
        dict_config['cmake_cfg'] + BUILD_TYPE + BUILD_STATIC_LIB + str_config )
    
    my_out_build_dir( str_name )
    
    
def build(str_name,dict_config):
    my_into_build_dir( str_name ,dict_config)
    current_dir = os.getcwd()
    system_ret = 0
    if(dict_config['arch'][:2]=="vs"):
        if(dict_config['debug']==True):
            system_ret = os.system('msbuild ALL_BUILD.vcxproj -maxcpucount:4 /p:Configuration=Debug')
            # system_ret = os.system('msbuild ALL_BUILD.vcxproj /p:Configuration=Debug')
        if(dict_config['debuginfo']==True):
            system_ret = os.system('msbuild ALL_BUILD.vcxproj -maxcpucount:4 /p:Configuration=RelWithDebInfo')
            # system_ret = os.system('msbuild ALL_BUILD.vcxproj /p:Configuration=RelWithDebInfo')
        if(dict_config['release']==True):
            system_ret = os.system('msbuild ALL_BUILD.vcxproj -maxcpucount:4 /p:Configuration=Release')
            # system_ret = os.system('msbuild ALL_BUILD.vcxproj /p:Configuration=Release')
    elif(dict_config['arch']=="nmake"):
        system_ret = os.system('nmake')
    elif(dict_config['arch']=="unix"):
        system_ret = os.system('make -j 32')
    elif(dict_config['arch']=="ninja"):
        system_ret = os.system('ninja -j 32')
    elif(dict_config['arch']=="ndk"):
        # system_ret = os.system('nmake')
        # os.system('set CL=/MP & nmake')
        system_ret = os.system('ninja -j 32')
    elif(dict_config['arch']=="em"):
        # system_ret = os.system('set CL=/MP nmake')
        system_ret = os.system('ninja -j 32')
    else:
        system_ret = os.system('make')
    my_out_build_dir( str_name )
    
    
    # error
    global CWD
    if(system_ret != 0):
        # print("error = " + CWD)
        print("error = " + current_dir)
        sys.exit(1)
        
    
def install(str_name,dict_config):
    my_into_build_dir( str_name ,dict_config)
    if(dict_config['arch'][:2]=="vs"):
        if(dict_config['debug']==True):
            os.system('msbuild INSTALL.vcxproj /p:Configuration=Debug')
        if(dict_config['debuginfo']==True):
            os.system('msbuild INSTALL.vcxproj /p:Configuration=RelWithDebInfo')
        if(dict_config['release']==True):
            os.system('msbuild INSTALL.vcxproj /p:Configuration=Release')
    elif(dict_config['arch']=="nmake"):
        os.system('nmake install')
    elif(dict_config['arch']=="unix"):
        os.system('make install')
    elif(dict_config['arch']=="ninja"):
        os.system('ninja install')
    elif(dict_config['arch']=="ndk"):
        # os.system('nmake install')
        # os.system('set CL=/MP & nmake install')
        os.system('ninja install')
    elif(dict_config['arch']=="em"):
        # os.system('nmake install')
        os.system('ninja install')
    else:
        os.system('make install')
    my_out_build_dir( str_name )
    pass
    
    
    
def copyfiles(str_source,str_dist,PATTERN):

    # sourcePath = r'E:\some\path'
    # destPath = r'E:\some\other\path'
    for root, dirs, files in os.walk(str_source):

        #figure out where we're going
        dest = str_dist + root.replace(str_source, '')

        #if we're in a directory that doesn't exist in the destination folder
        #then create a new folder
        if not os.path.isdir(dest):
            os.mkdir(dest)
            print ('Directory created at: ' + dest)

        #loop through all files in the directory
        for f in files:
            if glob.fnmatch.fnmatch(f, PATTERN):
                print (f)
                
                #compute current (old) & new file locations
                oldLoc = root + '\\' + f
                newLoc = dest + '\\' + f

                if not os.path.isfile(newLoc):
                    try:
                        shutil.copy2(oldLoc, newLoc)
                        print ('File ' + f + ' copied.')
                    except IOError:
                        print ('file "' + f + '" already exists')
                        
                        
                        
                    
                    