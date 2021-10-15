#!/usr/bin/python
# -*- coding: utf-8 -*-

from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/google/leveldb.git")
        # return
        
        
    if(dict_config['arch']=="em"):
        # em根本无法在本地建立leveldb数据库
        return
        
    STR_CFG = ''
    STR_CFG += " -DLEVELDB_BUILD_TEST=0"
    if(dict_config['static']):
        STR_CFG += " -DLEVELDB_BUILD_STATIC=1"
    else:
        STR_CFG += " -DLEVELDB_BUILD_STATIC=1" #只能编译静态库
    
    source_dir = os.getcwd() + '/../prebuild/leveldb-master'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    