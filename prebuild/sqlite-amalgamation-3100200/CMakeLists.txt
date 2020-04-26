
cmake_minimum_required(VERSION 2.8)

PROJECT(sqlite)
SET(LIBRARY_NAME sqlite)
SET(APP_NAME sqlite3)

IF(MSVC)
    SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
    SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
ELSEIF(MSYS AND MINGW)
    SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
    SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
ElSE(MSVC) #android
ENDIF(MSVC)

OPTION(SQLITE_BUILD_STATIC "Enable build static library" OFF)

SET( LIBSQLITE_HEADERS
    sqlite3ext.h
    sqlite3.h
)
SET( LIBSQLITE_SOURCE
    sqlite3.c
)

if( SQLITE_BUILD_STATIC )
    add_definitions(-DSQLITE_LIBRARY_STATIC)
    ADD_LIBRARY(${LIBRARY_NAME} STATIC ${LIBSQLITE_HEADERS} ${LIBSQLITE_SOURCE})
ELSE( SQLITE_BUILD_STATIC )
    add_definitions(-DSQLITE_LIBRARY)
    # add_definitions(-DSQLITE_CORE)
    ADD_LIBRARY(${LIBRARY_NAME} SHARED ${LIBSQLITE_HEADERS} ${LIBSQLITE_SOURCE} sqlite3.def)
ENDIF( SQLITE_BUILD_STATIC )


add_executable( ${APP_NAME} shell.c )
target_link_libraries(${APP_NAME} ${LIBRARY_NAME})

INSTALL(
        FILES       ${LIBSQLITE_HEADERS}
        DESTINATION include
        )
        
INSTALL(TARGETS ${LIBRARY_NAME} ${APP_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION lib
        )
