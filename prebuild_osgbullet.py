#!/usr/bin/python
# -*- coding: cp936 -*-

import os
import sys
import string
import subprocess
from common import *


#内部路径 升级后可能会替换
bullet_DIR_NAME = "bullet3-2.83.7"
osgworks_DIR_NAME = "osgworks-master"
osgbullet_DIR_NAME = "osgbullet-master"


#    
#库建造定义
#

def build_bullet( str_target ):
    print ("build bullet")
    my_make_build_dir( bullet_DIR_NAME , str_target )
    
    #删除源码中 gwen 的定义 -D_STATIC_CPPLIB
    
    str_ops = ""
    str_ops += " -DINSTALL_EXTRA_LIBS=ON -DINSTALL_LIBS=ON";
    str_ops += " -DBUILD_SHARED_LIBS=0";   #无法正常编译dll 只能静态
        
    if( str_target == "ndk"):
        str_ops += " -DBUILD_BULLET2_DEMOS=OFF";
        str_ops += " -DBUILD_OPENGL3_DEMOS=OFF";
        str_ops += " -DBUILD_CPU_DEMOS=OFF";
        str_ops += " -DBUILD_UNIT_TESTS=OFF";
        pass
    else:
        str_ops += " -DUSE_MSVC_RUNTIME_LIBRARY_DLL=1";
        # str_ops += " -DLIBRARY_OUTPUT_PATH=../../../3rdparty/lib ";
        
    my_configure(str_target , str_ops ,False , False)
    my_build(str_target)

def build_osgworks( str_target ):
    print ("build osgworks")
    my_make_build_dir( osgworks_DIR_NAME , str_target )
    
    str_ops = ""
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops += ' -DBUILD_SHARED_LIBS=1'
    else:
        str_ops += ' -DBUILD_SHARED_LIBS=0'
        
    if(str_target == "vc" and (not BUILD_DYNAMIC_LINK_LIBRARY)):
        str_ops += ' -DOPENTHREADS_LIBRARY=../../../3rdparty/lib/ot20-OpenThreads.lib'
        str_ops += ' -DOSG_LIBRARY=../../../3rdparty/lib/osg130-osg.lib'
        str_ops += ' -DOSGGA_LIBRARY=../../../3rdparty/lib/osg130-osgGA.lib'
        str_ops += ' -DOSGTEXT_LIBRARY=../../../3rdparty/lib/osg130-osgText.lib'
        str_ops += ' -DOSGDB_LIBRARY=../../../3rdparty/lib/osg130-osgDB.lib'
        str_ops += ' -DOSGVIEWER_LIBRARY=../../../3rdparty/lib/osg130-osgViewer.lib'
        str_ops += ' -DOSGSIM_LIBRARY=../../../3rdparty/lib/osg130-osgSim.lib'
        str_ops += ' -DOSGUTIL_LIBRARY=../../../3rdparty/lib/osg130-osgUtil.lib'
        
        str_ops += ' -DOSGWORKS_BUILD_APPS=OFF'
        str_ops += ' -DOSGWORKS_BUILD_TESTS=OFF'
        str_ops += ' -DOSGWORKS_BUILD_EXAMPLES=OFF'
        
    
    if(str_target == "ndk"):
        str_ops += ' -DOSGInstallType="Alternate Install Location"';
        str_ops += " -DOSGInstallLocation="+os.getcwd()+"../../../3rdparty";
        # str_ops += " -DOSG_DIR="+os.getcwd()+"/../../../armeabi-v7a/";
        str_ops += ' -DOPENTHREADS_INCLUDE_DIR='+os.getcwd()+'../../../armeabi-v7a/include -DOPENTHREADS_LIBRARY=../../../armeabi-v7a/lib/OpenThreads.a'
        str_ops += ' -DOSG_INCLUDE_DIR='+os.getcwd()+'../../../armeabi-v7a/include -DOSG_LIBRARY=../../../armeabi-v7a/lib/osg.a'
        str_ops += ' -DOSGGA_INCLUDE_DIR='+os.getcwd()+'../../../armeabi-v7a/include -DOSGGA_LIBRARY=../../../armeabi-v7a/lib/osgGA.a'
        str_ops += ' -DOSGDB_INCLUDE_DIR='+os.getcwd()+'../../../armeabi-v7a/include -DOSGDB_LIBRARY=../../../armeabi-v7a/lib/osgDB.a'
        str_ops += ' -DOSGTEXT_INCLUDE_DIR='+os.getcwd()+'../../../armeabi-v7a/include -DOSGTEXT_LIBRARY=../../../armeabi-v7a/lib/osgText.a'
        str_ops += ' -DOSGVIEWER_INCLUDE_DIR='+os.getcwd()+'../../../armeabi-v7a/include -DOSGVIEWER_LIBRARY=../../../armeabi-v7a/lib/osgViewer.a'
        str_ops += ' -DOSGSIM_INCLUDE_DIR='+os.getcwd()+'../../../armeabi-v7a/include -DOSGSIM_LIBRARY=../../../armeabi-v7a/lib/osgSim.a'
        str_ops += ' -DOSGUTIL_INCLUDE_DIR='+os.getcwd()+'../../../armeabi-v7a/include -DOSGUTIL_LIBRARY=../../../armeabi-v7a/lib/osgUtil.a'
        
        str_ops += ' -DOSGWORKS_BUILD_APPS=OFF'
        
    
    my_configure(str_target , str_ops ,False)
    my_build(str_target)
    
