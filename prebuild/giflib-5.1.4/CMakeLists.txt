
cmake_minimum_required(VERSION 2.8)

PROJECT(giflib)
SET(LIBRARY_NAME giflib)

OPTION(GIFLIB_BUILD_STATIC "Enable build static library" ON)

SET( LIBJPEG_HEADERS
    # lib/gif_lib_private.h
    lib/gif_lib.h
    # lib/gif_hash.h

)
SET( LIBJPEG_SOURCE 
    lib/dgif_lib.c
    lib/egif_lib.c
    lib/gif_err.c
    lib/gif_font.c
    lib/gif_hash.c
    lib/gifalloc.c
    lib/openbsd-reallocarray.c
    lib/quantize.c
)

# IF(MSVC)
    # SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
    # SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
    # CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/jconfig.vc ${PROJECT_SOURCE_DIR}/jconfig.h COPYONLY)
# ELSEIF(MSYS AND MINGW)
    # SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
    # SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
    # CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/jconfig.vc ${PROJECT_SOURCE_DIR}/jconfig.h COPYONLY)
# ElSE(MSVC) #android
    # CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/jconfig.cfg ${PROJECT_SOURCE_DIR}/jconfig.h COPYONLY)
# ENDIF(MSVC)

if( GIFLIB_BUILD_STATIC )
    add_definitions(-DGIF_LIBRARY_STATIC)
    ADD_LIBRARY(${LIBRARY_NAME} STATIC ${LIBJPEG_HEADERS} ${LIBJPEG_SOURCE})
ELSE( GIFLIB_BUILD_STATIC )
    add_definitions(-DGIF_LIBRARY)
    ADD_LIBRARY(${LIBRARY_NAME} SHARED ${LIBJPEG_HEADERS} ${LIBJPEG_SOURCE})
ENDIF( GIFLIB_BUILD_STATIC )

SET_TARGET_PROPERTIES(${LIBRARY_NAME}  PROPERTIES DEBUG_POSTFIX "d")

if(WIN32)
INSTALL(TARGETS ${LIBRARY_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION lib
        )
else()
INSTALL(TARGETS ${LIBRARY_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )
endif()
INSTALL(
        FILES       ${LIBJPEG_HEADERS}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        )

        