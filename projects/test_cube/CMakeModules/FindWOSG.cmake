
#定义变量

#	WOSG_USE_STATIC_LIBRARY	使用静态osg库

#	WOSG_INCLUDE_DIR		inlcude目录
#	WOSG_INCLUDE_BUILD_DIR	inlcude目录
#	WOSG_LIBRARY_DIR		lib目录（没用）

#	WOSG_LIBRARY_ALL			全部可能存在的lib文件表
#	WOSG_LIBRARY_ALL_PLUGINS	全部可能存在的lib文件表
#   WOSG_LIBRARY_ALL_DEPEND

#全局变量

SET(WOSG_USE_STATIC_LIBRARY  ON  CACHE BOOL "")
set(WOSG_PLUGINS_DIR_NAME    "osgPlugins-3.4.0" )

IF(MSVC)
    set(WOSG_STATIC_LIBRARY_PREFIX      "osg100-" )
    set(WOSG_STATIC_OT_LIBRARY_PREFIX   "ot20-"  )
    set(WOSG_STATIC_LIBRARY_POSTFIX     ""  )
    set(WOSG_STATIC_PLUGINS_POSTFIX     ".lib"  )
ENDIF(MSVC)

IF(CYGWIN)
    SET(WOSG_STATIC_LIBRARY_PREFIX      "lib"  )
    SET(WOSG_STATIC_OT_LIBRARY_PREFIX   "lib"  )
    SET(WOSG_STATIC_LIBRARY_POSTFIX     ".a" )
    set(WOSG_STATIC_PLUGINS_POSTFIX     ""  )
ENDIF(CYGWIN)

IF(ANDROID)
    SET(WOSG_STATIC_LIBRARY_PREFIX      "lib"  )
    SET(WOSG_STATIC_OT_LIBRARY_PREFIX   "lib"  )
    SET(WOSG_STATIC_LIBRARY_POSTFIX     ".a" )
    set(WOSG_STATIC_PLUGINS_POSTFIX     ""  )
ENDIF(ANDROID)


SET(WOSG_LIBRARY_ALL  )
SET(WOSG_LIBRARY_ALL_PLUGINS )
SET(WOSG_LIBRARY_ALL_DEPEND )


SET(MODULE_NAME_LIST
	osg osgDB osgUtil osgViewer osgGA 
    osgText 
    osgAnimation
	# osgFX 
    osgManipulator 
    osgParticle 
	# osgShadow osgSim osgTerrain 
	# osgVolume osgWidget
	#osgPresentation 
	#osgQt 
)

SET(PLUGINS_NAME_LIST
	# 3dc 3ds ac bmp bsp bvh cfg dds dot dw dxf 
	# glsl hdr ive logo lwo lws md2 mdl normals obj
	# openflight osga p3d pic ply pnm pov revisions rgb rot
	# scale shp stl tga tgz trans txf txp vtf x 
	
	# curl freetype  png  jpeg tiff 
    # fbx
    # zip
    # nvtt
	# dae gif gz qfont geo
    
	# osg
	# deprecated_osg
	# deprecated_osganimation
	# deprecated_osgfx
	# deprecated_osgparticle
	# deprecated_osgshadow
	# deprecated_osgsim
    # deprecated_osgterrain
    # deprecated_osgtext
    # deprecated_osgviewer
    # deprecated_osgvolume
    # deprecated_osgwidget
	# serializers_osg
	# serializers_osganimation
	# serializers_osgfx
    # serializers_osgmanipulator
    # serializers_osgparticle
    # serializers_osgshadow
    # serializers_osgsim
    # serializers_osgterrain
    # serializers_osgtext
    # serializers_osgvolume
)

SET(DEPEND_NAME_LIST

)

	

