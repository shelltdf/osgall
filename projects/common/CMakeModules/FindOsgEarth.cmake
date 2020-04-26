# This module defines

# OSGEARTH_LIBRARY
# OSGEARTH_FOUND, if false, do not try to link to osg
# OSGEARTH_INCLUDE_DIRS, where to find the headers
# OSGEARTH_INCLUDE_DIR, where to find the source headers

# to use this module, set variables to point to the osg build
# directory, and source directory, respectively
# OSGEARTHDIR or OSGEARTH_SOURCE_DIR: osg source directory, typically OpenSceneGraph
# OSGEARTH_DIR or OSGEARTH_BUILD_DIR: osg build directory, place in which you've
#    built osg via cmake

# Header files are presumed to be included like
# #include <osg/PositionAttitudeTransform>
# #include <osgUtil/SceneView>

###### headers ######

MACRO( FIND_OSGEARTH_INCLUDE THIS_OSGEARTH_INCLUDE_DIR THIS_OSGEARTH_INCLUDE_FILE )

FIND_PATH( ${THIS_OSGEARTH_INCLUDE_DIR} ${THIS_OSGEARTH_INCLUDE_FILE}
    PATHS
        ${OSGEARTH_DIR}
        $ENV{OSGEARTH_SOURCE_DIR}
        $ENV{OSGEARTHDIR}
        $ENV{OSGEARTH_DIR}
        /usr/local/
        /usr/
        /sw/ # Fink
        /opt/local/ # DarwinPorts
        /opt/csw/ # Blastwave
        /opt/
        [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;OSGEARTH_ROOT]/
        ~/Library/Frameworks
        /Library/Frameworks
    PATH_SUFFIXES
        /include/
)

ENDMACRO( FIND_OSGEARTH_INCLUDE THIS_OSGEARTH_INCLUDE_DIR THIS_OSGEARTH_INCLUDE_FILE )

FIND_OSGEARTH_INCLUDE( OSGEARTH_INCLUDE_DIR       osgEarth/Version )

###### libraries ######

MACRO( FIND_OSGEARTH_LIBRARY MYLIBRARY MYLIBRARYNAME )

FIND_LIBRARY(${MYLIBRARY}
    NAMES
        ${MYLIBRARYNAME}
    PATHS
        ${OSGEARTH_DIR}
        $ENV{OSGEARTH_BUILD_DIR}
        $ENV{OSGEARTH_DIR}
        $ENV{OSGEARTHDIR}
        $ENV{OSG_ROOT}
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw
        /opt/local
        /opt/csw
        /opt
        [HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session\ Manager\\Environment;OSGEARTH_ROOT]/lib
        /usr/freeware
    PATH_SUFFIXES
        /lib/
        /lib64/
        /build/lib/
        /build/lib64/
        /Build/lib/
        /Build/lib64/
     )

ENDMACRO(FIND_OSGEARTH_LIBRARY LIBRARY LIBRARYNAME)

FIND_OSGEARTH_LIBRARY( OSGEARTH_LIBRARY osgEarth)
FIND_OSGEARTH_LIBRARY( OSGEARTHFEATURES_LIBRARY osgEarthFeatures)
FIND_OSGEARTH_LIBRARY( OSGEARTHUTIL_LIBRARY osgEarthUtil )
FIND_OSGEARTH_LIBRARY( OSGEARTHQT_LIBRARY osgEarthQt )
FIND_OSGEARTH_LIBRARY( OSGEARTHSYMBOLOGY_LIBRARY osgEarthSymbology )
FIND_OSGEARTH_LIBRARY( OSGEARTHANNOTATION_LIBRARY osgEarthAnnotation )

FIND_OSGEARTH_LIBRARY( OSGEARTH_LIBRARY_DEBUG osgEarthd)
FIND_OSGEARTH_LIBRARY( OSGEARTHFEATURES_LIBRARY_DEBUG osgEarthFeaturesd)
FIND_OSGEARTH_LIBRARY( OSGEARTHUTIL_LIBRARY_DEBUG osgEarthUtild )
FIND_OSGEARTH_LIBRARY( OSGEARTHQT_LIBRARY_DEBUG osgEarthQtd )
FIND_OSGEARTH_LIBRARY( OSGEARTHSYMBOLOGY_LIBRARY_DEBUG osgEarthSymbologyd )
FIND_OSGEARTH_LIBRARY( OSGEARTHANNOTATION_LIBRARY_DEBUG osgEarthAnnotationd )


