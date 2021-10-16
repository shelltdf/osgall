
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

if (EMSCRIPTEN)
     set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread")
     set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")
endif (EMSCRIPTEN)

if (NOT WIN32)
     set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread")
     set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")
endif (NOT WIN32)

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

if (NOT WIN32)
    target_link_libraries(${LIBRARY_NAME} dl)
endif (NOT WIN32)

        
if(NOT EMSCRIPTEN)

add_executable( ${APP_NAME} shell.c )
target_link_libraries(${APP_NAME} ${LIBRARY_NAME})

INSTALL(TARGETS ${APP_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION lib
        )
endif()


INSTALL(
        FILES       ${LIBSQLITE_HEADERS}
        DESTINATION include
        )
        
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