#搜索插件
FUNCTION(WOSG_FIND_PLUGINS VARNAME RESULT_NAME0 RESULT_NAME1)

	IF(WOSG_USE_STATIC_LIBRARY)
	
		#lib名称
		SET(PLUGINS_LIB_NAME osgdb_${ARGV0})
		STRING(TOUPPER ${ARGV0} PLUGINS_LIB_NAME_UPPER )
		SET(PLUGINS_LIB WOSG_STATIC_PLUGINS_${PLUGINS_LIB_NAME_UPPER}_LIBRARY)

		#搜索release
		FIND_LIBRARY( ${PLUGINS_LIB}
						NAMES ${WOSG_PLUGINS_DIR_NAME}/${PLUGINS_LIB_NAME}${WOSG_STATIC_PLUGINS_POSTFIX}
						PATHS 
						"C:/Program Files/OpenSceneGraph/lib"
						"C:/Program Files (x86)/OpenSceneGraph/lib/"
						# $ENV{OSGDIR}/build/static/lib/osgPlugins-2.9.9
                        ${WOSG_INCLUDE_BUILD_DIR}/../lib/
						/usr/local/lib/
						) 
                        
        # message (waring " " ${WOSG_PLUGINS_DIR_NAME}/${PLUGINS_LIB_NAME}${WOSG_STATIC_PLUGINS_POSTFIX})
		#追加
		set (${RESULT_NAME0} ${${PLUGINS_LIB}} PARENT_SCOPE)

        
		#修改库名称
		SET(PLUGINS_LIB_NAME osgdb_${ARGV0}d)
		SET(PLUGINS_LIB WOSG_STATIC_PLUGINS_${PLUGINS_LIB_NAME_UPPER}_LIBRARY_DEBUG)

		#搜索debug
		FIND_LIBRARY( ${PLUGINS_LIB}
						NAMES ${WOSG_PLUGINS_DIR_NAME}/${PLUGINS_LIB_NAME}${WOSG_STATIC_PLUGINS_POSTFIX}
						PATHS 
						"C:/Program Files/OpenSceneGraph/lib"
						"C:/Program Files (x86)/OpenSceneGraph/lib/"
						# $ENV{OSGDIR}/build/static/lib/osgPlugins-2.9.9
						/usr/local/lib/
                        ${WOSG_INCLUDE_BUILD_DIR}/../lib/
						) 
		#追加
		set (${RESULT_NAME1} ${${PLUGINS_LIB}} PARENT_SCOPE)
	
	ELSE(WOSG_USE_STATIC_LIBRARY)
	ENDIF(WOSG_USE_STATIC_LIBRARY)

ENDFUNCTION(WOSG_FIND_PLUGINS)



#搜索OpenThreads库DLL和静态
MACRO(WOSG_SETUP_OPENTHREADS_LIBRARIES)
	IF(WOSG_USE_STATIC_LIBRARY)
    
        #搜索debug
		FIND_LIBRARY( WOSG_STATIC_OPENTHREADS_LIBRARY_DEBUG
						NAMES ${WOSG_STATIC_OT_LIBRARY_PREFIX}OpenThreadsd${WOSG_STATIC_LIBRARY_POSTFIX}
						PATHS 
						"C:/Program Files/OpenSceneGraph/lib"
						"C:/Program Files (x86)/OpenSceneGraph/lib"
						# $ENV{OSGDIR}/build/static/bin
						/usr/local/lib/
                        ${WOSG_INCLUDE_BUILD_DIR}/../lib/
						) 
		#追加
        if( WOSG_STATIC_OPENTHREADS_LIBRARY_DEBUG )
            list(APPEND WOSG_LIBRARY_ALL debug ${WOSG_STATIC_OPENTHREADS_LIBRARY_DEBUG} )
        endif()
        
		#搜索release
        message("name=")
        message(${WOSG_INCLUDE_BUILD_DIR}/../lib/)
        message(${WOSG_STATIC_OT_LIBRARY_PREFIX}OpenThreads${WOSG_STATIC_LIBRARY_POSTFIX})
		FIND_LIBRARY( WOSG_STATIC_OPENTHREADS_LIBRARY
						NAMES ${WOSG_STATIC_OT_LIBRARY_PREFIX}OpenThreads${WOSG_STATIC_LIBRARY_POSTFIX}
						PATHS
                        ${WOSG_INCLUDE_BUILD_DIR}/../lib
						"C:/Program Files/OpenSceneGraph/lib"
						"C:/Program Files (x86)/OpenSceneGraph/lib"
						# $ENV{OSGDIR}/build/static/bin
						/usr/local/lib/
						) 
		#追加
        if( WOSG_STATIC_OPENTHREADS_LIBRARY_DEBUG )
             list(APPEND WOSG_LIBRARY_ALL optimized )
             # message(${WOSG_STATIC_OPENTHREADS_LIBRARY_DEBUG})
        endif()
		list(APPEND WOSG_LIBRARY_ALL ${WOSG_STATIC_OPENTHREADS_LIBRARY} )
        # message(${WOSG_LIBRARY_ALL})

	
	ELSE(WOSG_USE_STATIC_LIBRARY)
		#搜索release
		FIND_LIBRARY( WOSG_DYNAMIC_OPENTHREADS_LIBRARY
						NAMES OpenThreads
						PATHS 
						"C:/Program Files/OpenSceneGraph/lib"
						"C:/Program Files (x86)/OpenSceneGraph/lib"
						# $ENV{OSGDIR}/build/dynamic/lib
						/usr/local/lib/
                        ${WOSG_INCLUDE_BUILD_DIR}/../lib/
						) 
		#追加
		list(APPEND WOSG_LIBRARY_ALL optimized ${WOSG_DYNAMIC_OPENTHREADS_LIBRARY} )
		#搜索debug
		FIND_LIBRARY( WOSG_DYNAMIC_OPENTHREADS_LIBRARY_DEBUG
						NAMES OpenThreadsd
						PATHS 
						"C:/Program Files/OpenSceneGraph/lib"
						"C:/Program Files (x86)/OpenSceneGraph/lib"
						# $ENV{OSGDIR}/build/dynamic/lib
						/usr/local/lib/
                        ${WOSG_INCLUDE_BUILD_DIR}/../lib/
						) 
		#追加
		list(APPEND WOSG_LIBRARY_ALL debug ${WOSG_DYNAMIC_OPENTHREADS_LIBRARY_DEBUG} )
	ENDIF(WOSG_USE_STATIC_LIBRARY)