set(OSGEARTH_LIBRARIES "")

# if(OSGEARTH_LIBRARY AND OSGEARTH_LIBRARY_DEBUG)
    # list(APPEND OSGEARTH_LIBRARIES optimized ${OSGEARTH_LIBRARY} debug ${OSGEARTH_LIBRARY_DEBUG}  )
# elseif(OSGEARTH_LIBRARY)
    # list(APPEND OSGEARTH_LIBRARIES ${OSGEARTH_LIBRARY} )
# elseif(OSGEARTH_LIBRARY_DEBUG)
    # list(APPEND OSGEARTH_LIBRARIES ${OSGEARTH_LIBRARY_DEBUG} )
# endif()
MACRO( ADD_OSGEARTH_LIBRARIES MYLIBRARY MYLIBRARY_DEBUG )
    if(${MYLIBRARY} AND ${MYLIBRARY_DEBUG})
        list(APPEND OSGEARTH_LIBRARIES optimized ${${MYLIBRARY}} debug ${${MYLIBRARY_DEBUG}}  )
    elseif(${MYLIBRARY})
        list(APPEND OSGEARTH_LIBRARIES ${${MYLIBRARY}} )
    elseif(${MYLIBRARY_DEBUG})
        list(APPEND OSGEARTH_LIBRARIES ${${MYLIBRARY_DEBUG}} )
    endif()
ENDMACRO(ADD_OSGEARTH_LIBRARIES MYLIBRARY MYLIBRARY_DEBUG)

ADD_OSGEARTH_LIBRARIES(OSGEARTH_LIBRARY OSGEARTH_LIBRARY_DEBUG)
ADD_OSGEARTH_LIBRARIES(OSGEARTHFEATURES_LIBRARY OSGEARTHFEATURES_LIBRARY_DEBUG)
ADD_OSGEARTH_LIBRARIES(OSGEARTHUTIL_LIBRARY OSGEARTHUTIL_LIBRARY_DEBUG)
ADD_OSGEARTH_LIBRARIES(OSGEARTHQT_LIBRARY OSGEARTHQT_LIBRARY_DEBUG)
ADD_OSGEARTH_LIBRARIES(OSGEARTHSYMBOLOGY_LIBRARY OSGEARTHSYMBOLOGY_LIBRARY_DEBUG)
ADD_OSGEARTH_LIBRARIES(OSGEARTHANNOTATION_LIBRARY OSGEARTHANNOTATION_LIBRARY_DEBUG)

        

SET( OSGEARTH_FOUND "NO" )
IF( OSGEARTH_LIBRARY AND OSGEARTH_INCLUDE_DIR )
    SET( OSGEARTH_FOUND "YES" )
    SET( OSGEARTH_INCLUDE_DIRS ${OSGEARTH_INCLUDE_DIR})
    GET_FILENAME_COMPONENT( OSGEARTH_LIBRARIES_DIR ${OSGEARTH_LIBRARY} PATH )
ENDIF( OSGEARTH_LIBRARY AND OSGEARTH_INCLUDE_DIR )


SET(PLUGINS_NAME_LIST
    earth
    kml

    osgearth_agglite
    osgearth_arcgis_map_cache
    osgearth_arcgis
    osgearth_billboard
    osgearth_bing
    osgearth_bumpmap
    osgearth_cache_filesystem
    osgearth_cache_leveldb
    osgearth_colorramp
    osgearth_debug
    osgearth_engine_byo
    osgearth_engine_mp
    osgearth_feature_elevation
    osgearth_feature_ogr
    osgearth_feature_raster
    osgearth_feature_tfs
    osgearth_feature_wfs
    osgearth_gdal
    osgearth_label_annotation
    osgearth_mapinspector
    osgearth_mask_feature
    osgearth_mbtiles
    osgearth_model_feature_geom
    osgearth_model_feature_stencil
    osgearth_model_simple
    osgearth_noise
    osgearth_normalmap
    osgearth_ocean_simple
    osgearth_osg
    osgearth_quadkey
    osgearth_refresh
    osgearth_scriptengine_javascript
    osgearth_sky_gl
    osgearth_sky_simple
    osgearth_splat_mask
    osgearth_splat
    osgearth_template_matclass
    osgearth_terrainshader
    osgearth_tilecache
    osgearth_tileindex
    osgearth_tileservice
    osgearth_tms
    osgearth_vdatum_egm84
    osgearth_vdatum_egm96
    osgearth_vdatum_egm2008
    osgearth_viewpoints
    osgearth_vpb
    osgearth_wcs
    osgearth_wms
    osgearth_xyz
    osgearth_yahoo
)

