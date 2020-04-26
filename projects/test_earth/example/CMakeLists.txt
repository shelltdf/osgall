SET(EXAMPLE_NAME osgMuti)

# SET(EXAMPLE_FILES hello.cpp)
SET(EXAMPLE_FILES 

InstancedObject.h

GLES2ShaderGenVisitor.h
GLES2ShaderGenVisitor.cpp
main.cpp

# graphics_3d.cc
# matrix.cc
# texture.cc
)


INCLUDE_DIRECTORIES(AFTER ${WOSG_INCLUDE_BUILD_DIR})
INCLUDE_DIRECTORIES(AFTER ${WOSG_INCLUDE_DIR})
link_directories(${WOSG_LIBRARY_DIR})
link_directories(${WOSG_LIBRARY_DIR}/gdal)

IF(PPAPI)
    ADD_DEFINITIONS(-DNIUBI_USE_PPAPI)
    ADD_DEFINITIONS(-DNIUBI_LIBRARY)
ENDIF(PPAPI)

IF(NACL)
    ADD_DEFINITIONS(-DNIUBI_USE_PNACL)
ENDIF(NACL)

IF(ANDROID OR PPAPI)
    IF(WOSG_USE_STATIC_LIBRARY)
        # message("cccccccccccc")
        ADD_DEFINITIONS(-DOSG_LIBRARY_STATIC)
        ADD_DEFINITIONS(-DOSGEARTH_LIBRARY_STATIC)
        ADD_DEFINITIONS(-DOSGEARTHFEATURES_LIBRARY_STATIC)
        ADD_DEFINITIONS(-DOSGEARTHUTIL_LIBRARY_STATIC)
        ADD_DEFINITIONS(-DOSGEARTHSYMBOLOGY_LIBRARY_STATIC)
        ADD_DEFINITIONS(-DOSGEARTHANNO_LIBRARY_STATIC)

        ADD_DEFINITIONS(-DXML_STATIC)
        # ADD_DEFINITIONS(-DXMLCALL)
        # ADD_DEFINITIONS(-DXML_BUILDING_EXPAT)
        ADD_DEFINITIONS(-DCPL_DISABLE_DLL)
        ADD_DEFINITIONS(-DCPL_DISABLE_STDCALL)
    ENDIF(WOSG_USE_STATIC_LIBRARY)

    ADD_LIBRARY(${EXAMPLE_NAME} SHARED ${EXAMPLE_FILES})

    SET_TARGET_PROPERTIES(${EXAMPLE_NAME} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:\"LIBCMT\"")


ELSE(ANDROID OR PPAPI)
    IF(WOSG_USE_STATIC_LIBRARY)
        # message(USE_STATIC_LIBRARY)
        ADD_DEFINITIONS(-DOSG_LIBRARY_STATIC)
        ADD_DEFINITIONS(-DOSGEARTH_LIBRARY_STATIC)
        
        ADD_EXECUTABLE(${EXAMPLE_NAME} ${EXAMPLE_FILES})
        
        # message("aaaaaaaaaaaa")
    ELSE(WOSG_USE_STATIC_LIBRARY)
        # message(NO_USE_STATIC_LIBRARY)
        ADD_EXECUTABLE(${EXAMPLE_NAME} ${EXAMPLE_FILES})
        # message("bbbbbbbbbbbb")
    ENDIF(WOSG_USE_STATIC_LIBRARY)
ENDIF(ANDROID OR PPAPI)