ENDMACRO(WOSG_SETUP_OPENTHREADS_LIBRARIES)


#搜索库DLL
MACRO(WOSG_SETUP_OSG_LIBRARIES)

	IF(WOSG_USE_STATIC_LIBRARY)
	
        STRING(TOUPPER ${ARGV0} dll_imp_lib_up )
    
    
        #lib名称
		SET(dll_imp_lib ${WOSG_STATIC_LIBRARY_PREFIX}${ARGV0}d)

		#库变量名称
		SET(dll_imp_lib_v WOSG_STATIC_${dll_imp_lib_up}_LIBRARY_DEBUG)

		#搜索debug
		FIND_LIBRARY( ${dll_imp_lib_v}
						NAMES ${dll_imp_lib}${WOSG_STATIC_LIBRARY_POSTFIX}
						PATHS 
						"C:/Program Files/OpenSceneGraph/lib"
						"C:/Program Files (x86)/OpenSceneGraph/lib"
						# $ENV{OSGDIR}/build/static/bin
						/usr/local/bin/
                        ${WOSG_INCLUDE_BUILD_DIR}/../lib/
						) 
		#追加
        if( ${dll_imp_lib_v} )
            list(APPEND WOSG_LIBRARY_ALL debug ${${dll_imp_lib_v}} )
        endif()
        
        
   
		#lib名称
		SET(dll_imp_lib ${WOSG_STATIC_LIBRARY_PREFIX}${ARGV0})

		#库变量名称
		SET(dll_imp_lib_v2 WOSG_STATIC_${dll_imp_lib_up}_LIBRARY)

		#搜索release
		FIND_LIBRARY( ${dll_imp_lib_v2}
						NAMES ${dll_imp_lib}${WOSG_STATIC_LIBRARY_POSTFIX}
						PATHS 
						"C:/Program Files/OpenSceneGraph/lib"
						"C:/Program Files (x86)/OpenSceneGraph/lib"
						# $ENV{OSGDIR}/build/static/bin
						/usr/local/lib/
                        ${WOSG_INCLUDE_BUILD_DIR}/../lib/
						) 

		#追加
        if( ${dll_imp_lib_v} )
            list(APPEND WOSG_LIBRARY_ALL optimized )
        endif()
        list(APPEND WOSG_LIBRARY_ALL " " )
        list(APPEND WOSG_LIBRARY_ALL ${${dll_imp_lib_v2}} )
        

	ELSE(WOSG_USE_STATIC_LIBRARY)
	
		#lib名称
		SET(dll_imp_lib ${ARGV0})

		#库变量名称
		STRING(TOUPPER ${ARGV0} dll_imp_lib_up )
		SET(dll_imp_lib_v WOSG_DYNAMIC_${dll_imp_lib_up}_LIBRARY)

		
		#搜索release
		FIND_LIBRARY( ${dll_imp_lib_v}
						NAMES ${dll_imp_lib} 
						PATHS 
						"C:/Program Files/OpenSceneGraph/lib"
						"C:/Program Files (x86)/OpenSceneGraph/lib"
						# $ENV{OSGDIR}/build/dynamic/lib
						/usr/local/lib/
                        ${WOSG_INCLUDE_BUILD_DIR}/../lib/
						) 
		#追加
		list(APPEND WOSG_LIBRARY_ALL optimized ${${dll_imp_lib_v}} )
		
		
		#修改库名称
		SET(dll_imp_lib ${ARGV0}d)

		#修改库变量名称
		SET(dll_imp_lib_v WOSG_DYNAMIC_${dll_imp_lib_up}_LIBRARY_DEBUG)

		#搜索debug
		FIND_LIBRARY( ${dll_imp_lib_v}
						NAMES ${dll_imp_lib}
						PATHS 
						"C:/Program Files/OpenSceneGraph/lib"
						"C:/Program Files (x86)/OpenSceneGraph/lib"
						# $ENV{OSGDIR}/build/dynamic/lib
						/usr/local/lib/
                        ${WOSG_INCLUDE_BUILD_DIR}/../lib/
						) 

		#追加
		list(APPEND WOSG_LIBRARY_ALL debug ${${dll_imp_lib_v}})
        
	ENDIF(WOSG_USE_STATIC_LIBRARY)
    
