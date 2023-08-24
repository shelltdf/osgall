
from ._common import *
from ._config import *

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("libtiff" , list_name,getDependency)
    list_name = addDependency("boost" , list_name,getDependency)
    list_name = addDependency("openexr" , list_name,getDependency)
    list_name = addDependency("libjpeg" , list_name,getDependency)
    list_name = addDependency("bzip2" , list_name,getDependency)
    list_name = addDependency("tbb" , list_name,getDependency)
    list_name = addDependency("openvdb" , list_name,getDependency)
    list_name = addDependency("ptex" , list_name,getDependency)
    list_name = addDependency("fmt" , list_name,getDependency)
    list_name = addDependency("libpng" , list_name,getDependency)
    list_name = addDependency("freetype" , list_name,getDependency)
    list_name = addDependency("opencolorio" , list_name,getDependency)
    list_name = addDependency("giflib" , list_name,getDependency)
    list_name = addDependency("hdf" , list_name,getDependency)
    list_name = addDependency("nvtt" , list_name,getDependency)
    list_name = addDependency("opencv" , list_name,getDependency)
    list_name = addDependency("libwebp" , list_name,getDependency)
    list_name = addDependency("openjpeg" , list_name,getDependency)
    list_name = addDependency("libjpeg-turbo" , list_name,getDependency)
    list_name = addDependency("libraw" , list_name,getDependency)
    list_name = addDependency("libheif" , list_name,getDependency)
    list_name = addDependency("dcmtk" , list_name,getDependency)
    # list_name = addDependency("field3d" , list_name,getDependency)
    list_name = addDependency("robin-map" , list_name,getDependency)
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_only_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_only_download):
        # download_source(str_name,"https://github.com/madler/zlib.git")
        # return
    
    install_dir = dict_config['install_dir'] + '/' + my_build_and_install_dir(dict_config)
    install_dir = install_dir.replace('\\','/')	
    
    STR_CFG = ''
    # STR_CFG += ' -DUSE_Package=0'
    STR_CFG += ' -DUSE_Nuke=0'
    STR_CFG += ' -DUSE_FFmpeg=0'
    STR_CFG += ' -DUSE_OpenEXR=1'
    STR_CFG += ' -DUSE_Field3D=0' # no pybind11
    STR_CFG += ' -DUSE_PYTHON=0'
    STR_CFG += ' -DBUILD_TESTING=0'
    STR_CFG += ' -DOIIO_BUILD_TOOLS=0'
    STR_CFG += ' -DOIIO_BUILD_TESTS=0'
    # STR_CFG += ' -DBoost_NO_BOOST_CMAKE=0'
    STR_CFG += ' -DBOOST_CUSTOM=1'
    if(dict_config['static']):
        STR_CFG += " -DBUILD_SHARED_LIBS=0"
        STR_CFG += " -DBoost_USE_STATIC_LIBS=1"
    else:
        STR_CFG += " -DBUILD_SHARED_LIBS=1"
        STR_CFG += " -DBoost_USE_STATIC_LIBS=0"
        
            
    # OPENEXR3 ILMBASE_INCLUDE_PATH
    # STR_CFG += ' -DILMBASE_INCLUDE_PATH=' + install_dir + '/include/Imath/'
    # STR_CFG += ' -DILMBASE_INCLUDES=' + install_dir + '/include/Imath/'
    # STR_CFG += ' -DOPENEXR_ILMIMF_LIBRARY=" "'
    # STR_CFG += ' -DOPENEXR_HALF_LIBRARY=" "'
    
    # OPENEXR2
    if(dict_config['debug']==True):
        STR_CFG += ' -DOPENEXR_IMATH_LIBRARY=' + install_dir + '/lib/Imathd.lib'
        STR_CFG += ' -DOPENEXR_ILMIMF_LIBRARY=' + install_dir + '/lib/IlmImfd.lib'
        STR_CFG += ' -DOPENEXR_IEX_LIBRARY=' + install_dir + '/lib/Iexd.lib'
        STR_CFG += ' -DOPENEXR_HALF_LIBRARY=' + install_dir + '/lib/Halfd.lib'
        STR_CFG += ' -DOPENEXR_ILMTHREAD_LIBRARY=' + install_dir + '/lib/IlmThreadd.lib'
    
    # Boost_VERSION, Boost_INCLUDE_DIRS, Boost_LIBRARY_DIRS, Boost_LIBRARIES.
    STR_CFG += ' -DBoost_INCLUDE_DIRS=' + install_dir + '/include/boost-1_82/'
    STR_CFG += ' -DBoost_LIBRARY_DIRS=' + install_dir + '/lib/'
    
    # STR_CFG += ' -DOpenCV_ROOT=' + install_dir + '/'
    # STR_CFG += ' -DDCMTK_ROOT=' + install_dir + '/'
    STR_CFG += ' -DOpenJpeg_ROOT=' + install_dir + '/'
    # STR_CFG += ' -DWebp_ROOT=' + install_dir + '/'
    
    if(ENABLE_QT):
        STR_CFG += ' -DQt5_ROOT=' + Qt_LIB
    
    STR_CFG += ' -DOpenCV_INCLUDE_DIR=' + install_dir + '/include/'
    if(dict_config['debug']==True):
        STR_CFG += ' -Dopencv_core_lib=' + install_dir + '/lib/opencv_core480d.lib'
        STR_CFG += ' -Dopencv_imgproc_lib=' + install_dir + '/lib/opencv_imgproc480d.lib'
        STR_CFG += ' -Dopencv_videoio_lib=' + install_dir + '/lib/opencv_videoio480d.lib'
    else:
        STR_CFG += ' -Dopencv_core_lib=' + install_dir + '/lib/opencv_core480.lib'
        STR_CFG += ' -Dopencv_imgproc_lib=' + install_dir + '/lib/opencv_imgproc480.lib'
        STR_CFG += ' -Dopencv_videoio_lib=' + install_dir + '/lib/opencv_videoio480.lib'
    
    STR_CFG += ' -DDCMTK_INCLUDE_PATH=' + install_dir + '/include/'
    
    STR_CFG += ' -DWEBP_INCLUDE_DIR=' + install_dir + '/include/'
    STR_CFG += ' -DWEBP_LIBRARY=' + install_dir + '/lib/libwebp.lib'
    
    STR_CFG += ' -DOPENJPEG_INCLUDE_DIR=' + install_dir + '/include/openjpeg-2.5/'
    
        
    # source_dir = os.getcwd() + '/../prebuild/oiio-master'
    # source_dir = os.getcwd() + '/../prebuild/oiio-Release-2.1.16.0'
    # source_dir = os.getcwd() + '/../prebuild/oiio-Release-2.1.20.0'
    source_dir = os.getcwd() + '/../prebuild/oiio-2.3.17.0'
    
    configure(str_name,dict_config,STR_CFG,"",source_dir)
    build(str_name,dict_config)
    install(str_name,dict_config)
    
    