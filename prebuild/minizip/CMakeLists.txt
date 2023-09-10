
cmake_minimum_required(VERSION 2.8)

PROJECT(minizip)
SET(LIBRARY_NAME minizip)

find_package(zlib)

OPTION(MINIZIP_BUILD_STATIC "Enable build static library" OFF)

set(MINIZIP_PATH ${CMAKE_SOURCE_DIR}/../zlib-1.2.8/contrib/minizip)

SET( MINIZIP_HEADERS
    ${MINIZIP_PATH}/crypt.h
    ${MINIZIP_PATH}/ioapi.h
    ${MINIZIP_PATH}/iowin32.h
    ${MINIZIP_PATH}/mztools.h
    ${MINIZIP_PATH}/unzip.h
    ${MINIZIP_PATH}/zip.h
)
SET( MINIZIP_SOURCE 
    ${MINIZIP_PATH}/ioapi.c
    ${MINIZIP_PATH}/iowin32.c
    # ${MINIZIP_PATH}/miniunz.c
    # ${MINIZIP_PATH}/minizip.c
    ${MINIZIP_PATH}/mztools.c
    ${MINIZIP_PATH}/unzip.c
    ${MINIZIP_PATH}/zip.c
)

IF(MSVC)
    SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
    SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
ELSEIF(MSYS AND MINGW)
    SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
    SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
ELSEIF(ANDROID) #android
ELSE(MSVC)
ENDIF(MSVC)

include_directories(${ZLIB_INCLUDE_DIR})

if( MINIZIP_BUILD_STATIC )
    add_definitions(-DMINIZIP_LIBRARY_STATIC)
    ADD_LIBRARY(${LIBRARY_NAME} STATIC ${MINIZIP_HEADERS} ${MINIZIP_SOURCE})
ELSE( MINIZIP_BUILD_STATIC )
    add_definitions(-DMINIZIP_LIBRARY)
    ADD_LIBRARY(${LIBRARY_NAME} SHARED ${MINIZIP_HEADERS} ${MINIZIP_SOURCE})
ENDIF( MINIZIP_BUILD_STATIC )

target_link_libraries(${LIBRARY_NAME} 
        ${ZLIB_LIBRARIES}
        )

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
        FILES       ${MINIZIP_HEADERS}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        )

        