ENDMACRO(WOSG_SETUP_OSG_LIBRARIES)






#搜索OSG的include目录
FIND_PATH( WOSG_INCLUDE_DIR osg/Object
			"C:/Program Files/OpenSceneGraph/include"
			"C:/Program Files (x86)/OpenSceneGraph/include"
			# $ENV{OSGDIR}/Include
			/usr/local/Include/
            ../../perbuild/include
			) 
            
#搜索OSG的include目录
FIND_PATH( WOSG_INCLUDE_DYNAMIC_BUILD_DIR osg/Config
			${WOSG_INCLUDE_DIR}/
			"C:/Program Files/OpenSceneGraph/include"
			"C:/Program Files (x86)/OpenSceneGraph/include"
			# $ENV{OSGDIR}/Include
			/usr/local/Include/
            ../../perbuild/include
			) 

FIND_PATH( WOSG_INCLUDE_STATIC_BUILD_DIR osg/Config
            ${WOSG_INCLUDE_DIR}/
            "C:/Program Files/OpenSceneGraph/include"
            "C:/Program Files (x86)/OpenSceneGraph/include"
            # $ENV{OSGDIR}/Include
            /usr/local/Include/
            ../../perbuild/include
            ) 
            
set ( WOSG_INCLUDE_BUILD_DIR )            
IF(WOSG_USE_STATIC_LIBRARY)
    set ( WOSG_INCLUDE_BUILD_DIR ${WOSG_INCLUDE_STATIC_BUILD_DIR})
ELSE(WOSG_USE_STATIC_LIBRARY)
    set ( WOSG_INCLUDE_BUILD_DIR ${WOSG_INCLUDE_DYNAMIC_BUILD_DIR})
ENDIF(WOSG_USE_STATIC_LIBRARY)           
	  
message(${WOSG_INCLUDE_BUILD_DIR})
      
#搜索OT
WOSG_SETUP_OPENTHREADS_LIBRARIES()


#搜索所有模块库 动态静态自动识别
foreach(_osg_component ${MODULE_NAME_LIST})
   WOSG_SETUP_OSG_LIBRARIES(${_osg_component})
endforeach()
# MESSAGE(STATUS ${WOSG_LIBRARY_ALL})	
# MESSAGE(STATUS ${WOSG_LIBRARY_ALL})	


