#!/usr/bin/python
# -*- coding: utf-8 -*-

import os.path
import sys
from pprint import pprint
import script._common

def findScript(str_name):

    #find script
    e = os.path.isfile("./script/" + str_name + ".py") 
    if(e == False):
        print (str_name + " is not exist")
        return False
    
    #import script
    s = __import__('script.' + str_name)
    val = getattr(s, str_name)

    return val
    
def getDependency( str_name ):
    val = findScript(str_name)
    
    list_name = val.getDependency( str_name ,getDependency )
    return list_name;


def getLibrary( str_name , b_only_download ,dict_config ):
    val = findScript(str_name)
    
    #do script
    val.SBI( str_name , b_only_download ,dict_config, getLibrary )
    

def main():
    # print "start"
    
    script._common.CWD = os.getcwd()
    
    ARG_CMD = "" # "source" / "install" 
    ARG_NAME = ""
    ARG_ARCH = ""
    ARG_INSTALL_DIR = "c:/dev"
    ARG_RELEASE = True      #Release
    ARG_DEBUG = False       #Debug
    # ARG_MINSIZE = False     #MinSizeRel
    ARG_DEBUGINFO = False   #RelWithDebInfo
    ARG_DYNAMIC = True
    ARG_STATIC = False
    ARG_ONLIY = False
    
    # if len(sys.argv) >= 3:
        # ARG_CMD = sys.argv[1]
        # ARG_NAME = sys.argv[2]
    # else:
        # return
        
    for arg_num in range(len(sys.argv)):
        if sys.argv[arg_num].strip('\r') == "source" or sys.argv[arg_num].strip('\r') == "install":
            ARG_CMD = sys.argv[arg_num].strip('\r')
            if(arg_num < len(sys.argv)-1):
                ARG_NAME = sys.argv[arg_num+1]
        if sys.argv[arg_num].strip('\r') == "-a" or sys.argv[arg_num].strip('\r') == "-arch":
            if(arg_num < len(sys.argv)-1):
                ARG_ARCH = sys.argv[arg_num+1].strip('\r')
        if sys.argv[arg_num].strip('\idir') == "-a" or sys.argv[arg_num].strip('\r') == "-install_dir":
            if(arg_num < len(sys.argv)-1):
                ARG_INSTALL_DIR = sys.argv[arg_num+1].strip('\r')
        if sys.argv[arg_num].strip('\r') == "-release":
            ARG_RELEASE = True
        if sys.argv[arg_num].strip('\r') == "-debug":
            ARG_DEBUG = True        
        if sys.argv[arg_num].strip('\r') == "-debuginfo":
            ARG_DEBUGINFO = True
        if sys.argv[arg_num].strip('\r') == "-dynamic":
            ARG_DYNAMIC = True
        if sys.argv[arg_num].strip('\r') == "-static":
            ARG_STATIC = True
        if sys.argv[arg_num].strip('\r') == "-only":
            ARG_ONLIY = True    
                
                
    # ARG_NAME = ARG_NAME.strip('\r')
    # ARG_ARCH = ARG_ARCH.strip('\r')
    
    print( "ARG_CMD=" + ARG_CMD )
    print( "ARG_NAME=" + ARG_NAME )
    print( "ARG_ARCH=" + ARG_ARCH )
    print( "ARG_INSTALL_DIR=" + ARG_INSTALL_DIR )
    print( "ARG_RELEASE=" + str(ARG_RELEASE) )
    print( "ARG_DEBUG=" + str(ARG_DEBUG) )
    print( "ARG_DEBUGINFO=" + str(ARG_DEBUGINFO) )
    print( "ARG_DYNAMIC=" + str(ARG_DYNAMIC) )
    print( "ARG_STATIC=" + str(ARG_STATIC) )
    print( "ARG_ONLIY=" + str(ARG_ONLIY) )
    
    
    if len(ARG_CMD)>0 and len(ARG_NAME)>0 and ARG_CMD=="source":
        pass
    elif len(ARG_CMD)>0 and len(ARG_NAME)>0 and len(ARG_ARCH)>0:
        pass
    else:
        print ("cmake-git.py source zlib ")
        print ("cmake-git.py install zlib -arch vs2017-32 [-release] [-debug] [-dynamic] [-static]")
        return
    
    
    # dict_config = {'release': True, 'static': False}
    dict_config = {}
    dict_config['release'] = ARG_RELEASE
    dict_config['debug'] = ARG_DEBUG
    dict_config['debuginfo'] = ARG_DEBUGINFO
    dict_config['dynamic'] = ARG_DYNAMIC
    dict_config['static'] = ARG_STATIC
    dict_config['arch'] = ARG_ARCH
    dict_config['install_dir'] = ARG_INSTALL_DIR
    
    if(dict_config['debug']):
        dict_config['release'] = False
        dict_config['debuginfo'] = False
    if(dict_config['debuginfo']):
        dict_config['release'] = False
        dict_config['debug'] = False
    if(dict_config['static']):
        dict_config['dynamic'] = False
        
    
    if(ARG_ARCH == "vs2005-32"):
        dict_config['cmake_cfg'] = ' -G "Visual Studio 8 2005" '
    if(ARG_ARCH == "vs2005-64"):
        dict_config['cmake_cfg'] = ' -G "Visual Studio 8 2005 Win64" '
        
    if(ARG_ARCH == "vs2008-32"):
        dict_config['cmake_cfg'] = ' -G "Visual Studio 9 2008" '
    if(ARG_ARCH == "vs2008-64"):
        dict_config['cmake_cfg'] = ' -G "Visual Studio 9 2008 Win64" '
    
    if(ARG_ARCH == "vs2010-32"):
        dict_config['cmake_cfg'] = ' -G "Visual Studio 10 2010" '
    if(ARG_ARCH == "vs2010-64"):
        dict_config['cmake_cfg'] = ' -G "Visual Studio 10 2010 Win64" '
    
    if(ARG_ARCH == "vs2012-32"):
        dict_config['cmake_cfg'] = ' -G "Visual Studio 11 2012" '
    if(ARG_ARCH == "vs2012-64"):
        dict_config['cmake_cfg'] = ' -G "Visual Studio 11 2012 Win64" '
    
    if(ARG_ARCH == "vs2013-32"):
        dict_config['cmake_cfg'] = ' -G "Visual Studio 12 2013" '
    if(ARG_ARCH == "vs2013-64"):
        dict_config['cmake_cfg'] = ' -G "Visual Studio 12 2013 Win64" '
    
    if(ARG_ARCH == "vs2015-32"):
        dict_config['cmake_cfg'] = ' -G "Visual Studio 14 2015" '
    if(ARG_ARCH == "vs2015-64"):
        dict_config['cmake_cfg'] = ' -G "Visual Studio 14 2015 Win64" '
        
    if(ARG_ARCH == "vs2017-32"):
        dict_config['cmake_cfg'] = ' -G "Visual Studio 15 2017" '
    if(ARG_ARCH == "vs2017-64"):
        dict_config['cmake_cfg'] = ' -G "Visual Studio 15 2017 Win64" '
        
    if(ARG_ARCH == "vs2019-32"):
        dict_config['cmake_cfg'] = ' -G "Visual Studio 16 2019" -A Win32'
    if(ARG_ARCH == "vs2019-64"):
        dict_config['cmake_cfg'] = ' -G "Visual Studio 16 2019" -A x64'

    if(ARG_ARCH == "mingw-32"):
        dict_config['cmake_cfg'] = ' -G "MSYS Makefiles" '
    if(ARG_ARCH == "mingw-64"):
        dict_config['cmake_cfg'] = ' -G "MSYS Makefiles" '

    if(ARG_ARCH == "nmake"):
        dict_config['cmake_cfg'] = ' -G "NMake Makefiles" '
        
    if(ARG_ARCH == "unix"):
        #dict_config['cmake_cfg'] = ' -G "Unix Makefiles" '
        #dict_config['cmake_cfg'] = ' -G "Kate - Unix Makefiles" '
        dict_config['cmake_cfg'] = ' -G "CodeBlocks - Unix Makefiles" '
        #dict_config['cmake_cfg'] = ' -G "CodeBlocks - Ninja" '

    if(ARG_ARCH == "ndk"):
        dict_config['cmake_cfg'] = ' -G "NMake Makefiles" '
        # dict_config['cmake_cfg'] = ' -G "MSYS Makefiles" '
        # dict_config['cmake_cfg'] = ' -G "Unix Makefiles" '
        
    if(ARG_ARCH == "ninja"):
        dict_config['cmake_cfg'] = ' -G Ninja'
        
    if(ARG_ARCH == "em"):
        dict_config['static'] = True
        dict_config['dynamic'] = False
        # dict_config['cmake_cfg'] = ' -G "NMake Makefiles" '
        dict_config['cmake_cfg'] = ' -G Ninja '
        
    print (dict_config)
    
    if(ARG_ONLIY):
    
        getLibrary( ARG_NAME , ARG_CMD == "source" , dict_config )
        
    else:
    
        # dependency
        list_name = getDependency( ARG_NAME )
        print ("Dependency -> " + str(list_name))
        
        # download and build install
        for x in list_name:
            print("==> " + x)
            getLibrary( x , ARG_CMD == "source" , dict_config )
    
    
if __name__ == "__main__":
    main()