SET_TARGET_PROPERTIES(${EXAMPLE_NAME} PROPERTIES PROJECT_LABEL "${EXAMPLE_NAME}")
SET_TARGET_PROPERTIES(${EXAMPLE_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
SET_TARGET_PROPERTIES(${EXAMPLE_NAME} PROPERTIES OUTPUT_NAME ${EXAMPLE_NAME})

IF( MSVC_IDE)
IF(MSVC_IDE AND ${MSVC_VERSION} LESS 1600)
	SET_TARGET_PROPERTIES(${EXAMPLE_NAME} PROPERTIES  PREFIX "../")
ENDIF(MSVC_IDE AND ${MSVC_VERSION} LESS 1600)
IF(MSVC_IDE AND ${MSVC_VERSION} EQUAL 1600)
    SET_TARGET_PROPERTIES(${EXAMPLE_NAME} PROPERTIES "RUNTIME_OUTPUT_DIRECTORY_RELEASE" "../../bin")
    SET_TARGET_PROPERTIES(${EXAMPLE_NAME} PROPERTIES "RUNTIME_OUTPUT_DIRECTORY_DEBUG" "../../bin")
ENDIF(MSVC_IDE AND ${MSVC_VERSION} EQUAL 1600)
ENDIF( MSVC_IDE)

# message(${WOSG_LIBRARY_ALL})
# message(${WOSG_LIBRARY_ALL_PLUGINS})

if(ANDROID)

    TARGET_LINK_LIBRARIES(${EXAMPLE_NAME}
        # ${WOSG_LIBRARY_ALL}
        # ${WOSG_LIBRARY_ALL_PLUGINS}
        # opengl32 glu32
        # ${OPENGL_LIBRARIES}
        android log
        EGL GLESv1_CM
        android_native_app_glue
        )
    
elseif(NACL)

	message(${WOSG_LIBRARY_ALL})
	message(${WOSG_LIBRARY_DIR})
	
    set_target_properties(${EXAMPLE_NAME} PROPERTIES
        SUFFIX ${PLATFORM_EXE_SUFFIX}
        )
    TARGET_LINK_LIBRARIES(${EXAMPLE_NAME}

        #切记 链接的顺序有影响  上面依赖下面的库
        
        # optimized ${OSGEARTH_LIBRARY}               debug ${OSGEARTH_LIBRARY_DEBUG}
		# optimized ${OSGEARTHFEATURES_LIBRARY}       debug ${OSGEARTHFEATURES_LIBRARY_DEBUG}
		# optimized ${OSGEARTHUTIL_LIBRARY}           debug ${OSGEARTHUTIL_LIBRARY_DEBUG}
		# optimized ${OSGEARTHQT_LIBRARY}             debug ${OSGEARTHQT_LIBRARY_DEBUG}
		# optimized ${OSGEARTHSYMBOLOGY_LIBRARY}      debug ${OSGEARTHSYMBOLOGY_LIBRARY_DEBUG}
		# optimized ${OSGEARTHANNOTATION_LIBRARY}     debug ${OSGEARTHANNOTATION_LIBRARY_DEBUG}
	
		osgEarth 
		osgEarthFeatures
		osgEarthUtil
		osgEarthSymbology
		osgEarthAnnotation
		
		${OSGEARTH_LIBRARY} 
		${OSGEARTHFEATURES_LIBRARY}
		${OSGEARTHUTIL_LIBRARY}
		# ${OSGEARTHQT_LIBRARY}
		${OSGEARTHSYMBOLOGY_LIBRARY}
		${OSGEARTHANNOTATION_LIBRARY}
        
		osgViewer osg osgGA osgUtil osgDB OpenThreads
        osgShadow osgSim osgTerrain 
        osgText osgAnimation osgManipulator
		${WOSG_LIBRARY_ALL}
		
		gdal111 ${GDAL_LIBRARY}
        curl ${CURL_LIBRARY}
        geos_c ${GEOS_C_LIBRARY}
        geos ${GEOS_LIBRARY} 
        z ${ZLIB_LIBRARY_RELEASE}
        geotiffd ${GEOTIFF_LIBRARY_RELEASE}
        tiff ${TIFF_LIBRARY_RELEASE}
        xtiffd ${XTIFF_LIBRARY}
        proj ${PROJ_LIBRARY}
        
        ppapi_gles2 ppapi ppapi_cpp sdk_util nacl_io
        
        )
    pnacl_finalise(${EXAMPLE_NAME})
    
else(ANDROID)

# pc

set(ADD_LIBRARIES "")
MACRO( ADD_LIBRARIES MYLIBRARY MYLIBRARY_DEBUG )
    if(${MYLIBRARY} AND ${MYLIBRARY_DEBUG})
        list(APPEND ADD_LIBRARIES optimized ${${MYLIBRARY}} debug ${${MYLIBRARY_DEBUG}}  )
    elseif(${MYLIBRARY})
        list(APPEND ADD_LIBRARIES ${${MYLIBRARY}} )
    elseif(${MYLIBRARY_DEBUG})
        list(APPEND ADD_LIBRARIES ${${MYLIBRARY_DEBUG}} )
    endif()
ENDMACRO(ADD_LIBRARIES MYLIBRARY MYLIBRARY_DEBUG)


    # SET( STATIC_LIBS "")
    # IF(WOSG_USE_STATIC_LIBRARY)
        # SET( STATIC_LIBS ${STATIC_LIBS} ${WOSGEARTH_LIBRARY_ALL_PLUGINS})
        # SET( STATIC_LIBS ${STATIC_LIBS} 
            # optimized ${GDAL_LIBRARY}    debug ${GDAL_LIBRARY_DEBUG}
            # optimized ${CURL_LIBRARY}    debug ${CURL_LIBRARY_DEBUG}
            # optimized ${GEOS_C_LIBRARY}  debug ${GEOS_C_LIBRARY_DEBUG}
            # optimized ${GEOS_LIBRARY}    debug ${GEOS_LIBRARY_DEBUG}
            # optimized ${ZLIB_LIBRARY}    debug ${ZLIB_LIBRARY_DEBUG}
            # optimized ${geotiff_LIBRARY} debug ${geotiff_LIBRARY_DEBUG}
            # optimized ${TIFF_LIBRARY}    debug ${TIFF_LIBRARY_DEBUG}
            # optimized ${xtiff_LIBRARY}   debug ${xtiff_LIBRARY_DEBUG}
            # optimized ${PROJ_LIBRARY}    debug ${PROJ_LIBRARY_DEBUG}
            # optimized ${PNG_LIBRARY}     debug ${PNG_LIBRARY_DEBUG}
            # optimized ${EXPAT_LIBRARY}   debug ${EXPAT_LIBRARY_DEBUG}
            # optimized ${SQLITE3_LIBRARY} debug ${SQLITE3_LIBRARY_DEBUG}
            # optimized ${FREETYPE_LIBRARY_RELEASE} debug ${FREETYPE_LIBRARY_DEBUG}
            # optimized ${JPEG_LIBRARY}    debug ${JPEG_LIBRARY_DEBUG}
        # )
    # ENDIF(WOSG_USE_STATIC_LIBRARY)
    
    ADD_LIBRARIES( GDAL_LIBRARY    GDAL_LIBRARY_DEBUG)
    ADD_LIBRARIES( CURL_LIBRARY    CURL_LIBRARY_DEBUG)
    ADD_LIBRARIES( GEOS_C_LIBRARY  GEOS_C_LIBRARY_DEBUG)
    ADD_LIBRARIES( GEOS_LIBRARY    GEOS_LIBRARY_DEBUG)
    ADD_LIBRARIES( ZLIB_LIBRARY    ZLIB_LIBRARY_DEBUG)
    ADD_LIBRARIES( GEOTIFF_LIBRARY GEOTIFF_LIBRARY_DEBUG)
    ADD_LIBRARIES( TIFF_LIBRARY    TIFF_LIBRARY_DEBUG)
    ADD_LIBRARIES( XTIFF_LIBRARY   XTIFF_LIBRARY_DEBUG)
    ADD_LIBRARIES( PROJ_LIBRARY    PROJ_LIBRARY_DEBUG)
    ADD_LIBRARIES( PNG_LIBRARY     PNG_LIBRARY_DEBUG)
    ADD_LIBRARIES( EXPAT_LIBRARY   EXPAT_LIBRARY_DEBUG)
    ADD_LIBRARIES( SQLITE3_LIBRARY SQLITE3_LIBRARY_DEBUG)
    ADD_LIBRARIES( FREETYPE_LIBRARY_RELEASE FREETYPE_LIBRARY_DEBUG)
    ADD_LIBRARIES( JPEG_LIBRARY    JPEG_LIBRARY_DEBUG)
            
    #pc
    TARGET_LINK_LIBRARIES(${EXAMPLE_NAME}
    
        ${ADD_LIBRARIES}
        
        # optimized ${OSGEARTHSYMBOLOGY_LIBRARY}      debug ${OSGEARTHSYMBOLOGY_LIBRARY_DEBUG}
        
		# optimized ${OSGEARTHFEATURES_LIBRARY}       debug ${OSGEARTHFEATURES_LIBRARY_DEBUG}
		# optimized ${OSGEARTHUTIL_LIBRARY}           debug ${OSGEARTHUTIL_LIBRARY_DEBUG}
		# optimized ${OSGEARTHQT_LIBRARY}             debug ${OSGEARTHQT_LIBRARY_DEBUG}
		
		# optimized ${OSGEARTHANNOTATION_LIBRARY}     debug ${OSGEARTHANNOTATION_LIBRARY_DEBUG}
        # optimized ${OSGEARTH_LIBRARY}               debug ${OSGEARTH_LIBRARY_DEBUG}
        
        ${OSGEARTH_LIBRARIES}
        ${WOSGEARTH_LIBRARY_ALL_PLUGINS}
        
        ${WOSG_LIBRARY_ALL}
        ${WOSG_LIBRARY_ALL_PLUGINS}
        
        # optimized ${GDAL_LIBRARY}    debug ${GDAL_LIBRARY_DEBUG}
        # optimized ${CURL_LIBRARY}    debug ${CURL_LIBRARY_DEBUG}
        # optimized ${GEOS_C_LIBRARY}  debug ${GEOS_C_LIBRARY_DEBUG}
        # optimized ${GEOS_LIBRARY}    debug ${GEOS_LIBRARY_DEBUG}
        # optimized ${ZLIB_LIBRARY}    debug ${ZLIB_LIBRARY_DEBUG}
        # optimized ${geotiff_LIBRARY} debug ${geotiff_LIBRARY_DEBUG}
        # optimized ${TIFF_LIBRARY}    debug ${TIFF_LIBRARY_DEBUG}
        # optimized ${xtiff_LIBRARY}   debug ${xtiff_LIBRARY_DEBUG}
        # optimized ${PROJ_LIBRARY}    debug ${PROJ_LIBRARY_DEBUG}
        # optimized ${PNG_LIBRARY}     debug ${PNG_LIBRARY_DEBUG}
        # optimized ${EXPAT_LIBRARY}   debug ${EXPAT_LIBRARY_DEBUG}
        # optimized ${SQLITE3_LIBRARY} debug ${SQLITE3_LIBRARY_DEBUG}
        # optimized ${FREETYPE_LIBRARY_RELEASE} debug ${FREETYPE_LIBRARY_DEBUG}
        # optimized ${JPEG_LIBRARY}    debug ${JPEG_LIBRARY_DEBUG}
        
        
        ${OPENGL_LIBRARIES}
        
        # Ws2_32.lib
        
		# D:\nacl_sdk\pepper_49\lib\win_x86_64_host\Release\ppapi_gles2.lib

		#optimized D:\nacl_sdk\pepper_49\lib\win_x86_32_host\Release\ppapi_gles2.lib
		#debug D:\nacl_sdk\pepper_49\lib\win_x86_32_host\Debug\ppapi_gles2.lib

        
        # optimized ${OSGEARTH_LIBRARY}               debug ${OSGEARTH_LIBRARY_DEBUG}
		# optimized ${OSGEARTHFEATURES_LIBRARY}       debug ${OSGEARTHFEATURES_LIBRARY_DEBUG}
		# optimized ${OSGEARTHUTIL_LIBRARY}           debug ${OSGEARTHUTIL_LIBRARY_DEBUG}
		# optimized ${OSGEARTHQT_LIBRARY}             debug ${OSGEARTHQT_LIBRARY_DEBUG}
		# optimized ${OSGEARTHSYMBOLOGY_LIBRARY}      debug ${OSGEARTHSYMBOLOGY_LIBRARY_DEBUG}
		# optimized ${OSGEARTHANNOTATION_LIBRARY}     debug ${OSGEARTHANNOTATION_LIBRARY_DEBUG}
        
        ${OSGEARTH_LIBRARIES}
        
        
        # optimized ${GDAL_LIBRARY}               debug ${GDAL_LIBRARY_DEBUG}  
        # optimized ${CURL_LIBRARY}               debug ${CURL_LIBRARY_DEBUG}     
        # optimized ${GEOS_C_LIBRARY}             debug ${GEOS_C_LIBRARY_DEBUG}  
        # optimized ${GEOS_LIBRARY}               debug ${GEOS_LIBRARY_DEBUG} 
        # optimized ${ZLIB_LIBRARY}               debug ${ZLIB_LIBRARY_DEBUG}  
        # optimized ${GEOTIFF_LIBRARY}            debug ${GEOTIFF_LIBRARY_DEBUG}
        # optimized ${TIFF_LIBRARY}               debug ${TIFF_LIBRARY_DEBUG}   
        # optimized ${XTIFF_LIBRARY}              debug ${XTIFF_LIBRARY_DEBUG} 
        # optimized ${PROJ_LIBRARY}               debug ${PROJ_LIBRARY_DEBUG}  
        # optimized ${SQLITE3_LIBRARY}            debug ${SQLITE3_LIBRARY_DEBUG}  
        # optimized ${LIBGIF_LIBRARY}             debug ${LIBGIF_LIBRARY_DEBUG}  
        # optimized ${LIBXML_LIBRARY}             debug ${LIBXML_LIBRARY_DEBUG}  
        # optimized ${EXPAT_LIBRARY}              debug ${EXPAT_LIBRARY_DEBUG}  
        
        ws2_32.lib
        )
endif(ANDROID)

IF(ANDROID)
INSTALL(TARGETS ${EXAMPLE_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
ELSEIF(NACL)
# no install pexe file
ELSE(ANDROID)
INSTALL(TARGETS ${EXAMPLE_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
ENDIF(ANDROID)