#搜索所有静态插件库 动态静态自动识别
IF(WOSG_USE_STATIC_LIBRARY)

	#SET(PLUGINS_NAME)
	foreach(PLUGINS_NAME ${PLUGINS_NAME_LIST})
		# list(APPEND _osg_modules_to_process ${_osg_component})

		#返回值
		SET(PLUGINS_LIB_RELEASE)
		SET(PLUGINS_LIB_DEBUG)

		WOSG_FIND_PLUGINS( ${PLUGINS_NAME} PLUGINS_LIB_RELEASE PLUGINS_LIB_DEBUG )
		
		IF( WOSG_USE_STATIC_LIBRARY)
			# MESSAGE(WARING ${PLUGINS_LIB_RELEASE})
			# MESSAGE(WARING ${PLUGINS_LIB_DEBUG})
			list(APPEND WOSG_LIBRARY_ALL_PLUGINS   optimized ${PLUGINS_LIB_RELEASE})
			list(APPEND WOSG_LIBRARY_ALL_PLUGINS   debug ${PLUGINS_LIB_DEBUG})
		ENDIF(WOSG_USE_STATIC_LIBRARY)
        
	endforeach()
	
	
	#测试输出
	# MESSAGE(STATUS ${WOSG_LIBRARY_ALL_PLUGINS})	
	# MESSAGE(STATUS ${WOSG_LIBRARY_ALL_PLUGINS})	

	# MESSAGE(STATUS ${FIND_PLUGINS(osg)})	
	# MESSAGE(STATUS FIND_PLUGINS( osg ${PLUGINS_LIST}))	

	
	IF(MSVC)
        #搜索第三方库
        # SET(WOSG_LIBRARY_ALL_DEPEND)
        SET(WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR ${PROJECT_SOURCE_DIR}/../../perbuild/lib  CACHE STRING "" )
        SET(WOSG_LIBRARY_POSTFIX .lib )
        
        #追加
        list(APPEND WOSG_LIBRARY_ALL_DEPEND optimized  ${WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR}/freetype${WOSG_LIBRARY_POSTFIX} )
        list(APPEND WOSG_LIBRARY_ALL_DEPEND debug      ${WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR}/freetyped${WOSG_LIBRARY_POSTFIX}  )
        
        list(APPEND WOSG_LIBRARY_ALL_DEPEND optimized  ${WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR}/jpeg${WOSG_LIBRARY_POSTFIX} )
        list(APPEND WOSG_LIBRARY_ALL_DEPEND debug      ${WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR}/jpegd${WOSG_LIBRARY_POSTFIX}  )
        
        list(APPEND WOSG_LIBRARY_ALL_DEPEND optimized  ${WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR}/libpng16${WOSG_LIBRARY_POSTFIX} )
        list(APPEND WOSG_LIBRARY_ALL_DEPEND debug      ${WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR}/libpng16d${WOSG_LIBRARY_POSTFIX}  )
        
        # list(APPEND WOSG_LIBRARY_ALL_DEPEND optimized  ${WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR}/zlib${WOSG_LIBRARY_POSTFIX} )
        # list(APPEND WOSG_LIBRARY_ALL_DEPEND debug      ${WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR}/zlibd${WOSG_LIBRARY_POSTFIX}  )
        
        list(APPEND WOSG_LIBRARY_ALL_DEPEND optimized  ${WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR}/libtiff_i${WOSG_LIBRARY_POSTFIX} )
        list(APPEND WOSG_LIBRARY_ALL_DEPEND debug      ${WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR}/libtiffd_i${WOSG_LIBRARY_POSTFIX}  )
        
        list(APPEND WOSG_LIBRARY_ALL_DEPEND optimized  ${WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR}/libcurl${WOSG_LIBRARY_POSTFIX} )
        list(APPEND WOSG_LIBRARY_ALL_DEPEND debug      ${WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR}/libcurld${WOSG_LIBRARY_POSTFIX}  )
        
        # list(APPEND WOSG_LIBRARY_ALL_DEPEND optimized  ${WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR}/static/nvtt${WOSG_LIBRARY_POSTFIX} )
        # list(APPEND WOSG_LIBRARY_ALL_DEPEND debug      ${WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR}/static/nvttd${WOSG_LIBRARY_POSTFIX}  )
        # list(APPEND WOSG_LIBRARY_ALL_DEPEND optimized  ${WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR}/static/nvcore${WOSG_LIBRARY_POSTFIX} )
        # list(APPEND WOSG_LIBRARY_ALL_DEPEND debug      ${WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR}/static/nvcored${WOSG_LIBRARY_POSTFIX}  )
        # list(APPEND WOSG_LIBRARY_ALL_DEPEND optimized  ${WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR}/static/nvimage${WOSG_LIBRARY_POSTFIX} )
        # list(APPEND WOSG_LIBRARY_ALL_DEPEND debug      ${WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR}/static/nvimaged${WOSG_LIBRARY_POSTFIX}  )
        # list(APPEND WOSG_LIBRARY_ALL_DEPEND optimized  ${WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR}/static/nvmath${WOSG_LIBRARY_POSTFIX} )
        # list(APPEND WOSG_LIBRARY_ALL_DEPEND debug      ${WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR}/static/nvmathd${WOSG_LIBRARY_POSTFIX}  )
        # list(APPEND WOSG_LIBRARY_ALL_DEPEND optimized  ${WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR}/static/squish${WOSG_LIBRARY_POSTFIX} )
        # list(APPEND WOSG_LIBRARY_ALL_DEPEND debug      ${WOSG_LIBRARY_ALL_3RDPARTY_LIBDIR}/static/squishd${WOSG_LIBRARY_POSTFIX}  )
        
	ENDIF(MSVC)
    
ENDIF(WOSG_USE_STATIC_LIBRARY)




	   