def build_osgbullet( str_target ):
    print ("build osgbullet")
    my_make_build_dir( osgbullet_DIR_NAME , str_target )
    
    str_ops = ""
    
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops += ' -DBUILD_SHARED_LIBS=1'
    else:
        str_ops += ' -DBUILD_SHARED_LIBS=0'
        
    if(str_target == "vc" and (not BUILD_DYNAMIC_LINK_LIBRARY)):
        str_ops += ' -DOPENTHREADS_LIBRARY=../../../3rdparty/lib/ot20-OpenThreads.lib'
        str_ops += ' -DOSG_LIBRARY=../../../3rdparty/lib/osg130-osg.lib'
        str_ops += ' -DOSGGA_LIBRARY=../../../3rdparty/lib/osg130-osgGA.lib'
        str_ops += ' -DOSGTEXT_LIBRARY=../../../3rdparty/lib/osg130-osgText.lib'
        str_ops += ' -DOSGDB_LIBRARY=../../../3rdparty/lib/osg130-osgDB.lib'
        str_ops += ' -DOSGVIEWER_LIBRARY=../../../3rdparty/lib/osg130-osgViewer.lib'
        str_ops += ' -DOSGSIM_LIBRARY=../../../3rdparty/lib/osg130-osgSim.lib'
        str_ops += ' -DOSGUTIL_LIBRARY=../../../3rdparty/lib/osg130-osgUtil.lib'
        str_ops += ' -DOSGSHADOW_LIBRARY=../../../3rdparty/lib/osg130-osgShadow.lib'
        
        str_ops += ' -DOSGBULLET_BUILD_APPLICATIONS=OFF'
        str_ops += ' -DOSGBULLET_BUILD_TESTS=OFF'
        str_ops += ' -DOSGBULLET_BUILD_EXAMPLES=OFF'
    
    
    str_ops += " -DosgWorks_DIR=../../3rdparty/lib";
    # str_ops += " -DBUILD_SHARED_LIBS=1";
    # str_ops += " -Dbullet_DIR=../../3rdparty/lib";
    # str_ops += " -DBULLET_INCLUDE_DIR=../../3rdparty/include";
    # str_ops += " -DBUILD_SHARED_LIBS=1 ";
    str_ops += ' -DBulletInstallType="Alternate Install Location"';
    str_ops += " -DBulletInstallLocation=../../../3rdparty";
    
    # if( str_target == "ndk"):
        # pass
    # else:
        # str_ops = " -DUSE_MSVC_RUNTIME_LIBRARY_DLL=1 ";
        
    my_configure(str_target , str_ops ,False)
    my_build(str_target)
    
    
#main    
def main():

    ALL_TARGET = "vc" #默认vc
    if len(sys.argv) >= 2:
        ALL_TARGET = sys.argv[1]
    print( ALL_TARGET )

        
    #
    build_bullet(ALL_TARGET)
    build_osgworks(ALL_TARGET)
    build_osgbullet(ALL_TARGET)

main()
