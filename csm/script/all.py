
from ._common import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("eigen" , list_name,getDependency)
    
    # list_name = addDependency("anttweakbar" , list_name,getDependency)
    # list_name = addDependency("avunity" , list_name,getDependency)
    list_name = addDependency("boost" , list_name,getDependency)
    list_name = addDependency("crashrpt" , list_name,getDependency)
    list_name = addDependency("crunch" , list_name,getDependency)
    # list_name = addDependency("cgal" , list_name,getDependency)
    list_name = addDependency("curl" , list_name,getDependency)
    list_name = addDependency("draco" , list_name,getDependency)
    
    list_name = addDependency("expat" , list_name,getDependency)
    # list_name = addDependency("flann" , list_name,getDependency)
    list_name = addDependency("freeglut" , list_name,getDependency)
    list_name = addDependency("freetype" , list_name,getDependency)
    list_name = addDependency("gdal" , list_name,getDependency)
    list_name = addDependency("geos" , list_name,getDependency)
    list_name = addDependency("giflib" , list_name,getDependency)
    list_name = addDependency("glew" , list_name,getDependency)
    list_name = addDependency("googletest" , list_name,getDependency)
    list_name = addDependency("hdf" , list_name,getDependency)
    list_name = addDependency("ilmbase" , list_name,getDependency)
    list_name = addDependency("imgui" , list_name,getDependency)
    # list_name = addDependency("ifcplusplus" , list_name,getDependency)
    list_name = addDependency("jasper" , list_name,getDependency)
    list_name = addDependency("jsoncpp" , list_name,getDependency)
    list_name = addDependency("leveldb" , list_name,getDependency)
    # list_name = addDependency("libbzip2" , list_name,getDependency)
    list_name = addDependency("libgeotiff" , list_name,getDependency)
    list_name = addDependency("libiconv" , list_name,getDependency)
    list_name = addDependency("libjpeg" , list_name,getDependency)
    list_name = addDependency("liblas" , list_name,getDependency)
    list_name = addDependency("libpng" , list_name,getDependency)
    list_name = addDependency("libtiff" , list_name,getDependency)
    list_name = addDependency("libxml2" , list_name,getDependency)
    list_name = addDependency("lua" , list_name,getDependency)
    # list_name = addDependency("nvmt" , list_name,getDependency)
    list_name = addDependency("nvtt" , list_name,getDependency)
    # list_name = addDependency("ogre3d" , list_name,getDependency)
    list_name = addDependency("openexr" , list_name,getDependency)
    # list_name = addDependency("openmvg" , list_name,getDependency)
    # list_name = addDependency("openmvs" , list_name,getDependency)
    # list_name = addDependency("ork" , list_name,getDependency)
    list_name = addDependency("osg" , list_name,getDependency)
    # list_name = addDependency("osgdraco" , list_name,getDependency)
    list_name = addDependency("osgearth" , list_name,getDependency)
    # list_name = addDependency("osgeffect" , list_name,getDependency)
    # list_name = addDependency("osgentity" , list_name,getDependency)
    # list_name = addDependency("osggltf" , list_name,getDependency)
    # list_name = addDependency("osgsplit" , list_name,getDependency)
    # list_name = addDependency("paraview" , list_name,getDependency)
    list_name = addDependency("proj4" , list_name,getDependency)
    # list_name = addDependency("proland" , list_name,getDependency)
    list_name = addDependency("pthreads" , list_name,getDependency)
    list_name = addDependency("qhull" , list_name,getDependency)
    list_name = addDependency("rapidxml" , list_name,getDependency)
    # list_name = addDependency("stamen" , list_name,getDependency)
    list_name = addDependency("sqlite3" , list_name,getDependency)
    list_name = addDependency("stb" , list_name,getDependency)
    # list_name = addDependency("tinyxml" , list_name,getDependency)
    # list_name = addDependency("vcg" , list_name,getDependency)
    list_name = addDependency("vpb" , list_name,getDependency)
    list_name = addDependency("zlib" , list_name,getDependency)
    
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    print(str_name)
    # pass
    
    
    