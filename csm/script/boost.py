
from ._common import *
# import shutil

def getDependency( str_name ,getDependency):
    list_name = []
    
    list_name = addDependency("zlib" , list_name,getDependency)
    # list_name = addDependency("libiconv" , list_name,getDependency)
    # list_name = addDependency("libbzip2" , list_name,getDependency)
    
    return list_name + [str_name]
    
    
def SBI( str_name , b_download ,dict_config, getLibrary ):
    # print(str_name)
    
    # if(b_download):
        # download_source(str_name,"https://github.com/boost-cmake/boost.git")
        # download_source(str_name,"https://github.com/microcai/boost.git")
        # download_source(str_name,"https://github.com/Orphis/boost-cmake.git")
        # download_source(str_name,"https://github.com/boostorg/boost.git",'master','boost-1.60.0')
        # download_source(str_name,"https://github.com/boostorg/boost.git",'master','boost-1.60.0')
        # download_source(str_name,"https://github.com/boostorg/boost.git",'master','boost-1.67.0')
        
        # return
        
    BOOST_ARCH = 'x86' # -march setting
    BOOST_ADDRESS_MODEL = '64'  # 32 64

    install_dir = os.getcwd() + '/install/' + my_build_and_install_dir(dict_config)
    install_dir = install_dir.replace('\\','/')	
    
    source_dir = os.getcwd() + '/../prebuild/boost_1_71_0'
    os.chdir( source_dir )
    
    if(dict_config['arch'][:2]=="vs"):
        my_exec( "bootstrap.bat" )

        str = 'b2 --toolset=msvc install debug release threading=multi runtime-link=shared'
        str += ' architecture=' + BOOST_ARCH + ' address-model=' + BOOST_ADDRESS_MODEL
        # str += ' --stagedir=../../3rdparty --includedir=../../3rdparty/include'
        # str += ' --prefix=../../3rdparty/'
        str += ' --prefix=' + install_dir
        str += ' --layout=tagged'
        # str += ' --layout=tagged'
        # str += ' --without-system'
        if( dict_config['dynamic'] ):
            str += ' link=shared'
        else:
            str += ' link=static'
        
        if(True):
            str += ' -s ZLIB_INCLUDE="' +install_dir+ '/include"'
            str += ' -s ZLIB_LIBPATH="' +install_dir+ '/lib"'
            # str += ' -s ZLIB_BINARY=zlib' 
            str += ' -s ZLIB_SOURCE="../zlib"'
            # str += ' -s ZLIB_SOURCE="' +install_dir+ '/include"'
            
        my_exec( str )
        

        #move dll to bin
        # os.chdir( "../../3rdparty/" )
        # os.system( "mkdir bin" )
        # os.chdir( "../prebuild/" + boost_DIR_NAME )
        # os.system( 'move /y ../../3rdparty/lib/*.dll ../../3rdparty/bin/' )
        
        str_move = 'move /y '+install_dir+'\\lib\\*.dll '+install_dir+'\\bin\\'
        print(str_move)
        os.system( str_move )

    if(dict_config['arch']=="unix"):
        my_exec( "chmod 777 bootstrap.sh" )
        my_exec( "chmod 777 ./tools/build/src/engine/build.sh" )
        my_exec( "./bootstrap.sh" )

        str = './b2 --toolset=gcc install debug release threading=multi runtime-link=shared'
        str += ' architecture=' + BOOST_ARCH + ' address-model=' + BOOST_ADDRESS_MODEL
        # str += ' --stagedir=../../3rdparty --includedir=../../3rdparty/include'
        # str += ' --prefix=../../3rdparty/'
        str += ' --prefix=' + install_dir
        str += ' --layout=tagged'
        # str += ' --layout=tagged'
        # str += ' --without-system'
        if( dict_config['dynamic'] ):
            str += ' link=shared'
        else:
            str += ' link=static'
        
        if(True):
            str += ' -s ZLIB_INCLUDE="' +install_dir+ '/include"'
            str += ' -s ZLIB_LIBPATH="' +install_dir+ '/lib"'
            # str += ' -s ZLIB_BINARY=zlib' 
            str += ' -s ZLIB_SOURCE="../zlib"'
            # str += ' -s ZLIB_SOURCE="' +install_dir+ '/include"'
            
        my_exec( str )


    # os.chdir( ".." )
    # os.chdir( ".." )
    os.chdir( CWD )
    
    
    # STR_CGG = ''
    # if(dict_config['static']):
        # STR_CGG += ''
    # else:
        # STR_CGG += ''
    
    # configure(str_name,dict_config)
    # build(str_name,dict_config)
    # install(str_name,dict_config)
    
    # rd_dir = ""
    # if(dict_config['debug']==True):
        # rd_dir += 'Debug'
    # if(dict_config['release']==True):
        # rd_dir += 'Release'
        
    # dist_dir = my_build_and_install_dir(dict_config)
    # dist_dir = os.getcwd() + '/install/' + dist_dir
    # source_dir = my_build_and_install_dir(dict_config)
    # source_dir = os.getcwd() + "/build/" + source_dir + "/" + str_name + "/" + rd_dir
    # print source_dir
    # print dist_dir
    # copyfiles(source_dir,dist_dir+"/lib","*.lib")
    
    # source_dir = os.getcwd() + "/source/boost/boost/boost_1_64_0/"
    # print source_dir
    # print dist_dir
    # copyfiles(source_dir,dist_dir+"/include/","*.hpp")
    
    
    