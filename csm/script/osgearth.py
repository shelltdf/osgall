
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("osg" , list_name,getDependency)
    list_name = addDependency("gdal" , list_name,getDependency)
    list_name = addDependency("geos" , list_name,getDependency)
    list_name = addDependency("leveldb" , list_name,getDependency)

    return list_name + [str_name]
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):

    # if(b_only_download):
        # download_source(str_name,"https://github.com/gwaldron/osgearth.git")
        # download_source(str_name,"https://github.com/shelltdf/osgearth.git","2.8")
        # return
    
    dir_name = my_build_and_install_dir(dict_config)
    install_dir = os.getcwd().replace('\\','/') +"/install/"+ my_build_and_install_dir(dict_config)
    
    
    STR_CFG = ""
    STR_CFG += ' -DOSG_DIR="' + install_dir + '/"'
    STR_CFG += " -DOSGEARTH_USE_QT=0"
    STR_CFG += " -DOSGEARTH_QT_BUILD=0"
    # STR_CFG += " -DINSTALL_TO_OSG_DIR=1"
    STR_CFG += " -DAPPEND_OPENSCENEGRAPH_VERSION=1"
    
    if(dict_config['arch'][:2]=="vs"):
        STR_CFG += " -DGDAL_INCLUDE_DIR='" + install_dir + "/include/gdal'"
        STR_CFG += " -DSQLITE3_INCLUDE_DIR='" + install_dir + "/include'"
        if(dict_config['release']):
            STR_CFG += " -DGDAL_LIBRARY='" + install_dir + "/lib/gdal31.lib'"
            STR_CFG += " -DCURL_LIBRARY='" + install_dir + "/lib/libcurl_imp.lib'"
            
            STR_CFG += " -DSQLITE3_LIBRARY='" + install_dir + "/lib/sqlite.lib'"
            
        else:
            STR_CFG += " -DGDAL_LIBRARY='" + install_dir + "/lib/gdal31d.lib'"
            STR_CFG += " -DCURL_LIBRARY='" + install_dir + "/lib/libcurld_imp.lib'"
            STR_CFG += " -DOPENTHREADS_LIBRARY='" + install_dir + "/lib/OpenThreadsd.lib'"
            STR_CFG += " -DOSG_LIBRARY='" + install_dir + "/lib/osgd.lib'"
            STR_CFG += " -DOSGDB_LIBRARY='" + install_dir + "/lib/osgDBd.lib'"
            STR_CFG += " -DOSGFX_LIBRARY='" + install_dir + "/lib/osgFXd.lib'"
            STR_CFG += " -DOSGGA_LIBRARY='" + install_dir + "/lib/osgGAd.lib'"
            STR_CFG += " -DOSGMANIPULATOR_LIBRARY='" + install_dir + "/lib/osgManipulatord.lib'"
            STR_CFG += " -DOSGSHADOW_LIBRARY='" + install_dir + "/lib/osgShadowd.lib'"
            STR_CFG += " -DOSGSIM_LIBRARY='" + install_dir + "/lib/osgSimd.lib'"
            STR_CFG += " -DOSGTERRAIN_LIBRARY='" + install_dir + "/lib/osgTerraind.lib'"
            STR_CFG += " -DOSGTEXT_LIBRARY='" + install_dir + "/lib/osgTextd.lib'"
            STR_CFG += " -DOSGUTIL_LIBRARY='" + install_dir + "/lib/osgUtild.lib'"
            STR_CFG += " -DOSGVIEWER_LIBRARY='" + install_dir + "/lib/osgViewerd.lib'"
            STR_CFG += " -DOSGWIDGET_LIBRARY='" + install_dir + "/lib/osgWidgetd.lib'"
            
            STR_CFG += " -DSQLITE3_LIBRARY='" + install_dir + "/lib/sqlited.lib'"
    
    if(dict_config['arch']=="em"):
    
        STR_CFG += " -DDYNAMIC_OSGEARTH=0"
        STR_CFG += " -DBUILD_OSGEARTH_EXAMPLES=0"
        STR_CFG += " -DBUILD_APPLICATIONS=0"
        STR_CFG += " -DBUILD_TESTS=0"
        
        STR_CFG += ' -DGDAL_INCLUDE_DIR="' + install_dir + '/include/gdal"'
        STR_CFG += ' -DCURL_INCLUDE_DIR="' + install_dir + '/include/"'
        
        STR_CFG += " -DGDAL_LIBRARY='" + install_dir + "/lib/libgdal31.a'"
        STR_CFG += " -DCURL_LIBRARY='" + install_dir + "/lib/libcurl.a'"
        
        # STR_CFG += ' -DOSG_DIR="' + install_dir + '"/'
        STR_CFG += ' -DOSG_INCLUDE_DIR="' + install_dir + '/include/"'
        STR_CFG += ' -DOSG_GEN_INCLUDE_DIR="' + install_dir + '/include/"'
    
        STR_CFG += " -DOPENTHREADS_LIBRARY='" + install_dir + "/lib/libOpenThreads.a'"
        STR_CFG += " -DOSG_LIBRARY='" + install_dir + "/lib/libosg.a'"
        STR_CFG += " -DOSGDB_LIBRARY='" + install_dir + "/lib/libosgDB.a'"
        STR_CFG += " -DOSGFX_LIBRARY='" + install_dir + "/lib/libosgFX.a'"
        STR_CFG += " -DOSGGA_LIBRARY='" + install_dir + "/lib/libosgGA.a'"
        STR_CFG += " -DOSGMANIPULATOR_LIBRARY='" + install_dir + "/lib/libosgManipulator.a'"
        STR_CFG += " -DOSGSHADOW_LIBRARY='" + install_dir + "/lib/libosgShadow.a'"
        STR_CFG += " -DOSGSIM_LIBRARY='" + install_dir + "/lib/libosgSim.a'"
        STR_CFG += " -DOSGTERRAIN_LIBRARY='" + install_dir + "/lib/libosgTerrain.a'"
        STR_CFG += " -DOSGTEXT_LIBRARY='" + install_dir + "/lib/libosgText.a'"
        STR_CFG += " -DOSGUTIL_LIBRARY='" + install_dir + "/lib/libosgUtil.a'"
        STR_CFG += " -DOSGVIEWER_LIBRARY='" + install_dir + "/lib/libosgViewer.a'"
        STR_CFG += " -DOSGWIDGET_LIBRARY='" + install_dir + "/lib/libosgWidget.a'"
        
        STR_CFG += " -DMATH_LIBRARY=m"
            
    
    source_dir = os.getcwd() + '/../prebuild/osgearth-2.10.1'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    