
cmake_minimum_required(VERSION 2.8)

PROJECT(libtiff)
SET(LIBRARY_NAME tiff)


OPTION(TIFF_BUILD_STATIC "Enable build static library" OFF)


SET( HEADER_FILES
    # libtiff/libtiff.def
    
    libtiff/tif_config.h
    libtiff/tiffconf.h
    
    libtiff/tiff.h
    libtiff/tiffio.h
    libtiff/tiffvers.h
)
SET( SOURCE_FILES
    ./libtiff/tif_aux.c
	./libtiff/tif_close.c
	./libtiff/tif_codec.c
	./libtiff/tif_color.c
	./libtiff/tif_compress.c
	./libtiff/tif_dir.c
	./libtiff/tif_dirinfo.c
	./libtiff/tif_dirread.c
	./libtiff/tif_dirwrite.c
	./libtiff/tif_dumpmode.c
	./libtiff/tif_error.c
	./libtiff/tif_extension.c
	./libtiff/tif_fax3.c
	./libtiff/tif_fax3sm.c
	./libtiff/tif_getimage.c
	./libtiff/tif_jbig.c
	./libtiff/tif_jpeg.c
	./libtiff/tif_jpeg_12.c
	./libtiff/tif_ojpeg.c
	./libtiff/tif_flush.c
	./libtiff/tif_luv.c
	./libtiff/tif_lzw.c
	./libtiff/tif_next.c
	./libtiff/tif_open.c
	./libtiff/tif_packbits.c
	./libtiff/tif_pixarlog.c
	./libtiff/tif_predict.c
	./libtiff/tif_print.c
	./libtiff/tif_read.c
	./libtiff/tif_stream.cxx
	./libtiff/tif_swab.c
	./libtiff/tif_strip.c
	./libtiff/tif_thunder.c
	./libtiff/tif_tile.c
	./libtiff/tif_version.c
	./libtiff/tif_warning.c
	./libtiff/tif_write.c
	./libtiff/tif_zip.c 
)

if(WIN32)
    SET( SOURCE_FILES ${SOURCE_FILES} ./libtiff/tif_win32.c)
else(WIN32)
    SET( SOURCE_FILES ${SOURCE_FILES} ./libtiff/tif_unix.c)
endif(WIN32)

IF(MSVC)
    SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
    SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/libtiff/tif_config.vc.h ${PROJECT_SOURCE_DIR}/libtiff/tif_config.h COPYONLY)
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/libtiff/tiffconf.vc.h ${PROJECT_SOURCE_DIR}/libtiff/tiffconf.h COPYONLY)
ELSEIF(MINGW)
    SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
    SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/libtiff/tif_config.mingw.h ${PROJECT_SOURCE_DIR}/libtiff/tif_config.h COPYONLY)
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/libtiff/tiffconf.mingw.h ${PROJECT_SOURCE_DIR}/libtiff/tiffconf.h COPYONLY)
ELSEIF(ANDROID) #android
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/libtiff/tif_config.ndk.h ${PROJECT_SOURCE_DIR}/libtiff/tif_config.h COPYONLY)
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/libtiff/tiffconf.ndk.h ${PROJECT_SOURCE_DIR}/libtiff/tiffconf.h COPYONLY)
ELSEIF(NACL)
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/libtiff/tif_config.nc.h ${PROJECT_SOURCE_DIR}/libtiff/tif_config.h COPYONLY)
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/libtiff/tiffconf.nc.h ${PROJECT_SOURCE_DIR}/libtiff/tiffconf.h COPYONLY)
ELSEIF(EMSCRIPTEN)
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/libtiff/tif_config.emcc.h ${PROJECT_SOURCE_DIR}/libtiff/tif_config.h COPYONLY)
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/libtiff/tiffconf.emcc.h ${PROJECT_SOURCE_DIR}/libtiff/tiffconf.h COPYONLY)
ELSE(MSVC)
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/libtiff/tif_config.h.in ${PROJECT_SOURCE_DIR}/libtiff/tif_config.h COPYONLY)
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/libtiff/tiffconf.h.in ${PROJECT_SOURCE_DIR}/libtiff/tiffconf.h COPYONLY)
ENDIF(MSVC)


if( TIFF_BUILD_STATIC )
    # add_definitions(-DJPEG_LIBRARY_STATIC)
    ADD_LIBRARY(${LIBRARY_NAME} STATIC  ${SOURCE_FILES})
ELSE( TIFF_BUILD_STATIC )
    # add_definitions(-DJPEG_LIBRARY)
    ADD_LIBRARY(${LIBRARY_NAME} SHARED  ${SOURCE_FILES} ./libtiff/libtiff.def )
ENDIF( TIFF_BUILD_STATIC )

# ADD_LIBRARY(${LIBRARY_NAME} SHARED  ./libtiff/libtiff.def ${SOURCE_FILES})

INSTALL(TARGETS ${LIBRARY_NAME} 
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        )
INSTALL(
        FILES       ${HEADER_FILES}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        )

        