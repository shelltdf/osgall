#!/usr/bin/python

import os
import sys
import string
import subprocess
import shutil 
from common import *


#main    
def main():

    ALL_TARGET = "vc" #default
    if len(sys.argv) >= 1:
        ALL_TARGET = sys.argv[1]
    # print( ALL_TARGET  )
        
    # prebuild
    for dir in os.listdir(os.getcwd()+"/prebuild"):
        # print dir
        if os.path.isdir(os.getcwd()+"/prebuild/"+dir):
            for dir2 in os.listdir(os.getcwd()+"/prebuild/"+dir):
                if os.path.isdir(os.getcwd()+"/prebuild/"+dir+"/"+dir2):
                    if(dir2 == "build_"+ALL_TARGET):
                        print (os.getcwd()+"/prebuild/"+dir+"/"+dir2)
                        # os.rmdir(os.getcwd()+"/prebuild/"+dir+"/"+dir2)
                        shutil.rmtree(os.getcwd()+"/prebuild/"+dir+"/"+dir2)
    
    # projects
    for dir in os.listdir(os.getcwd()+"/projects"):
        # print dir
        if os.path.isdir(os.getcwd()+"/projects/"+dir):
            for dir2 in os.listdir(os.getcwd()+"/projects/"+dir):
                if os.path.isdir(os.getcwd()+"/projects/"+dir+"/"+dir2):
                    if(dir2 == "build_"+ALL_TARGET):
                        print (os.getcwd()+"/projects/"+dir+"/"+dir2)
                        # os.rmdir(os.getcwd()+"/projects/"+dir+"/"+dir2)
                        shutil.rmtree(os.getcwd()+"/projects/"+dir+"/"+dir2)
                        
main()



