
cmake_minimum_required(VERSION 2.8)

PROJECT(lua)
SET(LIBRARY_NAME lua)

OPTION(LUA_BUILD_STATIC "Enable build static library" OFF)


SET( LUA_HEADERS
    src/lua.h 
    src/luaconf.h 
    src/lualib.h 
    src/lauxlib.h 
    src/lua.hpp
)
SET( LUA_SOURCE 
    src/lapi.c
    src/lauxlib.c
    src/lbaselib.c
    src/lbitlib.c
    src/lcode.c
    src/lcorolib.c
    src/lctype.c
    src/ldblib.c
    src/ldebug.c
    src/ldo.c
    src/ldump.c
    src/lfunc.c
    src/lgc.c
    src/linit.c
    src/liolib.c
    src/llex.c
    src/lmathlib.c
    src/lmem.c
    src/loadlib.c
    src/lobject.c
    src/lopcodes.c
    src/loslib.c
    src/lparser.c
    src/lstate.c
    src/lstring.c
    src/lstrlib.c
    src/ltable.c
    src/ltablib.c
    src/ltm.c
    src/lua.c
    src/lundump.c
    src/lvm.c
    src/lzio.c
)

IF(MSVC)
    SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")
    SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
    # CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/jconfig.vc ${PROJECT_SOURCE_DIR}/jconfig.h COPYONLY)
ElSE(MSVC) #android
    # CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/jconfig.cfg ${PROJECT_SOURCE_DIR}/jconfig.h COPYONLY)
ENDIF(MSVC)


if( LUA_BUILD_STATIC )
    # add_definitions(-DLUA_LIBRARY_STATIC)
    add_library(${LIBRARY_NAME} STATIC ${LUA_HEADERS} ${LUA_SOURCE})
ELSE( LUA_BUILD_STATIC )
IF(WIN32)
    add_definitions(-DLUA_BUILD_AS_DLL)
ENDIF(WIN32)
    add_definitions(-DLUA_LIB)
    add_definitions(-DLUA_CORE)
    add_library(${LIBRARY_NAME} SHARED ${LUA_HEADERS} ${LUA_SOURCE})
ENDIF( LUA_BUILD_STATIC )


INSTALL(TARGETS ${LIBRARY_NAME} 
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        )
INSTALL(
        FILES       ${HEADERS}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        )

        