set(WOSGEARTH_LIBRARY_ALL_PLUGINS "")


#搜索插件
FUNCTION(WOSGEARTH_FIND_PLUGINS VARNAME RESULT_NAME0 RESULT_NAME1)

	# IF(WOSG_USE_STATIC_LIBRARY)
	
		#lib名称
		SET(PLUGINS_LIB_NAME osgdb_${ARGV0})
		STRING(TOUPPER ${ARGV0} PLUGINS_LIB_NAME_UPPER )
		SET(PLUGINS_LIB WOSG_STATIC_PLUGINS_${PLUGINS_LIB_NAME_UPPER}_LIBRARY)

		#搜索release
		FIND_LIBRARY( ${PLUGINS_LIB}
                NAMES ${PLUGINS_LIB_NAME}
                PATHS ${OSGEARTH_DIR}
                ) 
                        
        # message (waring " " ${WOSG_PLUGINS_DIR_NAME}/${PLUGINS_LIB_NAME}${WOSG_STATIC_PLUGINS_POSTFIX})
		#追加
		set (${RESULT_NAME0} ${${PLUGINS_LIB}} PARENT_SCOPE)

        
		#修改库名称
		SET(PLUGINS_LIB_NAME osgdb_${ARGV0}d)
		SET(PLUGINS_LIB WOSG_STATIC_PLUGINS_${PLUGINS_LIB_NAME_UPPER}_LIBRARY_DEBUG)

		#搜索debug
		FIND_LIBRARY( ${PLUGINS_LIB}
                NAMES ${PLUGINS_LIB_NAME}
                PATHS ${OSGEARTH_DIR}
                ) 
		#追加
		set (${RESULT_NAME1} ${${PLUGINS_LIB}} PARENT_SCOPE)
	
	# ELSE(WOSG_USE_STATIC_LIBRARY)
	# ENDIF(WOSG_USE_STATIC_LIBRARY)

ENDFUNCTION(WOSGEARTH_FIND_PLUGINS)


foreach(PLUGINS_NAME ${PLUGINS_NAME_LIST})
    # list(APPEND _osg_modules_to_process ${_osg_component})

    #返回值
    SET(PLUGINS_LIB_RELEASE)
    SET(PLUGINS_LIB_DEBUG)

    WOSGEARTH_FIND_PLUGINS( ${PLUGINS_NAME} PLUGINS_LIB_RELEASE PLUGINS_LIB_DEBUG )
    
    # IF( WOSG_USE_STATIC_LIBRARY)
        # MESSAGE(WARING ${PLUGINS_LIB_RELEASE})
        # MESSAGE(WARING ${PLUGINS_LIB_DEBUG})
        
        # list(APPEND WOSGEARTH_LIBRARY_ALL_PLUGINS   optimized ${PLUGINS_LIB_RELEASE})
        # list(APPEND WOSGEARTH_LIBRARY_ALL_PLUGINS   debug ${PLUGINS_LIB_DEBUG})
        
        if(PLUGINS_LIB_RELEASE AND PLUGINS_LIB_DEBUG)
            list(APPEND WOSG_LIBRARY_ALL optimized ${PLUGINS_LIB_RELEASE} debug ${PLUGINS_LIB_DEBUG}  )
        elseif(PLUGINS_LIB_RELEASE)
            list(APPEND WOSG_LIBRARY_ALL ${PLUGINS_LIB_RELEASE} )
        elseif(PLUGINS_LIB_DEBUG)
            list(APPEND WOSG_LIBRARY_ALL ${PLUGINS_LIB_DEBUG} )
        endif()
        
        
    # ENDIF(WOSG_USE_STATIC_LIBRARY)
    
endforeach()
    
    
MESSAGE(WARING ${WOSGEARTH_LIBRARY_ALL_PLUGINS})

