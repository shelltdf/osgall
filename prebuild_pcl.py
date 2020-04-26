#!/usr/bin/python
# -*- coding: cp936 -*-

import os
import sys
import string
import subprocess
from common import *


#内部路径 升级后可能会替换
pcl_DIR_NAME = "pcl-pcl-1.8.1"


#    
#库建造定义
#

def build_pcl( str_target ):
    print "build VTK"
    my_make_build_dir( pcl_DIR_NAME , str_target )
    
    str_ops = ""
    
    str_ops += ' -DBOOST_ROOT="%(1)s/../../../3rdparty/"' % {"1": os.getcwd()}
    str_ops += ' -DBoost_DIR="%(1)s/../../../3rdparty/include/"' % {"1": os.getcwd()}
    str_ops += ' -DBoost_INCLUDE_DIR="%(1)s/../../../3rdparty/include/"' % {"1": os.getcwd()}
    str_ops += ' -DBOOST_LIBRARYDIR="%(1)s/../../../3rdparty/lib/"' % {"1": os.getcwd()}
    str_ops += ' -DBoost_FILESYSTEM_LIBRARY_DEBUG="%(1)s/../../../3rdparty/lib/boost_filesystem-mt-gd.lib"' % {"1": os.getcwd()}
    str_ops += ' -DBoost_FILESYSTEM_LIBRARY_RELEASE="%(1)s/../../../3rdparty/lib/boost_filesystem-mt.lib" ' % {"1": os.getcwd()}
    str_ops += ' -DBoost_THREAD_LIBRARY_DEBUG="%(1)s/../../../3rdparty/lib/boost_thread-mt-gd.lib"' % {"1": os.getcwd()}
    str_ops += ' -DBoost_THREAD_LIBRARY_RELEASE="%(1)s/../../../3rdparty/lib/boost_thread-mt.lib" ' % {"1": os.getcwd()}
    str_ops += ' -DBoost_DATE_TIME_LIBRARY_DEBUG="%(1)s/../../../3rdparty/lib/boost_date_time-mt-gd.lib"' % {"1": os.getcwd()}
    str_ops += ' -DBoost_DATE_TIME_LIBRARY_RELEASE="%(1)s/../../../3rdparty/lib/boost_date_time-mt.lib" ' % {"1": os.getcwd()}
    str_ops += ' -DBoost_IOSTREAMS_LIBRARY_DEBUG="%(1)s/../../../3rdparty/lib/boost_iostreams-mt-gd.lib"' % {"1": os.getcwd()}
    str_ops += ' -DBoost_IOSTREAMS_LIBRARY_RELEASE="%(1)s/../../../3rdparty/lib/boost_iostreams-mt.lib" ' % {"1": os.getcwd()}

    
    if( BUILD_DYNAMIC_LINK_LIBRARY ):
        str_ops += " -DPCL_SHARED_LIBS=1";
        str_ops += " -DBoost_USE_STATIC_LIBS=0";
    else:
        str_ops += " -DPCL_SHARED_LIBS=0";
        str_ops += " -DBoost_USE_STATIC_LIBS=1";
        
    my_configure(str_target , str_ops)
    my_build(str_target)


    
#main    
def main():

    ALL_TARGET = "vc" #默认vc
    if len(sys.argv) >= 2:
        ALL_TARGET = sys.argv[1]
    print( ALL_TARGET )

        
    #
    build_pcl(ALL_TARGET)

main()
