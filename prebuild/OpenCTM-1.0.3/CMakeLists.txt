
cmake_minimum_required(VERSION 2.8)

PROJECT(openctm)
SET(LIBRARY_NAME openctm)

OPTION(OPENCTM_BUILD_STATIC "Enable build static library" OFF)

include_directories(lib/liblzma)

SET( OPENCTM_HEADERS
    lib/internal.h
    lib/openctm.h
    lib/openctmpp.h

)
SET( OPENCTM_SOURCE 
    lib/compressMG1.c
    lib/compressMG2.c
    lib/compressRAW.c
    lib/openctm.c
    lib/stream.c
    
    lib/liblzma/Alloc.c
    lib/liblzma/LzFind.c
    lib/liblzma/LzmaDec.c
    lib/liblzma/LzmaEnc.c
    lib/liblzma/LzmaLib.c
)

IF(MSVC)
    SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
    SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
    # CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/jconfig.vc ${PROJECT_SOURCE_DIR}/jconfig.h COPYONLY)
ELSEIF(MSYS AND MINGW)
    SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
    SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
    # CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/jconfig.vc ${PROJECT_SOURCE_DIR}/jconfig.h COPYONLY)
ELSEIF(ANDROID) #android
    # CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/jconfig.vc ${PROJECT_SOURCE_DIR}/jconfig.h COPYONLY)
ELSE(MSVC)
    # CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/jconfig.cfg ${PROJECT_SOURCE_DIR}/jconfig.h COPYONLY)
ENDIF(MSVC)

if( OPENCTM_BUILD_STATIC )
    add_definitions(-DOPENCTM_STATIC)
    ADD_LIBRARY(${LIBRARY_NAME} STATIC ${OPENCTM_HEADERS} ${OPENCTM_SOURCE})
ELSE( OPENCTM_BUILD_STATIC )
    add_definitions(-DOPENCTM_BUILD)
    ADD_LIBRARY(${LIBRARY_NAME} SHARED ${OPENCTM_HEADERS} ${OPENCTM_SOURCE})
ENDIF( OPENCTM_BUILD_STATIC )


SET(INSTALL_INCDIR include)
SET(INSTALL_BINDIR bin)
IF(WIN32)
    SET(INSTALL_LIBDIR bin)
    SET(INSTALL_ARCHIVEDIR lib)
ELSE(WIN32)
    SET(INSTALL_LIBDIR lib)
    SET(INSTALL_ARCHIVEDIR lib)
ENDIF(WIN32)

INSTALL(TARGETS ${LIBRARY_NAME} 
        RUNTIME DESTINATION ${INSTALL_BINDIR}
		LIBRARY DESTINATION ${INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${INSTALL_ARCHIVEDIR}
        )
INSTALL(
        FILES       ${LIBJPEG_HEADERS}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        )

        